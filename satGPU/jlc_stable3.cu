#include <stdio.h>
#include <time.h>
#if !(defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
#include <sys/time.h>
#include <sys/types.h>
#else
 #ifndef uint
 typedef unsigned int uint;
 #endif
#endif
#include <vector>
using namespace std;


#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
clock_t global_time;
#else
struct timeval time_start,time_stop;
#endif

//INITandy
// strategie per la variable selection in CPU (mask_prop())
#define NONE -1
#define MINCLAUSE 0
#define JEROWANGTWOSIDED 1
#define JEROWANGONESIDED 2
#define DLIS 3
#define DLCS 4
#define DEFAULT_VARSTRAT MINCLAUSE
//opzione per il sorting delle variabili unknown prima di lanciare il kernel
// nel mode 2. Si utilizza la euristica usata per la variable selection in CPU.
#define MAPSVARSORT_NO 0
#define MAPSVARSORT_YES 1
#define DEFAULT_MAPSVARSORT MAPSVARSORT_NO
//FINEandy


//*** IMPORTANT KERNEL STATIC OPTION.
//*** UnComment/Comment the following line
// In the first case (parameter defined) it simply assigns the block variables.
// In the second case it performs fast unit propagation after any assignment
#define SIMPLEBLOCK 0

//**** STATIC SIZE FOR block_vars and delta_vars
//**** ARRAYS USED IN PARALLELIZING SEARCH  // erano 101 e 110
#define  BV_SIZE 171 
#define  DV_SIZE 170 
#define  DV_SIZE_W 500 

//*** Comment/Uncomment the following line for information
//*** on memory allocation during CUDA execution
//#define MYTRACE 0

// The following PARAMETERS decide the parallelism degree
// and the complexity of the formula to be delagetd to CUDA
// We allow two versions, with static (#define) and dynamic values

//INITandy
int NUMPARAMETERS = 8; 
//#define TRACESELECT 0
//FINEandy

// *** PARAMETER #1 (argv 1)
// *** Modes for using GPU. Currently 0-3
int USEGPU = 0;   // The default value 

// *** PARAMETER #2 (argv 2)
// NUMBER OF BLOCKS = CUDABLOCKS
// LOG_BLKS is the number of vars fixed by the block address
int LOG_BLKS=2;
int CUDABLOCKS=(1<<LOG_BLKS); 

// *** PARAMETER #3 (argv 3)
// NUMBER OF THREADS per BLOCK  = CUDATHREADS 
// LOG_THRDS is the number of vars fixed by the thread
// In the current GPUs 10 is the maximum value (1024 threads per block)
int LOG_THRDS=5; 
int CUDATHREADS=(1<<LOG_THRDS); 

// *** PARAMETER #4 (argv 4)
// MaxV is the max number of non ground variables delegated to the GPU
int MaxV=50;  

//INITandy
// *** PARAMETER #6 (argv 6)
//strategia per select delle variabili
int strategia_varSelHost = DEFAULT_VARSTRAT;
// gestione delle strategie:
uint * host_strat_vars_weight; // "pesi" delle variabili (unknown)
int * host_strat_vars_Oneocc; // un idx di una delle clausole contententi la variabile
uint * host_strat_vars_clausesize; // dimensione della clausola (num. variabili unknown)

// *** PARAMETER #7 (argv 7)
// flag per attivare il riordono delle var prima di invocare il kernel in mode 2
int strategia_maps_varSort = DEFAULT_MAPSVARSORT;
// vettore di permutazione delle variabili (usato in filter_formula()):
int * permutator_maps_vars;
//FINEandy



// Delta is the number of variables treated ND by a CUDA thread. 
// The "+1" is needed since delta_vars[0] is not used!
int Delta= MaxV - LOG_BLKS + 1;   

// *** STATIC LIMITS TO FORMULA SIZE (before and after learning)

int MAX_NL   = 8000000;  
int MAX_NC   = 550000;
int nclausel = 0;

// *** PARAMETERS FOR FOR PARALLELIZING UNIT PROPAGATION
//   THREADS  (typically, 256, 512, 1024)
#define THREADS 512
//  Number of blocks used (assigned later using instance and HW data)
int PARblocks=1;          



//*********************************************
// GLOBAL ARRAYS USED
//*********************************************

int * formula;            // Formula and vars in the host    
int * clause_pointer;     // 
int * filtered_formula;   // Reduced formula in the tail 
int * filtered_clause_pointer; // stage
int * dev_formula;        // Formula to be passed in the device
int * dev_cp;             // in the UP 
int * dev_mapped_formula; // Formula in the device
int * dev_mapped_cp;      // in the unit propagation
int * host_vars;          // variables in the host
int * filtered_vars;      // Device vars (tail)
int * dev_vars;           // Device vars (u.p.)
int * dev_parma_vars;     // Device vars (u.p.)

// mode 5
int * dev_vars_w;         // unit watched
int * dev_v2c_w;         // unit watched
int * dev_w_w;
int * dev_next_w;
int * dev_unit_w;
int * dev_c2v_w;
int * dev_varstack_w;

//*********************************************

int * mask_data; //*** unificato per unico trasferimento
int * h_mask_data; 
int * maps_vars;

// *** stack/trail information

int * level;
int * learnt_clause;
int * seen;
int * refs;
int * trail;

int learning      =1;
int level_to_jump = -2; // in caso di backtracking/jumping

// *** GLOBAL VARIABLES

int NV,NC,NL;        // Formula parameters
int CUDA_count=0;    // It counts the calls to a device
int CUDA_count_lower=0;    // It counts the calls to a device (option 1 and 3)
int backtracking=0;  // It counts the backtrackings in the host
int backjump=0;  // It counts the backtrackings in the host
int sat_val,clauind,selected_var; // Again, for backtracking purposes
float deltatime;
cudaEvent_t start, stop;

//*** Error Handling
__global__ void parmask_prop( int* mask_data, int* vars,int* dev_mapped_formula,int* dev_mapped_cp, int g_tn, int dbg);
static void HandleError(cudaError_t err,const char *file,int line){
    if (err != cudaSuccess){
      printf("%s in %s at line %d\n",cudaGetErrorString(err),file,line);
      printf("ncl %d at lev %d\n",nclausel,level[0]);
      for(int i=1;i<trail[0];i++)
           printf("%d@%dr%d,",trail[i],level[abs(trail[i])],refs[abs(trail[i])]);
      printf("\n");
      exit( EXIT_FAILURE );}}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))
#define HANDLE_NULL( a ){if (a == NULL) { \
     printf( "Host memory failed in %s at line %d\n", __FILE__, __LINE__ ); \
     exit( EXIT_FAILURE );}}
#define IMIN(a,b) (a<b?a:b)
#define IMAX(a,b) (a>b?a:b)


void checkCUDAError(const char *msg) {
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        fprintf(stderr, "ERRORE CUDA: >%s<: >%s<.\n", msg, cudaGetErrorString(err) );
        exit(-1);
    }                         
}



// *** ALLOCATE / DEALLOCATE

__host__ void allocate_first(){
//*** SIZE INDEPENDENT ON THE FORMULA  
  if (USEGPU>0){
    HANDLE_ERROR( cudaHostAlloc( (void**)&formula,         MAX_NL * sizeof(int), cudaHostAllocDefault ) );
    HANDLE_ERROR( cudaMalloc( (void**)&dev_mapped_formula, MAX_NL * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_formula,         MAX_NL * sizeof(int)));
    HANDLE_ERROR( cudaHostAlloc( (void**)&clause_pointer, (MAX_NC+1) * sizeof(int), cudaHostAllocDefault ) );
    HANDLE_ERROR( cudaMalloc( (void**)&dev_mapped_cp,     (MAX_NC+1) * sizeof(int) ));
    HANDLE_ERROR( cudaMalloc((void**)&dev_cp,             (MAX_NC+1) * sizeof(int) ));

    // mode 5
    HANDLE_ERROR( cudaMalloc((void**)&dev_vars_w,          DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_v2c_w,          2*DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_w_w,          2*DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_next_w,          2*DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_unit_w,          DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_c2v_w,          2*DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_varstack_w,          2*DV_SIZE_W*((CUDABLOCKS*CUDATHREADS)) * sizeof(int)));

  }
  else{
    formula=(int*)malloc(MAX_NL * sizeof(int));
    clause_pointer=(int*)malloc((MAX_NC+1) * sizeof(int));
  }
  filtered_clause_pointer=(int*) malloc((MAX_NC+1) * sizeof(int));
  filtered_formula       =(int*) malloc(MAX_NL * sizeof(int));
}    
    
__host__ void allocate_second(){
#ifdef MYTRACE
    printf("Memory allocation (in main):\n");
    printf("    (cuda)                     phi:\t%d bytes for %d literals\n", NL * sizeof(int), NL);
    printf("    (cuda)                      cp:\t%d bytes for %d clauses\n", (NC + 1) * sizeof(int), NC);
    printf("    (cuda)               host_vars:\t%d bytes for %d vars\n", NV * sizeof(int), NV);
    printf("    (cuda)          dev_parma_vars:\t%d bytes for %d vars\n", NV * sizeof(int), NV);
    printf(" (host)                 h_mask_num:\t%d bytes for %d blocks\n", PARblocks * sizeof(int), PARblocks);
    printf(" (host)                  h_mask_id:\t%d bytes for %d blocks\n", PARblocks * sizeof(int), PARblocks);
    fflush(stdout);
#endif

//*** SIZE DEPENDING ON THE FORMULA
//INITandy
    if ( (strategia_varSelHost == JEROWANGONESIDED) || (strategia_varSelHost == DLIS)) {
	host_strat_vars_weight=(uint*)malloc((2*NV) * sizeof(uint));
	host_strat_vars_Oneocc=(int*)malloc((2*NV) * sizeof(int));
	memset(host_strat_vars_weight,((uint)0),(2*NV)*sizeof(uint));
	memset(host_strat_vars_Oneocc,((int)0),(2*NV)*sizeof(int));
    }
    if ( (strategia_varSelHost == JEROWANGTWOSIDED) || (strategia_varSelHost == DLCS) ) {
	host_strat_vars_weight=(uint*)malloc(NV * sizeof(uint));
	host_strat_vars_Oneocc=(int*)malloc(NV * sizeof(int));
	memset(host_strat_vars_weight,((uint)0),NV*sizeof(uint));
	memset(host_strat_vars_Oneocc,((int)0),NV*sizeof(int));
    }
    if ( (strategia_varSelHost == MINCLAUSE) && (strategia_maps_varSort == MAPSVARSORT_YES) ) {
	host_strat_vars_weight=(uint*)malloc(NV * sizeof(uint));
	memset(host_strat_vars_weight,((uint)0),NV*sizeof(uint));
    }
    permutator_maps_vars = (int*) malloc(NV * sizeof( int));
//FINEandy

    host_vars=(int*)malloc(NV * sizeof(int)); 

    if (USEGPU>0){
      HANDLE_ERROR( cudaHostAlloc( (void**)&h_mask_data, (3* PARblocks+NV) * sizeof(int),  cudaHostAllocDefault ) ); //questo non serve su solo cpu 
      HANDLE_ERROR( cudaMalloc((void**)&dev_vars,              NV * sizeof( int) )) ;
      HANDLE_ERROR( cudaMalloc( (void**)&dev_parma_vars,       NV * sizeof(int) ) );
      HANDLE_ERROR( cudaMalloc( (void**)&mask_data,      (3* PARblocks+NV) * sizeof(int) ) );
    }
    filtered_vars = (int*) malloc(NV * sizeof( int));
    maps_vars     = (int*) malloc(NV * sizeof( int));
    level         = (int*) malloc(NV * sizeof(int));
    refs          = (int*) malloc(NV * sizeof(int));
    trail         = (int*) malloc(NV * sizeof(int));
    seen          = (int*) malloc(NV * sizeof(int));
    learnt_clause = (int*) malloc(NV * sizeof(int)); // AGO" giusto NV?
    // *** Initialization:
    if (USEGPU>0){
    HANDLE_ERROR( cudaMemcpy( dev_mapped_formula, formula,    NL * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( dev_mapped_cp, clause_pointer, (NC+1) * sizeof(int), cudaMemcpyHostToDevice ) );
    }
    for(int i=0;i<NV;i++){host_vars[i]=-1; level[i] = -1; refs[i] = -1; seen[i]=0;}
    trail[0] = 1; 
    level[0] = 0;
}

__host__ void deallocate() {      
  free( host_vars );  //AGO: solo queste deallochiamo?
  if (USEGPU>0){
    HANDLE_ERROR( cudaFree( mask_data ));      //
    HANDLE_ERROR( cudaFree(dev_vars));
    HANDLE_ERROR( cudaFree(dev_formula));
    HANDLE_ERROR( cudaFree(dev_cp));
    cudaEventDestroy( start );
    cudaEventDestroy( stop );
  }
}
    

//**************************************************
// INPUT: 
// Data Structure: the formula is stored in two vectors.
// The first stores the literals in sequence. 
// Its length is NL (0..NL-1)
// The second stores the beginning of the various clauses
// The number of clauses is NC. The vector is 0..NC
// The last (redundant) cell stores the value NL
// It also returns NV = 1+number of vars in the formula
// Remark: the formula is read twice 
//**************************************************

__host__ int simple_parsing(char c, FILE *miofile){
  int sum=c-'0';
  char t = fgetc(miofile);
  while (47 <t  && t < 58){
       sum=sum*10+t-48;
       t = fgetc(miofile);
  }
  return sum;   
}

__host__ void leggilinea(FILE *miofile){
  char t = fgetc(miofile);
  while (t != 13 && t != 10) // Look for CR or NL
       t = fgetc(miofile);
}

__host__ void load_formula(char *filename, int *NV, int *NC,int *NL){
  int i=0,j=1,max = 0,flag=0;
  char t;
  FILE *miofile;

  clause_pointer[0]=0;  
  if((miofile = fopen(filename,"r"))==NULL) {
     printf("File not found >%s< (premature exit)\n",filename); exit(-1);
  }
  else { 
    while(!feof(miofile)){
      t = fgetc(miofile);
      if (t=='c' || t=='p' || t=='%')
         leggilinea(miofile); // Skip comments
      else if (t=='-'){       
         flag=1;
         formula[i] = -simple_parsing(48,miofile);
         max = IMAX(max,-formula[i]); 
         i++;
      }      
      else if ('0' <t  && t <= '9'){
         flag=1;
         formula[i] = simple_parsing(t, miofile);
         max = IMAX(max,formula[i]); 
         i++;
      } 
      else if (t == '0' && flag==1){
         clause_pointer[j]=i; j++; flag=0;
      }   
    }            
    printf("Num clauses: %d, Num vars: %d, Num literals: %d \n",j-1,max,i);       
    *NV = max+1;   *NC = j-1;    *NL = i;
    fclose(miofile);     
  } // else  
}

//**************************************************
// OUTPUT functions: 
//**************************************************

//INITandy
void verify_solution(int *vars) {
	int cissat=0; int i,j;
	int fissat=1;
	for (i=0; i < NC; i++) {
		cissat=1;
		j=clause_pointer[i];
		while (j<clause_pointer[i+1]) {
			if ((formula[j]<0 && vars[-formula[j]]!=1)||(formula[j]>0 && vars[formula[j]]!=0)) {
				cissat=1;
				break;
			}
		j++;
		}
		if (cissat == 0) {
			printf("clausola %d violata\n",i);
			fissat = 0;
			break;
		}
	}
	printf("\nSOLUTION CHECK (0==False, 1==True, -1==Undef): %s\n\n",((fissat==1)?"***OK***":"### ALERT ### FORMULA NOT SATISFIED!!! #####"));fflush(stdout);
}


void print_option (int option) {
switch (option) {
	//case NONE: { printf("NONE"); break; }
	case MINCLAUSE: { printf("MINCLAUSE"); break; }
	case JEROWANGTWOSIDED: { printf("JEROWANGTWOSIDED"); break; }
	case JEROWANGONESIDED: { printf("JEROWANGONESIDED"); break; }
	case DLCS: { printf("DLCS"); break; }
	case DLIS: { printf("DLIS"); break; }
	default: { printf("DEFAULT"); break; }
}
}
//FINEandy

__host__ void  print_info(cudaDeviceProp prop){
    printf("INFO: Variable reordering strategy (before calling the device): %s\n", (strategia_maps_varSort==MAPSVARSORT_YES)?"YES":"NO");fflush(stdout);
//    printf("INFO: multiproc %d\n",prop.multiProcessorCount);
//    printf("INFO: sh mem per block %d\n",(int)prop.sharedMemPerBlock);
//    printf("INFO: threads per block %d\n",prop.maxThreadsPerBlock);
    printf("INFO: kernelExecTimeoutEnabled %d\n",prop.kernelExecTimeoutEnabled);
//    printf("INFO: max tex 1d %d\n",prop.maxTexture1D);
//    printf("INFO: max tex 2d %d %d\n",prop.maxTexture2D[0],prop.maxTexture2D[1]);
//    printf("INFO: tex align %d\n",(int)prop.textureAlignment);
    printf("INFO: Block Allocation ");
    #ifdef SIMPLEBLOCK
        printf("simple\n");
    #else
        printf("with unit propagation\n");
    #endif
}

__host__ void print_instance( int * phi, int * cp, int NC){
      int i=0,j=1; 
      printf("SAT INSTANCE: \n");
      while(j <= NC){
         printf("%d ",phi[i]);
         i++;
         if(i == cp[j]){printf("\n");j++;}
      }   
      printf("*****************\n"); 
}       

__host__ void print_result(int *vars, int NV){
  printf("\n %d GPU mask prop %d GPU lower tree search, %d backtracks, %d backjumps, %d clauses learnt\n",CUDA_count,CUDA_count_lower,
	 backtracking,backjump,nclausel);
    if (vars[0]){
         for(int i=1;i<NV;i++){
           printf( "V[%d]=%d", i, vars[i]);
           if (i < NV-1) printf(", ");
           if (i % 7 == 0) printf("\n");
        }
        printf("\n");
	verify_solution(vars);
    }    
    else if  (vars[0]==-1)
         printf( "Computation ended by timeout\n");
    else printf( "No solution\n");
}    

// print_time prints a time expressed in ms 

__host__ void print_time(float time){
    //int MIN;
    //if (time>=60000){
    //      MIN = (int)time/60000;
    //      time = (time - MIN*60000)/1000;
    //      printf("Execution Time: %d m %.5f s\n",MIN,time);
    //}      
    //else
     printf("Execution Time: %.5f sec\n",time/1000);
}


//*****************************************************
//*** PROPAGATION PHASE
//*****************************************************

//INITandy
// unit propagation is implemented by mask_prop (in one of its variants).
// Given a (partial) assignment "host_vars", for each clause i, 
// mask= 0 => clause is true 
// mask=-1 => clause i is false 
// mask> 0 => clause i can be satisfied (there are unground variables)
// *****************************************************************
// return  0 if all clauses are satisfied
// return -1 if there is a false clause
// return  >0  otherwise and in this case *clauseind is the index of a
// **** clause containing *sel_var 
// **** sel_var is a pointer to the (selected) unassigned literal
// **** *minmask is a weight of the selected clause/literal
// ******************************************************************

//  BIG_EXP meno di __WORDSIZE 
// usato in JEROWANG*  (2^BIG_EXP e' il contributo max di una occorrenza di una var)
#define BIG_EXP 20

__host__ int mask_prop(int* sel_var, int* minmask, int* clauseind){
  int i,j,min=0,t,mask=0,pt,bestpt=0,bestcl=0;
  bool initial=true;
  int maybe_sat; // 2 surely sat, 1 maybe, 0 unsat
  
  //***********************************************************************
  //****** SIMPLE CASE (coherent with mask_prop_delta and PARMASKprop)
  //***********************************************************************
  // *** AGO: some little speed-up can be obtained storing pt=t instead of
  // *** pt = j (and changing all tests accordingly - basically removing formula[...]). 
  // *** Left this way to be fully consistent with mask_prop_delta
  //***********************************************************************

*clauseind=0;
*minmask=0;

switch (strategia_varSelHost) {

/*
case NONE: { // prende la prima che trova
  i=0;    
  while((mask >= 0) && (i <NC)){
    mask = 0;   
    j=clause_pointer[i];
    maybe_sat = 0;
    pt = 0; // Greater than number of vars
    while((maybe_sat < 2) && (j<clause_pointer[i+1])) {
      t=formula[j]; // t = literal in position j 
      if ((t<0 && host_vars[-t]==0)||(t>0 && host_vars[t]==1)){
	mask      = 0;
	maybe_sat = 2; 
	break;
      }  
      else if (host_vars[abs(t)] < 0) { // Literal Unknown 
	mask++; 
	if (pt == 0 || (pt > 0 && abs(t) < abs(formula[pt]))) 
	  pt=j;  // ==> Here the remark above
	j++;
	maybe_sat = 1;
      } 
      else j++;
    }  // while
    //if (j==clause_pointer[i+1] && maybe_sat==0) {mask = -1;  }// EOC: UNSAT!
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1;
	    sel_var=NULL; //SERVE avere uno dei lit di clauseind?
	    *clauseind=i;
#ifdef TRACESELECT
    printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (mask != 0) { //trovata
       min=mask; bestpt=pt; bestcl=i; break;
    }
    i++;     
  } // while principale
  //  OUTPUT 
  *sel_var  =  formula[bestpt];
  *minmask =  min;
  *clauseind=bestcl;
#ifdef TRACESELECT
	printf("%s", min==0 ? "+" : (min==1 ? "u": "N"));fflush(stdout);
#endif
  return(min);        

} // FINE STRATEGIA NONE
*/

case MINCLAUSE: { //variabile che occorre nella clausola piu' corta
  i=0;    
  if (strategia_maps_varSort == MAPSVARSORT_YES) {memset(host_strat_vars_weight,((uint)0),NV*sizeof(uint)); }
//VERSIONE ORIGINALE:
/*
  while((mask >= 0) && (i <NC)){
    mask = 0;
    j=clause_pointer[i];
    maybe_sat = 0;
    pt = 0; // Greater than number of vars
    while((maybe_sat < 2) && (j<clause_pointer[i+1])) {
      t=formula[j]; // t = literal in position j 
      if ((t<0 && host_vars[-t]==0)||(t>0 && host_vars[t]==1)){
        mask      = 0;
        maybe_sat = 2;
      }
      else if (host_vars[abs(t)] < 0) { // Literal Unknown 
        mask++;
if (strategia_maps_varSort == MAPSVARSORT_YES) {
//printf("INC host_strat_vars_weight[abs(t)] t=%d\n",t); 
(host_strat_vars_weight[abs(t)])++;}

        if (pt == 0 || (pt > 0 && abs(t) < abs(formula[pt])))
          pt=j;  // ==> Here the remark above
        j++;
        maybe_sat = 1;
      }
      else j++;
    }  // while
    if (j==clause_pointer[i+1] && maybe_sat==0) mask = -1; // EOC: UNSAT!
    if (initial) {
      if (mask != 0)
        {min=mask; bestpt=pt; initial=false; bestcl=i;}
    }
    else if (mask!=0){ // not initial
      if  (mask<min || (mask==min && abs(formula[pt]) < abs(formula[bestpt])) ||
           (mask==min && abs(formula[pt]) == abs(formula[bestpt]) && i < bestcl))
        {min=mask; bestpt=pt; bestcl=i;}
    }
    i++;
  } // while principale
*/

//VERSIONE MODIFICATA:
// /*
  while((mask >= 0) && (i <NC)){
    mask = 0;   
    j=clause_pointer[i];
    maybe_sat = 0;
    pt = 0; // Greater than number of vars
    while((maybe_sat < 2) && (j<clause_pointer[i+1])) {
      t=formula[j]; // t = literal in position j 
      if ((t<0 && host_vars[-t]==0)||(t>0 && host_vars[t]==1)){
	mask      = 0;
	maybe_sat = 2; 
	break;
      }  
      else if (host_vars[abs(t)] < 0) { // Literal Unknown 
	mask++; 
	if (strategia_maps_varSort == MAPSVARSORT_YES) {(host_strat_vars_weight[abs(t)])++;}
	if (pt == 0 || (pt > 0 && abs(t) < abs(formula[pt]))) 
	  pt=j;  // ==> Here the remark above
	j++;
	maybe_sat = 1;
      } 
      else j++;
    }  // while
    //if (j==clause_pointer[i+1] && maybe_sat==0) {mask = -1;  }// EOC: UNSAT!
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1;
	    sel_var=NULL; //SERVE avere uno dei lit di clauseind?
	    *clauseind=i;
#ifdef TRACESELECT
    printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (initial) {  // non ho ancora trovato clausole/lit
      if (mask != 0){
	min=mask; bestpt=pt; initial=false; bestcl=i;
      } 
    }       
    else if (mask!=0){ // not initial
      if  ((mask<min)  || (mask==min && abs(formula[pt]) < abs(formula[bestpt])) ||
	   (mask==min && abs(formula[pt]) == abs(formula[bestpt]) && i < bestcl))
	{min=mask; bestpt=pt; bestcl=i;}
    }
    i++;     
  } // while principale
// */

  //*** OUTPUT 
  *sel_var  =  formula[bestpt];
  *minmask =  min;
  *clauseind=bestcl;
#ifdef TRACESELECT
	printf("%s", min==0 ? "+" : (min==1 ? "u": "M"));fflush(stdout);
#endif
  return(min);        

} // FINE STRATEGIA MIN-CLAUSE



case JEROWANGTWOSIDED: {
  *minmask=0;
  i=0;    
  memset(host_strat_vars_weight,((uint)0),NV*sizeof(uint));
  //memset(host_strat_vars_Oneocc,((int)0),NV*sizeof(int)); //NONSERVE
  while (i < NC) {
    mask = 0;   
    maybe_sat = 0;
    j=clause_pointer[i];
    while (j<clause_pointer[i+1]) {
      if ((formula[j]<0 && host_vars[-formula[j]]==0)||(formula[j]>0 && host_vars[formula[j]]==1)){ //clausola sat
	mask      = 0;
	maybe_sat = 2;
	break; 
      } else if (host_vars[abs(formula[j])] < 0) { // Literal Unknown 
	mask++; 
	t=j;  //memo id del lit in formula[]
	maybe_sat = 1;
      } 
      j++;
    }  // while (lits della clausola)
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1; sel_var=NULL;
	    *clauseind=i;
#ifdef TRACESELECT
	printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (mask>1) {
      for (j=clause_pointer[i]; j<clause_pointer[i+1]; j++) {
	      if (host_vars[abs(formula[j])]<0) {
		      host_strat_vars_weight[abs(formula[j])] += (1<< IMAX(1,(BIG_EXP-mask)));
		      host_strat_vars_Oneocc[abs(formula[j])] = i*(formula[j]<0?-1:1);  // ANDY: gestione grezza!! DA MIGLIORARE (memo il segno del lit)
	      }
      } // for (lits della clausola)
    } else if (mask==1) {
	    *minmask=1; *clauseind=i; *sel_var=formula[t]; //t is idx of last Unkn Found;
#ifdef TRACESELECT
	printf("u");fflush(stdout);
#endif
	    return(1);
    }
    i++;
  } // while (i) principale (sulle clausole)

  //cerco atomo unknown con la best weight (riciclo la variabile bestpt)
  for (j=1; j<NV; j++) { //cerco il primo pesante 
    if (host_strat_vars_weight[j]>0) {
	    bestpt=j;
	    break;
    }
  } 
  if (j==NV) {
#ifdef TRACESELECT
	printf("+");fflush(stdout);
#endif
	  return(0); //nessun lit unknown e formula non unsat
  }
  j++;
  for (; j<NV; j++) { // cerco il piu' pesante
    if (host_strat_vars_weight[bestpt]<host_strat_vars_weight[j]) {
	    bestpt=j;
    } 
  } 
  *clauseind = abs(host_strat_vars_Oneocc[bestpt]);       // indice di una clausola (qualsiasi, ORA e' l'ultime trovata) che contenga select lit 
  *sel_var  =  bestpt * (host_strat_vars_Oneocc[bestpt]<0 ?-1:1); // segno della var selected in tale clausola
  //*sel_var  =  bestpt * (host_strat_vars_Oneocc[bestpt]>0 ?-1:1); // segno opposto rispetto a MINCLAUSE
  *minmask =  IMIN(INT_MAX,host_strat_vars_weight[bestpt]);
#ifdef TRACESELECT
	printf("J");fflush(stdout);
#endif
  return(*minmask); //basta return di un >1

} // FINE STRATEGIA JEROWANGTWOSIDED


case JEROWANGONESIDED: {
  *minmask=0;
  i=0;    
  memset(host_strat_vars_weight,((uint)0),(2*NV)*sizeof(uint));
  //memset(host_strat_vars_Oneocc,((int)0),(2*NV)*sizeof(int)); //NONSERVE
  while (i < NC) {
    mask = 0;   
    maybe_sat = 0;
    j=clause_pointer[i];
    while (j<clause_pointer[i+1]) {
      if ((formula[j]<0 && host_vars[-formula[j]]==0)||(formula[j]>0 && host_vars[formula[j]]==1)){ //clausola sat
	mask      = 0;
	maybe_sat = 2;
	break; 
      } else if (host_vars[abs(formula[j])] < 0) { // Literal Unknown 
	mask++; 
	t=j;  //memo id del lit in formula[]
	maybe_sat = 1;
      } 
      j++;
    }  // while (lits della clausola)
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1; sel_var=NULL;
	    *clauseind=i;
#ifdef TRACESELECT
	printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (mask>1) {
      for (j=clause_pointer[i]; j<clause_pointer[i+1]; j++) {
	      if (host_vars[abs(formula[j])]<0) {
		      if (formula[j]<0) {
			      host_strat_vars_weight[NV-formula[j]] += (1<< IMAX(1,(BIG_EXP-mask)));
			      host_strat_vars_Oneocc[NV-formula[j]] = i;  // la clausola in cui occorre
		      } else {
			      host_strat_vars_weight[formula[j]] += (1<< IMAX(1,(BIG_EXP-mask)));
			      host_strat_vars_Oneocc[formula[j]] = i;  // la clausola in cui occorre
		      }
	      }
      } // for (lits della clausola)
    } else if (mask==1) {
	    *minmask=1; *clauseind=i; *sel_var=formula[t]; //t is idx of last Unkn Found;
#ifdef TRACESELECT
	printf("u");fflush(stdout);
#endif
	    return(1);
    }
    i++;
  } // while (i) principale (sulle clausole)

  //cerco atomo unknown con la best weight (riciclo la variabile bestpt)
  for (j=1; j<(2*NV); j++) { //cerco il primo pesante 
    if (host_strat_vars_weight[j]>0) {
	    bestpt=j; break;
    }
  } 
  if (j==(2*NV)) {
#ifdef TRACESELECT
	printf("+");fflush(stdout);
#endif
	  return(0); //nessun lit unknown e formula non unsat
  }
  j++;
  for (; j<(2*NV); j++) { // cerco il piu' pesante
    if (host_strat_vars_weight[bestpt]<host_strat_vars_weight[j]) {
	    bestpt=j;
    }
  } 
  *clauseind = host_strat_vars_Oneocc[bestpt];       // indice di una clausola (qualsiasi, ORA e' l'ultima trovata) che contenga select lit 
  *sel_var  =  (bestpt < NV)? bestpt : -(bestpt-NV); 
  *minmask =  IMIN(INT_MAX,host_strat_vars_weight[bestpt]);
#ifdef TRACESELECT
	printf("J");fflush(stdout);
#endif
  return(*minmask); //basta return di un >1

} // FINE STRATEGIA JEROWANGONESIDED




case DLCS: { //Dynamic Largest Combined Sum
  *minmask=0;
  i=0;    
  memset(host_strat_vars_weight,((uint)0),NV*sizeof(uint));
  while (i < NC) { //si potrebbe fare con un solo loop su formula[]
    mask = 0;   
    maybe_sat = 0;
    j=clause_pointer[i];
    while (j<clause_pointer[i+1]) {
      if ((formula[j]<0 && host_vars[-formula[j]]==0)||(formula[j]>0 && host_vars[formula[j]]==1)){ //clausola sat
	mask      = 0;
	maybe_sat = 2;
	break; 
      } else if (host_vars[abs(formula[j])] < 0) { // Literal Unknown 
	mask++; 
	t=j;  //memo id del lit in formula[]
	maybe_sat = 1;
      } 
      j++;
    }  // while (lits della clausola)
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1; sel_var=NULL;
	    *clauseind=i;
#ifdef TRACESELECT
	printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (mask>1) {
      for (j=clause_pointer[i]; j<clause_pointer[i+1]; j++) {
	      if (host_vars[abs(formula[j])]<0) {
		      (host_strat_vars_weight[abs(formula[j])])++;
		      host_strat_vars_Oneocc[abs(formula[j])] = i*(formula[j]<0?-1:1);  // ANDY: gestione grezza!! DA MIGLIORARE (memo il segno del lit)
	      }
      } // for (lits della clausola)
    } else if (mask==1) {
	    *minmask=1; *clauseind=i; *sel_var=formula[t]; //t is idx of last Unkn Found;
#ifdef TRACESELECT
	printf("u");fflush(stdout);
#endif
	    return(1);
    }
    i++;
  } // while (i) principale (sulle clausole)

  //cerco atomo unknown con la best weight (riciclo la variabile bestpt)
  for (j=1; j<NV; j++) { //cerco il primo pesante 
    if (host_strat_vars_weight[j]>0) {
	    bestpt=j;
	    break;
    }
  } 
  if (j==NV) {
#ifdef TRACESELECT
	printf("+");fflush(stdout);
#endif
	  return(0); //nessun lit unknown e formula non unsat
  }
  j++;
  for (; j<NV; j++) { // cerco il piu' pesante
    if (host_strat_vars_weight[bestpt]<host_strat_vars_weight[j]) {
	    bestpt=j;
    } 
  } 
  *clauseind = abs(host_strat_vars_Oneocc[bestpt]);       // indice di una clausola (qualsiasi, ORA e' l'ultime trovata) che contenga select lit 
  *sel_var  =  bestpt * (host_strat_vars_Oneocc[bestpt]<0 ?-1:1); // segno della var selected in tale clausola
  *minmask =  IMIN(INT_MAX,host_strat_vars_weight[bestpt]);
#ifdef TRACESELECT
	printf("C");fflush(stdout);
#endif
  return(*minmask); //basta return di un >1

} // FINE STRATEGIA DCIS


case DLIS: { //Dynamic Largest Individual Sum
  *minmask=0;
  i=0;    
  memset(host_strat_vars_weight,((uint)0),(2*NV)*sizeof(uint));
  //memset(host_strat_vars_Oneocc,((int)0),(2*NV)*sizeof(int)); //NONSERVE
  while (i < NC) {
    mask = 0;   
    maybe_sat = 0;
    j=clause_pointer[i];
    while (j<clause_pointer[i+1]) {
      if ((formula[j]<0 && host_vars[-formula[j]]==0)||(formula[j]>0 && host_vars[formula[j]]==1)){ //clausola sat
	mask      = 0;
	maybe_sat = 2;
	break; 
      } else if (host_vars[abs(formula[j])] < 0) { // Literal Unknown 
	mask++; 
	t=j;  //memo id del lit in formula[]
	maybe_sat = 1;
      } 
      j++;
    }  // while (lits della clausola)
    if (j==clause_pointer[i+1] && maybe_sat==0) { // EOC: UNSAT!
	    mask = -1;
	    *minmask = -1; sel_var=NULL;
	    *clauseind=i;
#ifdef TRACESELECT
	printf("-");fflush(stdout);
#endif
	    return(-1);
    }
    if (mask>1) {
      for (j=clause_pointer[i]; j<clause_pointer[i+1]; j++) {
	      if (host_vars[abs(formula[j])]<0) {
		      if (formula[j]<0) {
			      (host_strat_vars_weight[NV-formula[j]])++;
			      host_strat_vars_Oneocc[NV-formula[j]] = i;  // la clausola in cui occorre
		      } else {
			      (host_strat_vars_weight[formula[j]])++;
			      host_strat_vars_Oneocc[formula[j]] = i;  // la clausola in cui occorre
		      }
	      }
      } // for (lits della clausola)
    } else if (mask==1) {
	    *minmask=1; *clauseind=i; *sel_var=formula[t]; //t is idx of last Unkn Found;
#ifdef TRACESELECT
	printf("u");fflush(stdout);
#endif
	    return(1);
    }
    i++;
  } // while (i) principale (sulle clausole)

  //cerco atomo unknown con la best weight (riciclo la variabile bestpt)
  for (j=1; j<(2*NV); j++) { //cerco il primo pesante 
    if (host_strat_vars_weight[j]>0) {
	    bestpt=j; break;
    }
  } 
  if (j==(2*NV)) {
#ifdef TRACESELECT
	printf("+");fflush(stdout);
#endif
	  return(0); //nessun lit unknown e formula non unsat
  }
  j++;
  for (; j<(2*NV); j++) { // cerco il piu' pesante
    if (host_strat_vars_weight[bestpt]<host_strat_vars_weight[j]) {
	    bestpt=j;
    }
  } 
  *clauseind = host_strat_vars_Oneocc[bestpt];       // indice di una clausola (qualsiasi, ORA e' l'ultima trovata) che contenga select lit 
  *sel_var  =  (bestpt < NV)? bestpt : -(bestpt-NV); 
  *minmask =  IMIN(INT_MAX,host_strat_vars_weight[bestpt]);
#ifdef TRACESELECT
	printf("I");fflush(stdout);
#endif
  return(*minmask); //basta return di un >1

} // FINE STRATEGIA DLIS






default: {
	 printf("\nERROR: STRATEGIA NON DEFINITA PER mask_prop.  Exiting...\n");fflush(stdout);
	 exit(-1);
 }
} // switch
} // mask_prop


//FINEandy







// ******************************************************************
// Device version without the array and with careful treatment of vars
// vars[i]<0 means still unlabeled,
// in this case, a=vars[i] is -index of deltavars for variable i

__device__ inline int mask_prop_delta(
         int * formula, int * cp,
         int * block_vars,   short int *delta_vars, 
         int NC, int * lit){
    int i=0,t,a,min=0,mask=0;
    int j;
    int pt, bestpt=0 ;
    bool initial=true;
    int maybe_sat=0; // 2 surely sat, 1 maybe, 0 unsat
    
    while((mask >= 0) && (i <NC)){
      mask = 0;   
      j=cp[i];
      maybe_sat = 0;
      pt = 0;
      while((maybe_sat < 2) && (j<cp[i+1])) {
            t=formula[j];         // Temp variables are used to reduce 
            a=block_vars[abs(t)]; // memory accesses
            if (( (t<0) && ((a==0) || ((a<0) && (delta_vars[-a]==0))))    || 
                ( (t>0) && ((a==1) || ((a<0) && (delta_vars[-a]==1)))) ) {
               mask = 0;
               maybe_sat = 2; 
            }  
            else if ((a<0) && (delta_vars[-a]== -1)){ // Literal Unknown 
               mask++; 
               if (pt == 0 || (pt > 0 && abs(t) < abs(formula[pt]))) 
                    pt=j; // CAMBIATO QUI - 22/5
               j++;
               maybe_sat = 1;
             } 
            else j++;
      }  // while
      if (j==cp[i+1] && maybe_sat==0) mask = -1; // EOC: UNSAT!
      if (initial) {
         if (mask != 0)
             {min=mask; bestpt=pt; initial=false; } 
      }       
      else if (mask!=0){ // not initial
           if  ( mask< min ||
		         (mask==min && abs(formula[pt]) < abs(formula[bestpt])))
               {min=mask; bestpt=pt;}
           }
      i++;     
   } // while principale 
   *lit = bestpt;
   return min;      
} 

//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************


__global__ void parmask_prop( int* mask_data, int* vars,int* dev_mapped_formula,int* dev_mapped_cp, int g_tn){
  int stride = blockDim.x * gridDim.x;
  int g_tid = threadIdx.x + blockIdx.x * blockDim.x;
  int tid = threadIdx.x;    
  __shared__ int s_mask_num[THREADS];
  __shared__ int s_mask_id[THREADS];   
  __shared__ int s_mask_cl[THREADS];   // memorizza la clausola che fa unit prop  

  s_mask_num[tid] = 0;  

  while(g_tid < g_tn){
    int l,v;
    short maybe_sat; // maybe_sat == 2 surely sat, 1 maybe, 0 unsat
    int iter,end;
         
    iter = dev_mapped_cp[g_tid];
    end =  dev_mapped_cp[g_tid + 1];      
	
    maybe_sat = 0;
    int num=0; // portato a registri, cosi' posso tenere s_mask per tutte le clausole
    int id=2147483647; // MAXINT <==========
    while((maybe_sat < 2) && (iter < end)) { // Per ogni lett non sat nella cl
        l = dev_mapped_formula[iter];
        v = vars[abs(l)];      
        iter++;                            // passo al prossimo lett
        if ((( l < 0 ) && ( v == 0 )) ||   // se ha segno concorde
	        (( l > 0 ) && ( v == 1 ))) {   // con la variabile ground
	         num = 0;                      // la clausola è vera
	         maybe_sat = 2;                // sicuramente sat
        }  
        else if (v == -1){ // Literal non-ground
	    // Nota. s_mask_num e s_mask_id sono shared, dunque ad accesso veloce
	         num++;          // +1 al num di lett non-ground in clausola
	         if (abs(l)<abs(id))
	             id = l;      // salvo l'indirizzo del lett 
	         maybe_sat = 1;              // maybe sat
        } 
    }  // while
    if (iter == end && maybe_sat == 0) 
      num = -1; // EndOfClause, clausola falsa.

    // aggiorna analisi clausola nello store shared
    int bestnum=s_mask_num[tid];
    int bestid=s_mask_id[tid];
    int bestcl=s_mask_cl[tid];

    // se nuovo calcolato e' piu' interessante -> memorizza!
    if( num == -1 || bestnum == -1) {
        if (num==-1 && bestnum!=-1){ // la nuova cl e' conflitto
	         s_mask_num[tid] = -1; // copia solo se il precedente non era fallito -> tengo g_tid minimo per cella tid
	         s_mask_cl[tid] = g_tid; // segno la clausola che fa fallire
        }
    }
    else if (bestnum == 0) {
        s_mask_num[tid] = num;
        s_mask_id[tid] = id;
        s_mask_cl[tid] = g_tid;
    }
    else if (num > 0 && 
	     (bestnum > num || 
         (bestnum == num && abs(bestid) > abs(id)) || 
         (bestnum == num && abs(bestid) == abs(id) && bestcl > g_tid)
	      )) {
        s_mask_num[tid] = num;
        s_mask_id[tid] = id;
        s_mask_cl[tid] = g_tid;
    }

    g_tid+=stride;
  } /// while ogni pezzo

  //*************************************************************************************
  __syncthreads();

  // DISTRIBUTED algorithm for implementing first-fail choice (and minimum)      
  short tn = blockDim.x;          
  while(tn > 1){ 
     short half = (tn >> 1); // stesso che tn = tn/2
     short tid_2 = tid + half;
     if(tid < half){
        if( s_mask_num[tid_2] == -1 || s_mask_num[tid] == -1) {
	        if ((s_mask_num[tid_2] == -1 && s_mask_num[tid]>-1 ) || 
                (s_mask_num[tid_2] == -1 && s_mask_cl[tid_2]<s_mask_cl[tid] )){ // copio se il piu' alto e' -1 e scrivo il minimo (a parita' di -1)
	           s_mask_num[tid] = -1;
	           s_mask_cl[tid] = s_mask_cl[tid_2];
	        //printf("riduzione cl %d (%d) num %d (%d)-> da %d a %d\n",s_mask_cl[tid_2],s_mask_cl[tid],s_mask_num[tid_2],s_mask_num[tid],tid_2,tid);
	        }
        }
//*** AGO GIU12: i seguenti due bodies sono uguali. Bug?
    else if (s_mask_num[tid] == 0) {
	    s_mask_num[tid] = s_mask_num[tid_2];
	    s_mask_id[tid] = s_mask_id[tid_2];
	    s_mask_cl[tid] = s_mask_cl[tid_2];
    }
    else if  (s_mask_num[tid_2] > 0 && 
		(s_mask_num[tid] > s_mask_num[tid_2] || 
          (s_mask_num[tid] == s_mask_num[tid_2] && abs(s_mask_id[tid]) > abs(s_mask_id[tid_2]))  || 
          (s_mask_num[tid] == s_mask_num[tid_2] && abs(s_mask_id[tid]) == abs(s_mask_id[tid_2]) && 
           s_mask_cl[tid] > s_mask_cl[tid_2])
		 )) {
	    s_mask_num[tid] = s_mask_num[tid_2];
	    s_mask_id[tid] = s_mask_id[tid_2];
	    s_mask_cl[tid] = s_mask_cl[tid_2];
      }
    }
    
    //*************************************************************************************
    __syncthreads();
    tn = half;
  } // while (tn>1)              

  if(tid == 0){      
    mask_data[3*blockIdx.x] = s_mask_num[0];
    mask_data[3*blockIdx.x+1] = s_mask_id[0];
    mask_data[3*blockIdx.x+2] = s_mask_cl[0];
  }
      
} // parmask_prop


//****************************************************************
//*** FORMULA SIMPLIFICATION (ATL-VEN July 12, 2011)
// simplifies a (still satisfiable) formula by 
// removing true clauses and false literals
//*****************************************************************
//INITandy
int comp(const void * a, const void * b) {
        const uint *pval1 = (uint *)(host_strat_vars_weight+*((uint *)a));
        const uint *pval2 = (uint *)(host_strat_vars_weight+*((uint *)b));
        if (*pval1 == *pval2)
                return(0);
        else
                return(*pval1 < *pval2 ? -1 : 1);
}
int compbis(const void * a, const void * b) {
        const uint *pval1 = (uint *)(host_strat_vars_weight+*((uint *)a));
        const uint *pval2 = (uint *)(host_strat_vars_weight+*((uint *)b));
        const uint *qval1 = (uint *)(host_strat_vars_weight+NV+*((uint *)a));
        const uint *qval2 = (uint *)(host_strat_vars_weight+NV+*((uint *)b));
        if ((*pval1 + *qval1) == (*pval2 + *qval2))
                return(0);
        else
                return((*pval1 + *qval1) < (*pval2 + *qval2) ? -1 : 1);
}
int pmoc(const void * a, const void * b) {
        const uint *pval1 = (uint *)(host_strat_vars_weight+*((uint *)a));
        const uint *pval2 = (uint *)(host_strat_vars_weight+*((uint *)b));
        if (*pval1 == *pval2)
                return(0);
        else
                return(*pval1 > *pval2 ? -1 : 1);
}
//FINEandy




__host__ int filter_formula(int &fv){ 

 int i,lp,cp,flp,fnc,vera;
 
 filtered_vars[0]=-1;

//INITandy
 //maps_vars[0]=0;
 memset(maps_vars,0,NV*sizeof(int));
 for (i=1; i<NV; i++) {permutator_maps_vars[i]=i;}

// se sono qui ho eseguito mask_propagation_cpu, sfrutto l'ordinamento che ha calcolato
if ((strategia_maps_varSort==MAPSVARSORT_YES) && (strategia_varSelHost != NONE )) {
    if ( (strategia_varSelHost == JEROWANGONESIDED) || (strategia_varSelHost == DLIS)) {
	qsort(permutator_maps_vars+1 , NV-1 , sizeof(int), compbis);
    } else if (strategia_varSelHost == MINCLAUSE) {
	qsort(permutator_maps_vars+1 , NV-1 , sizeof(int), pmoc);
    } else {
	qsort(permutator_maps_vars+1 , NV-1 , sizeof(int), comp);
    }
}

 //printf(".");fflush(stdout);

// Here only the unassigned (free) vars are considered. 
// maps_vars stores the correspondence new var / old var
 
 fv = 0;
 for(i=1;i<NV;i++){
    if (host_vars[i] == -1){
      fv++;
      maps_vars[i] = permutator_maps_vars[fv]; // OLD NONGROUND VARS ARE ASSIGNED TO NEW
      filtered_vars[permutator_maps_vars[fv]] = -1;
    }  
    // else maps_vars[i] = i; //(this value would be never used)
 }                         
 
/*
for(int i=1; i < NV; i++) { //if(maps_vars[i] != 0) 
	            { printf("maps_vars[%d]=%d\n",i, maps_vars[i]);   } }

for(int i=1; i < NV; i++) { //if(maps_vars[i] != 0) 
	            { printf("permutator_maps_vars[%d]=%d\n",i, permutator_maps_vars[i]);   } }
*/

// for(i=1;i<NV;i++){
//	 printf("mv[%d]=%d\n",i,maps_vars[i] );
// }
//FINEandy
 
// The formula is simplified and projected
// on the new variables.
     
 lp=0; fnc=0; 
 filtered_clause_pointer[0] = 0;
  
 for (cp=0; cp < NC; cp++){
    vera=1;
    flp = filtered_clause_pointer[fnc];
    while (lp < clause_pointer[cp+1]){
       // FIRST CASE: the literal, hence the clause C_cp is satisfied. Remove C_cp
       if ((formula[lp] > 0) && (host_vars[formula[lp]] == 1) ||
           (formula[lp] < 0) && (host_vars[-formula[lp]] == 0)){
          vera = 1;
          lp = clause_pointer[cp+1]; break; // Exit the loop
       }  // SECOND CASE: the literal is not ground. Copy it, renaming the var
       else if (host_vars[abs(formula[lp])] == -1){
          filtered_formula[flp] =  maps_vars[abs(formula[lp])];  
          if (formula[lp] <= 0)  filtered_formula[flp]=-filtered_formula[flp];
          flp++;
          lp++;
          vera=0;
       }  //THIRD CASE: the literal is false. Just skip it            
       else lp++;
   } // while
   if (!vera) { // NOTA: se e' falsa, la tolgo come se fosse vera
       fnc++;   // Non dovrebbe mai essere chiamata in tal caso.
       filtered_clause_pointer[fnc] = flp;
   }                       
  } // for  
  
  return fnc;
  
} // function filter_formula


//***************************************************************
//***** KERNEL FUNCTION.
//***** DEVICE VERSION OF DPLL
//***** Without recursion
//***** Use block/thread address to guide the search
//***** Use shared block memories
//***************************************************************

// __shared__ short int* block_varsE;

__global__ void CUDADPLL(int* formula, int* cp,  int *vars, int NC, int NV, 
              int CUDATHREADS, int LOG_BLKS, int Delta, int LOG_THRDS ){
    
  //*** If some other thread already found solution, skip the thread

  if (vars[0]<1){
   // *** SHARED ARRAY OF THE BLOCK
   __shared__  int block_vars[BV_SIZE];
   // *** LOCAL ARRAYS OF THE THREAD
   short int delta_vars[DV_SIZE], varstack[DV_SIZE][2]; 
          
   int addr=blockIdx.x,count=0,top=-1,pos,i; 
   int lit;
   bool end=false, updated=false;
   int min_mask=0;

   //****************************************************************
   // The first LOG_BLKS vars are assigned using block coordinates
   // The others are delegated to delta_vars
   //****************************************************************

     
#ifdef SIMPLEBLOCK

// *************************************************************************
// **** SIMPLE CODE. GOOD FOR DATA PARALLELISM. SOME DETERMINISM IS
// **** NOT WELL-EXPLOITED
// *************************************************************************

     if (threadIdx.x==0) {
       block_vars[0]=0;
       for(i=1;i<NV;i++){
         if (count < LOG_BLKS){ 
             block_vars[i] =  addr % 2; 
             addr=addr/2; count++;
          }         
          else { 
             block_vars[i] = top;
             top--;
          }       
       } // for i ...
     } // if ((threadIdx.x==0)...)

#else

// *************************************************************************
// **** ALTERNATIVE CODE WITH FAST UNIT PROPAGATION IN THE FIRST STAGE
// **** DATA PARALLELISM NOT POSSIBLE
// *************************************************************************
   int j,ui=0,uj=0;
   int l,v;

   if (threadIdx.x==0) {
     //**** Reset of block_vars 
     block_vars[0]=0;
     for(i=1;i<NV;i++)
        block_vars[i]=-1;
     //****
     for(i=1;i<NV;i++){
       if (count < LOG_BLKS){
            j=0; // Find the next free variable
            while((i+j < NV) && (block_vars[i+j] >= 0)) 
               j++;
            if(i+j < NV){
               block_vars[i+j] =  addr % 2;
              addr=addr/2; 
              count++;
            }
       // *** FAST UNIT PROPAGATION DIRECTLY HERE
           min_mask=1; 
           while(min_mask==1){   
             ui = 0;                 
             min_mask=0;
             while((ui < NC) && (min_mask != 1)){
                min_mask=0;  
                uj=cp[ui];
                while((min_mask < 2) && (uj < cp[ui+1])) {
                   l=formula[uj];
                   v=block_vars[abs(l)];
                   if ((( l < 0 ) && ( v == 0 )) ||   // se ha segno concorde : block -> mask=0, stop is sat!
                       (( l > 0 ) && ( v == 1 ))){
                       uj=cp[ui+1];
                       min_mask=0;
                   }
                   else if (block_vars[abs(formula[uj])] < 0) {
                      min_mask++; 
                      lit = uj;
                   } 
                   uj++;
                }  // while  min_mask < 2
                ui++;
              } // while  ui < NC  
              if (min_mask==1){
                 block_vars[ abs(formula[lit])] = (formula[lit] > 0);
              }
          }  // while(min_mask==1)
       } // if count < 
       else if(block_vars[i] == -1){   // Assign the remaining variables (if any)                    
           block_vars[i] = top;
           top--;
       }
                         
       }  // for i=1 .. NV             
     } // if ((threadIdx.x==0)...)

#endif

//*************************************************************************************
__syncthreads();


// *** Every thread resets its local part of the var array
       for(i=0;i < Delta;i++)  delta_vars[i] = -1;

// Use the (block,thread) coordinate to guide the successive 
// LOG_THRDS ND choices
// top is now a pointer to the top of the stack 
// lit points to the selected unknown literal
// pos is its sign

   addr=threadIdx.x;
   top=-1; count=0;    
   while(!end){
       if(top>=0) { delta_vars[varstack[top][0]] = varstack[top][1] % 2; }
       
       //*****************************************************
       //*** Partial substitution evaluation here:
       //*****************************************************       
       min_mask = mask_prop_delta(formula, cp, block_vars,delta_vars,NC,&lit);        
        
       if (min_mask == 0) {
          end=true; break;  // logically superfluous "break", but it speeds it up...
       } else if (min_mask > 0) { // min_mask > 0
          pos = (formula[lit] > 0); // sign of the unknown literal
          top++;  
          varstack[top][0]=-block_vars[abs(formula[lit])];
          if (min_mask == 1) // There is ONE non-ground literal: determinism 
               varstack[top][1]=pos;
          else if (count < LOG_THRDS){ // Set the variable using thread coords   
              varstack[top][1] = addr % 2; addr>>=1;
              count++;
              }         
          else // min_mask > 1 && count >= LOG_THRDS
             varstack[top][1]=2+pos; // Assign a backtrackable value
      } // else  min_mask > 0  
      
      else {   // if (min_mask < 0) *** Failure: Backtracking  
    while((top>=0) && (!updated)){
             pos=varstack[top][1]; // pos is used to avoid 2 addressing to varstack[top][1]
             if (pos > 1){   // 3-> try 0, 2-> try 1 
                 varstack[top][1] = 3-pos;   
                 updated=true;  
             } else {
                delta_vars[varstack[top][0]] = -1; // Restore unknown status
                top--;   // 1 -> stop, 0-> stop
             } 
          }
          if (top <0) {end = true; break;} else updated=false;
       } // else
    } // while

//*************************************************************************************
__syncthreads();
    
   if (vars[0]<1 && (min_mask==0)){ // *** A solution: export it on vars
     //printf("yes\n");
     vars[0]=1;
     for(pos=1; pos<NV; pos++) 
         if (block_vars[pos] >= 0)
            vars[pos] = block_vars[pos];
         else 
            vars[pos] = delta_vars[-block_vars[pos]]; 
   } 
 } // if (vars[0]...
} 


__device__ void insert_gpu(int c, int v, int p_W, int zerone, int* V2C, 
			   int* W, int* NEXT, int* C2V){
     int ptemp = V2C[v];
     V2C[v]         = p_W;
     W[p_W]         = c;
     NEXT[p_W]      = ptemp; 
     C2V[2*c+zerone] = v;
}     

__device__  int  unit_propagationw(int* U, int* UNIT, int* V2C, 
					 int* W, int* NEXT, int* C2V,
					 int* top, int* varstack,
					 int* nextselected,
					 int* host_vars,
					 int* clause_pointer,
					 int* formula,
					 int NV,
					 int dbg
					 ){
   
   int n_p,temp,c,v, lit,previous;
  
   // printf("#### ENTERED IN UNIT PROPAGATION\n");

   *nextselected = 0;
   while(*U >= 0){
      // *** v is the current (deterministic) variable

      v = abs(UNIT[*U]);
      if (dbg) printf("unit: v %d, hv %d\n",UNIT[*U],host_vars[v]);
      if (host_vars[v] < 0){
          host_vars[v] = (UNIT[*U] > 0);
          (*top)++;
          varstack[*top*2+0] = UNIT[*U]; //*** signed literal
          varstack[*top*2+1] = 2 ; //*** 2 -> not backtrackable 
	  if (dbg)
	    printf("CASE 4 - UP var[%d]=%d\n",abs(UNIT[*U]),(UNIT[*U]/abs(UNIT[*U])+1)/2);
      }// assumo che se gia' presente non e' in constrasto
      (*U)--; //*** v removed from the unit list.

      n_p = V2C[v];
      if (dbg) printf("v:%d\n",n_p);
      while (n_p > -1){ //*** Visit the v list (starting from V2C[v])       
         c = W[n_p];
         lit = clause_pointer[c];
         //*** It is not sufficient to check only the "v" based  literal!
         //*** Could be sped up with a further (backtrackable) array
         temp=1;
         while(temp && (lit < clause_pointer[c+1]))
             if ( (host_vars[abs(formula[lit])] >= 0) && ((formula[lit] > 0) == host_vars[abs(formula[lit])]))
                 temp=0;
             else lit++;
         //*** check if v satisfies the clause
         if (!temp) {
            // printf(">>> Clause %d satisfied by current assignment\n",c);
            previous = n_p;
            n_p  = NEXT[previous];
        }   
        else { //*** clause NOT satisfied by the v assignment 
               //*** seek for one unassigned literal (restart from beginning of the clause)
             // printf(">>> Clause %d **NOT** satisfied by current assignment\n",c);
             lit = clause_pointer[c];
             while((lit < clause_pointer[c+1]) && (host_vars[abs(formula[lit])] >= 0))  lit++;
             // *** Clause c is false: backtracking needed.
             if (lit == clause_pointer[c+1]){ 
	       // printf(">>> Exit due to failure\n");
                 *U = -1; // Empty the Unit list
                 return 0;
              }      
             //*** We know that formula[lit] is satisfiable. We know abs(formula[lit]) is not v   
             // *** if the first non ground literal found is already watching c, seek another
             if (abs(formula[lit]) == C2V[2*c+0] || abs(formula[lit]) == C2V[2*c+1]){
                 temp=formula[lit];
                 lit++;
                 while((lit < clause_pointer[c+1]) && (host_vars[abs(formula[lit])] >= 0)) lit++;
                // *** if there is not another:  UNIT CASE: add it to unit and update pointers
                if (lit == clause_pointer[c+1]){  
                    if (dbg) printf("Aggiungo %d a UNIT\n", temp);
                    (*U)++;
                    UNIT[*U] = temp;
                    previous = n_p;
                    n_p = NEXT[previous];
                 }                     
              } // if the first literal is watching   
              
              if (lit < clause_pointer[c+1]){ //*** Update in the non unit case
                  // **** SELECTION STRATEGY: the LAST found non ground watched literal different 
                     *nextselected = formula[lit]; // from v in the LAST non satisfied clause visited 
                     //printf("SELECTED literal %d\n",formula[lit]);
                     //*** LISTS REARRANGEMENTS
                    if (C2V[2*c+0]==v) C2V[2*c+0] = abs(formula[lit]);
                       else  C2V[2*c+1] = abs(formula[lit]);
                    temp = V2C[abs(formula[lit])];
                    if (n_p == V2C[v]){  //*** We are at the beginning of the list 
                       V2C[v] = NEXT[n_p];
                       NEXT[n_p] = temp;
                       V2C[abs(formula[lit])] = n_p;
                       n_p = V2C[v];
                    }     
                    else {
                       NEXT[previous] = NEXT[n_p];                             
                       V2C[abs(formula[lit])] = n_p;
                       NEXT[n_p] = temp;
                       n_p = NEXT[previous];
                    }
              } // Update in the non unit casa      
          } // end if clause not satisfied by "v" assignment
      } // visit the v list
    }  // while *U      

    return 1;
}        

__device__ int first_seedw(int* host_vars, int* V2C, int NV){
    int i=1,temp=1;
    
     while(temp)
          if( (i < NV) && (host_vars[i] < 0) && (V2C[i] >= 0) )
                temp = 0;
          else i++;       
     if (i < NV) 
         return i; //*** Found a non ground literals pointing
     else {
         i=1;
         temp=1;    
         while(temp)
              if( (i < NV) && (host_vars[i] < 0) )
                temp = 0;
               else i++;
         if (i < NV)  
               return i; //*** Found a non ground literal not pointing
         else return 0;
    } 
} 


///////// versione con watched literals
__global__ void CUDADPLLW(int* formula, int* clause_pointer,  int *vars, int NC, int NV, 
			  int CUDATHREADS, int LOG_BLKS, int LOG_THRDS,
			  int* host_vars_g,
			  int* V2C_g,
			  int* W_g, int* NEXT_g, int* UNIT_g, int* C2V_g, int* varstack_g){
    
  // ale: nota: qui vars scritto solo alla fine in caso di successo
  // uso host_vars come variabili locali al thread

  //printf("BL %d: TH %d: HI: var0 %d\n",blockIdx.x,threadIdx.x,vars[0]);
  int dbg=0;
  //if (threadIdx.x==0 && blockIdx.x==0) dbg=1;


//cuda-memcheck ./a.out 5 1 3 17 0 0 0 0 INS-1/02_4queens.cnf

  if (vars[0]<1){
      
      int* host_vars=&host_vars_g[DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* V2C=&V2C_g[2*DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* W=&W_g[2*DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* NEXT=&NEXT_g[2*DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* UNIT=&UNIT_g[DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* C2V=&C2V_g[2*DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];
      int* varstack=&varstack_g[2*DV_SIZE_W*(blockIdx.x*blockDim.x+threadIdx.x)];

  int i, U,sat,top, proceed, SEL=0, temp;

    
//*** RESET OF THE WATCHED STRUCTURE:

  for(i=0;i<2*NC;i++){ W[i]   = -1;  NEXT[i] = -1; }
  for(i=0;i<NC;i++){ C2V[2*i+0] = 0;  C2V[2*i+1] = 0; }
  for(i=0;i<NV;i++)  { V2C[i] = -1;  UNIT[i] = 0;  }
  U=-1;  top=-1;  sat=1; SEL=0;
  
//*** INITIALIZATION OF WATCHED MATRIX

  for(i=0;i<NC;i++){
    //if (blockIdx.x==0 && threadIdx.x==0)
      //printf("----%d------\n",i);
    //  for(i=0;i<NC;i++){
      //*** FIRST LITERAL WATCHED
    //printf("%d %d\n",i,clause_pointer[i]);
      int v=abs(formula[clause_pointer[i]]);
      int ptemp = V2C[v];
      //    printf("%d\n",v);
      V2C[v]     = 2*i;
      W[2*i]     = i;
      C2V[2*i+0] = v;
      NEXT[2*i]=ptemp;
      //*** CHECK THAT THERE ARE AT LEAST TWO LITERALS IN THE CLAUSE
      if (clause_pointer[i+1] > clause_pointer[i] + 1){
      //*** SECOND LITERAL WATCHED (IF ANY)
	 int v=abs(formula[clause_pointer[i]+1]);
	 int ptemp = V2C[v];
	 V2C[v]     = 2*i+1;
	 W[2*i+1]   = i;
	 C2V[2*i+1] = v;
	 NEXT[2*i+1]=ptemp;      
      } else { //*** READY TO APPLY UNIT PROPAGATION FOR THIS LITERAL (no repetition control!!! - this might cause error)
         U++;
         UNIT[U] = formula[clause_pointer[i]]; // Positive or negative
      }
      // printf("----%d------\n",i);

   }   
  

  if (1==0 && dbg){
   printf("**** DATA STRUCTURE *****************************************\n");  
   for(i=0;i<NC;i++)
     printf("Watched clausola %d = %d, %d\n", i, C2V[2*i+0], C2V[2*i+1]);
   for(i=0;i<2*NC;i++)
     printf("Linked structure i=%d, (%d,%d) \n", i, W[i], NEXT[i]);
   for(i=1;i<NV;i++)
     printf("Var pointer V2C[%d] = %d\n", i, V2C[i] );
   printf("**********************************************************\n");  
  }


  ///////// assegna var su stack, sia blocco che di thread
  host_vars[0]=0;
  for (int i=1;i<NV;i++)
    host_vars[i]=-1;



  int addr=(blockIdx.x<<LOG_BLKS)+threadIdx.x;
  int fixed=LOG_BLKS+LOG_THRDS;

  if (dbg) printf("add %d, %d %d\n",addr,blockIdx.x,threadIdx.x);
  //if (blockIdx.x==0) 
  //printf("bl %d th %d add %d\n",blockIdx.x,threadIdx.x,addr);




        


      int retv=-1; 

      if (retv<0){ // non trivial result
	// Heuristic first seed, if not set by unit_propagation
      
	if (SEL == 0) SEL = first_seedw(host_vars,V2C,NV); 

	// *** Initialize the stack with a backtrackable var      
	top++;
	varstack[2*top+0]= SEL;   // E.g. v satisfied by 1 -> -1
	varstack[2*top+1]= 1-addr%2; // cosi' e' backtrack, ma solo 1 val
	addr/=2;
	fixed--;
                       
	//****************************************************************************
	//*** MAIN LOOP
	//****************************************************************************
      

	while((top >=0) && (top < NV-1)){
	  proceed = 0;
	  while((top >= 0) && !proceed){  
	    if (varstack[2*top+1] > 1){ // *** no longer  backtrackable
	    if (dbg) printf("CASE 1 - pop of var[%d]=%d\n",abs(varstack[2*top+0]),host_vars[abs(varstack[2*top+0])]);
	    host_vars[abs(varstack[2*top+0])] = -1;
	    varstack[2*top+1] = -1;
	    top--;// Restore unknown status              
	  } else if (varstack[2*top+1] < 0) {// Never tried value. Try and update
	    if (dbg)  printf("CASE 2 - 1st attempt for var[%d]-\n",abs(varstack[2*top+0]));
	    proceed = 1;
	    host_vars[abs(varstack[2*top+0])] = 2+varstack[2*top+1]; 
	    varstack[2*top+1] = host_vars[abs(varstack[2*top+0])];
	  } else if (varstack[2*top+1] < 2) {// Last chance
	    if (dbg)  printf("CASE 3 - 2nd attempt  for var[%d]=%d\n",abs(varstack[2*top+0]),1-varstack[2*top+1]);
	    proceed = 1;
	    host_vars[abs(varstack[2*top+0])] = 1-varstack[2*top+1];
	    varstack[2*top+1] = 2;
	  } 
        }// *** while (! proceed)
if (dbg) printf("top %d\n",top);             
	if (top >= 0){
	  U++;
	  UNIT[U]= varstack[2*top+0];
	  if (dbg) printf("add unit %d %d\n",U,varstack[2*top+0]);
	  //*** CALL TO UNIT PROPAGATION  
	  sat =
	  unit_propagationw(&U,UNIT,V2C,W,NEXT,C2V,&top,varstack,&SEL,host_vars,clause_pointer,formula,NV,dbg);

   if (1==1 && dbg){
    printf("EXITED U.P. with U=%d, SEL=%d, top=%d, sat=%d top=%d NV=%d\n", U, SEL, top, sat,top, NV);
    if (1==1){
      for(i=1; i <NV; i++) 
	printf("v[%d]=%d",i,host_vars[i]);
      printf("\n");
    }
   }

	    if (sat && (top < NV -1) && !SEL){
	      //*** SEL becomes the next entry for U.P.
	      i=1; temp=1;
	      while(temp && (i < NV))
		if( (host_vars[i] < 0) )  
		  temp = 0;
		else i++;
	      SEL=i;
	    } 
              
	    if (sat && SEL)  {
	      top++;
	      varstack[2*top+0]= SEL;   // E.g. v satisfied by 1 -> -1
	      if (fixed>0) // ancora preassegnate
	      {
	      if (dbg) printf("force %d %d\n",SEL,addr%2);
	      varstack[2*top+1]= 1-addr%2; // cosi' e' backtrack, ma
	      addr/=2;
	      //solo 1 val, inverto il valore perche' l'assegnamento
	      // lo rigira!!	
	      
	      fixed--;	      
	      }else{
	       varstack[2*top+1]=(SEL >0) -2;     
	      }
	    }
	  
	} // if (top>=0)      
      } // *** main loop     

      retv=sat;
      }

      /// il thread ha finito
      __syncthreads();
      //if (dbg) printf("retv %d vars %d\n",retv,vars[0]);
      if (retv==1){ // *** A solution: export it on vars (potential conflict in writing solution)
	//printf("yes\n");
	vars[0]=1;
	for(int pos=1; pos<NV; pos++){
            vars[pos] = host_vars[pos];
	    if (dbg) printf("(%d %d) %d=%d\n",blockIdx.x,threadIdx.x,pos,host_vars[pos]);
	}

      }
  }
} 


//**************************************************************************
//*********    CUDA_caller                         *************************
//**************************************************************************

__host__ int CUDA_caller(){

   int unk_clauses;
   
   CUDA_count_lower++;                       
    
   // *** filter_formula call - global variables are used
   int FV;
    unk_clauses= filter_formula(FV);
    
//   DEBUG PRINT
//       printf("Called GPU with %d free vars\n",FV);
//       printf("Before: V=%d, C=%d, L=%d, After: V=%d, C=%d, L=%d\n",
//              NV-1,NC,clause_pointer[NC],FV,unk_clauses,filtered_clause_pointer[unk_clauses]);

/*
    printf("In CUDA_caller: VarsMap:\n");
            // printf("(var[%d] -> %d )\n",maps_vars[0],0);   
    for(int i=1; i < NV; i++) {
	    //if(maps_vars[i] != 0) 
	    {
	    //if(maps_vars[i] < i) 
             //printf("var[%d] -> %d \t((maps_vars[i] < i)==%d)\n",maps_vars[i],i,(maps_vars[i] < i));   
	     printf("maps_vars[%d]->%d\n",i,maps_vars[i]);   
	    }
    }
    printf("\n");fflush(stdout);

    print_instance(filtered_formula,filtered_clause_pointer,unk_clauses);  
    
    for(int i=1; i < NV; i++) 
             printf("host_vars[%d]=%d\n",i,host_vars[i]);   
    printf("\n");
*/

    //printf("unk_clau %d nl %d, fv %d\n",unk_clauses,filtered_clause_pointer[unk_clauses],FV);
    //print_instance(filtered_formula,filtered_clause_pointer,unk_clauses);  
        
    // copy the value of filtered vars, formula, and clause pointers 
    // to device global variables

    HANDLE_ERROR(cudaMemcpy(dev_vars, filtered_vars, (1+FV)*sizeof( int), cudaMemcpyHostToDevice ));
    HANDLE_ERROR(cudaMemcpy(dev_formula, filtered_formula, filtered_clause_pointer[unk_clauses]*sizeof( int), cudaMemcpyHostToDevice ));
    HANDLE_ERROR(cudaMemcpy(dev_cp, filtered_clause_pointer, (unk_clauses+1)*sizeof(int), cudaMemcpyHostToDevice ));
    
      	cudaThreadSynchronize();
		checkCUDAError("allocazione dev_cp execution");
#ifdef MYTRACE
  printf("\nMore memory allocation (in CUDA_caller):\n");
  printf("    (cuda)             dev_vars:\t%d bytes for %d vars\n", (1+FV)* sizeof(int), FV);
  printf("    (cuda)          dev_formula:\t%d bytes for %d vars\n", filtered_clause_pointer[unk_clauses]* sizeof(int), filtered_clause_pointer[unk_clauses]);
  printf("    (cuda)               dev_cp:\t%d bytes for %d clauses\n", (unk_clauses+1) * sizeof(int), (unk_clauses+1));
  printf(" (host)           filtered_vars:\t%d bytes for %d vars\n", (1+FV) * sizeof(int), FV);
  printf(" (host)               maps_vars:\t%d bytes for %d vars\n", NV * sizeof(int), NV);
  printf(" (host)        filtered_formula:\t%d bytes for %d vars\n", clause_pointer[NC] * sizeof(int), clause_pointer[NC]);
  printf(" (host) filtered_clause_pointer:\t%d bytes for %d clauses\n", (NC+1) * sizeof(int), NC);
  printf("CUDA_caller calling CUDADPPL3: blocks = %d  threads per block  = %d\n",CUDABLOCKS,CUDATHREADS);
  printf("          (shared) block_vars:\t%d bytes for %d vars. Total of %d bytes per block\n", MaxV * sizeof(int), MaxV, MaxV * sizeof(int));
  printf("          (thread) delta_vars:\t%d bytes for %d vars. Total of %d bytes per block\n", Delta * sizeof(int), Delta, CUDATHREADS*Delta * sizeof(int));
  printf("            (thread) varstack:\t%d bytes for %d vals. Total of %d bytes per block\n", 2*Delta * sizeof(int), 2*Delta, CUDATHREADS*2*Delta * sizeof(int));
  fflush(stdout);
#endif

    // ***************   DEVICE COMPUTATION: *************************
//    printf("Delta %d (Check if it is less than  %d)\n",Delta,DV_SIZE);
    CUDADPLL<<<CUDABLOCKS,CUDATHREADS>>>(dev_formula,dev_cp,dev_vars,unk_clauses,1+FV,CUDATHREADS,LOG_BLKS,Delta,LOG_THRDS); 

      	cudaThreadSynchronize();
		checkCUDAError("kernel CUDADPLL execution");
    // ***************   Results' analysis   *************************
    // Only the first value (a flag) is copied back
    HANDLE_ERROR(cudaMemcpy(host_vars,dev_vars, sizeof( int), cudaMemcpyDeviceToHost));
    //  If there is no solution, skip, otherwise copy the whole
    //  assignment back from DEVICE to HOST
    //  Then assembly the solution from vars and filtered_vars
    
    if (host_vars[0]==1){ 
      HANDLE_ERROR(cudaMemcpy(filtered_vars,dev_vars, (1+FV)*sizeof( int), cudaMemcpyDeviceToHost));
       for(int i = 1; i < NV; i++)  
          if (host_vars[i] < 0)          
             host_vars[i] = filtered_vars[maps_vars[i]];              
    }  
    
   
   // In realta' le prox quattro potrebbero anche essere globali o statiche,
   // comunque grandi (stanno su host) e si  evita  di riallocare
   
    return (host_vars[0]==1)*2; // 2 if good, 0 if bad
}


__host__ int CUDA_caller_watched(){

   int unk_clauses;
   
   CUDA_count_lower++;                       
    
   // *** filter_formula call - global variables are used
   int FV;
    unk_clauses= filter_formula(FV);
   
    //printf("Called GPU with %d free vars\n",FV);

//   DEBUG PRINT
       if (1==0){

       printf("Before: V=%d, C=%d, L=%d, After: V=%d, C=%d, L=%d\n",
              NV-1,NC,clause_pointer[NC],FV,unk_clauses,filtered_clause_pointer[unk_clauses]);

	   printf("Vars Mapping:\n");
	   for(int i=1; i < NV; i++) if(maps_vars[i] < i)
	   printf("var[%d] -> %d \t",maps_vars[i],i);   
	   printf("\n");

	 print_instance(filtered_formula,filtered_clause_pointer,unk_clauses);  
    
	 for(int i=1; i < NV; i++) 
	   printf("var[%d]=%d, ",i,host_vars[i]);   
	 printf("\n");

	 /*	 for(int i=1; i < FV+1; i++) 
		 printf("var[%d]=%d, ",i,filtered_vars[i]);   
		 printf("\n");
	 */
       }

       //printf("unk_clau %d nl %d, fv %d\n",unk_clauses,filtered_clause_pointer[unk_clauses],FV);
    //print_instance(filtered_formula,filtered_clause_pointer,unk_clauses);  
        
    // copy the value of filtered vars, formula, and clause pointers 
    // to device global variables

    HANDLE_ERROR(cudaMemcpy(dev_vars, filtered_vars, (1+FV)*sizeof( int), cudaMemcpyHostToDevice ));
    HANDLE_ERROR(cudaMemcpy(dev_formula, filtered_formula, filtered_clause_pointer[unk_clauses]*sizeof( int), cudaMemcpyHostToDevice ));
    HANDLE_ERROR(cudaMemcpy(dev_cp, filtered_clause_pointer, (unk_clauses+1)*sizeof(int), cudaMemcpyHostToDevice ));
    
#ifdef MYTRACE
  printf("\nMore memory allocation (in CUDA_caller):\n");
  printf("    (cuda)             dev_vars:\t%d bytes for %d vars\n", (1+FV)* sizeof(int), FV);
  printf("    (cuda)          dev_formula:\t%d bytes for %d vars\n", filtered_clause_pointer[unk_clauses]* sizeof(int), filtered_clause_pointer[unk_clauses]);
  printf("    (cuda)               dev_cp:\t%d bytes for %d clauses\n", (unk_clauses+1) * sizeof(int), (unk_clauses+1));
  printf(" (host)           filtered_vars:\t%d bytes for %d vars\n", (1+FV) * sizeof(int), FV);
  printf(" (host)               maps_vars:\t%d bytes for %d vars\n", NV * sizeof(int), NV);
  printf(" (host)        filtered_formula:\t%d bytes for %d vars\n", clause_pointer[NC] * sizeof(int), clause_pointer[NC]);
  printf(" (host) filtered_clause_pointer:\t%d bytes for %d clauses\n", (NC+1) * sizeof(int), NC);
  printf("CUDA_caller calling CUDADPPL3: blocks = %d  threads per block  = %d\n",CUDABLOCKS,CUDATHREADS);
  printf("          (shared) block_vars:\t%d bytes for %d vars. Total of %d bytes per block\n", MaxV * sizeof(int), MaxV, MaxV * sizeof(int));
  printf("          (thread) delta_vars:\t%d bytes for %d vars. Total of %d bytes per block\n", Delta * sizeof(int), Delta, CUDATHREADS*Delta * sizeof(int));
  printf("            (thread) varstack:\t%d bytes for %d vals. Total of %d bytes per block\n", 2*Delta * sizeof(int), 2*Delta, CUDATHREADS*2*Delta * sizeof(int));
  fflush(stdout);
#endif

    // ***************   DEVICE COMPUTATION: *************************
  if (unk_clauses>DV_SIZE_W ||FV>DV_SIZE_W){
  printf("CHECK: have %d clauses, < %d\n",unk_clauses,DV_SIZE_W);
  printf("CHECK: have %d vars, < %d\n",FV,DV_SIZE_W);
  exit(-1);
  }

  if (unk_clauses==0) return 2; //ALE: aggiunto per caso in cui filter
     		      	     	//formula toglie tutto

  int myCUDABLOCKS=CUDABLOCKS;
  int myCUDATHREADS=CUDATHREADS;
  if (FV<LOG_THRDS){
    myCUDABLOCKS=1;      
    myCUDATHREADS=(1<<FV); 
  }
  else if (FV<LOG_THRDS+LOG_BLKS){
    myCUDABLOCKS=(1<<FV-LOG_THRDS);
  }
  //printf("FV %d bl %d th %d\n",FV,myCUDABLOCKS,myCUDATHREADS);
  CUDADPLLW<<<myCUDABLOCKS,myCUDATHREADS>>>(dev_formula,dev_cp,dev_vars,unk_clauses,1+FV,myCUDATHREADS,LOG_BLKS,LOG_THRDS,
					    dev_vars_w,dev_v2c_w,dev_w_w,dev_next_w,dev_unit_w,dev_c2v_w,dev_varstack_w);
 
  HANDLE_ERROR(cudaThreadSynchronize());

  // ***************   Results' analysis   *************************
  // Only the first value (a flag) is copied back
  HANDLE_ERROR(cudaMemcpy(host_vars,dev_vars, sizeof( int), cudaMemcpyDeviceToHost));
  //  If there is no solution, skip, otherwise copy the whole
  //  assignment back from DEVICE to HOST
  //  Then assembly the solution from vars and filtered_vars
    
  if (host_vars[0]==1){ 
    HANDLE_ERROR(cudaMemcpy(filtered_vars,dev_vars, (1+FV)*sizeof( int), cudaMemcpyDeviceToHost));
    for(int i = 1; i < NV; i++)  
      if (host_vars[i] < 0)          
	host_vars[i] = filtered_vars[maps_vars[i]];              
  }  
   
  // In realta' le prox quattro potrebbero anche essere globali o statiche,
   // comunque grandi (stanno su host) e si  evita  di riallocare
  //printf("hv %d\n",host_vars[0]);
    return (host_vars[0]==1)*2; // 2 if good, 0 if bad
}


// **************************************************************************************************
// **************************************************************************************************
// *******************     clause learning **********************************************************
// **************************************************************************************************
// **************************************************************************************************

__host__ void clause_learning() {
  // sets jump and level_to_jump

	// inizializzazioni
	int i, c=0, p=0, q, var_p=0, var_q, clause, lp=0, btlevel=0, tptr=trail[0],
		verbose=0;

	if (verbose>1) {
		printf("Current level:\t\t%d\n",level[0]);
		printf("Conflict clause:\t%d\n",refs[0]);
		int i;
		printf("trail[%d] = [",trail[0]);
		for(i=1;i<trail[0]-1;i++)
			printf("%d@%d,",trail[i],level[abs(trail[i])]);
		printf("%d@%d]\n",trail[i],level[abs(trail[i])]);
	}
	//for(int u=0;u<NV;u++)
	//	if(host_vars[u]!=-1)
	//		printf("refs[%d] = %d\n",u,refs[u]);

	// *** Trovo first-UIP e creo la clausola

	//printf("\ntrail:\n");
	//for(int b=1;b<trail[0];b++)
	//	printf("%-5d @ %-5d r %-5d\n",trail[b],level[abs(trail[b])],refs[abs(trail[b])]);
	//printf("\n\n");

	memset(seen,0,NV*sizeof(int));
	do {
	  if (p==0) {				// inizio con la conflict clause 
	    clause = refs[0];	// conflict clause
	  } else
	    clause = refs[var_p];	// reason clause
	  if (verbose>1) {
	    printf("p:\t%d\t",p);
	    printf("var_p:\t%d\t",var_p);
	    printf("clause:\t%d\t",clause);
	    printf("c_start:\t%d\t",clause_pointer[clause]);
	    printf("end: %d\n",clause_pointer[clause+1]);
	    printf("reason (clause %d) is:\t",clause);
	    for(i=clause_pointer[clause];i<clause_pointer[clause+1];i++) {
	      printf("%d\t",formula[i]);
	    }
	    printf("\n");
	  }
	  //printf("clause %-6d cp[clause] %-10 clause_pointer[clause+1] %-10d var_p %-6d refs[var_p] %-10d\n",clause,clause_pointer[clause],clause_pointer[clause+1],var_p,refs[var_p]);
	  for(i=clause_pointer[clause];i<clause_pointer[clause+1];i++) {
	    var_q = abs(formula[i]);
	    q = -formula[i];
	    if ((p==0) || (var_q!=var_p)) { // se sono al primo giro (p==0) devo processare tutta la clausola, altrimenti skippo la unit propagation
	      if (verbose>1) {
		printf("q:%d\n",q);
		printf("lp at %d, q is %d at level %d, ref %d\n",lp,q,level[var_q],refs[var_q]);
	      }
	      if (!seen[var_q]) {
		if (verbose>1) printf("q never seen! ");
		seen[var_q] = 1;
		if (level[var_q] == level[0]) { // (q == decision level corrente)
		  c++;
		  if (verbose>1) printf("and of same current level (%d==%d) so c++.\n",level[var_q],level[0]);
		}
		else if (level[var_q] > 0) { // se q è di un decision level inferiore, ma non il root level (al momento il root level è inutilizzato)

		  learnt_clause[lp++] = -q;		    

		  // calcolo level to backtrack, il massimo della learnt_clause, escluso il first-uip che verrà aggiunto alla fine
		  btlevel = max(btlevel,level[var_q]);
		  if (verbose>1) {
		    printf("and of minor level (%d<%d) so add %d.\n",level[var_q],level[0],-trail[i]);
		    if (lp>0) {
		      printf("current learnt clause is: ");
		      int j;
		      for(j=0;j<lp-1;j++)
			printf("%d,",learnt_clause[j]);
		      printf("%d.\n",learnt_clause[j]);
		    }
		  }
		}
	      } else if (verbose>1) printf("q is seen\n");
	    }
	  }
	  do {
	    p = trail[--tptr];
	    var_p = abs(p);
	    if (verbose>1) {
	      printf("c:%d p%d\n",c,p);
	    }
	    if (verbose>1) {
	      printf("scan p=%d\ttptr=%d\tseen[var_p]=%d\n",p,tptr,seen[var_p]);
	    }	
	  }
	  while (!seen[var_p]);
	  c--;
	  //seen[var_p] = 0;
	  if (verbose>1) printf("c--, process var %d, c:%d, ref %d val %d\n",var_p,c,refs[var_p],host_vars[var_p]);	
	}
	while (c>0);

	//ALE: 8 giu commentata per avere backjumping
	//btlevel = max(btlevel,level[abs(p)]-1); // in caso di solo UIP senza reasons, comunque devo annullare la decisione del livello corrente (dato che poi sommo 1, qui lo sottraggo)

	// in p ora ho il first-UIP, ci va il suo negato nella learnt clause
	learnt_clause[lp++] = -p;

	// controllo inserimento clausole doppie: todo

	// debug
	if (verbose>0) {
	  printf("Add First-UIP: %d lev %d\nLearn clause %d: ",-p,level[abs(p)],NC);
	  //		printf("* LEARNT CLAUSE: ");
		for(i=0;i<lp-1;i++) {
			//printf("%d ",learnt_clause[i]); // stampa per .cnf
		  //			printf("%d@%d, ",learnt_clause[i],level[abs(learnt_clause[i])]);
		  printf("%d ",learnt_clause[i]);
		}
		//printf("%d 0\n",learnt_clause[i]);// stampa per .cnf
		//		printf("%d@%d.\n",learnt_clause[i],level[abs(learnt_clause[i])]);
		printf("%d 0\n",learnt_clause[i]);
	}
	
	// *** aggiungo la clausola imparata
	  if (//lp <10 &&
	    ((NL+lp)<=MAX_NL) && (NC+1<=MAX_NC)) { // se ho abbastanza memoria... (non è detto che debba fare forgetting!)
	  // mi arriva:	learnt_clause[] di interi con i numeri delle host_vars della clausola
	  //	 lp = intero con la lunghezza della clausola
	  // ..e la aggiunge alla formula
	  // - aggiorno il clause pointer finale	  
	  nclausel++;

	  clause_pointer[NC+1] = clause_pointer[NC]+lp;
	  // - aggiungo la clausola alla formula
	  for(i=0;i<lp;i++)
	    formula[clause_pointer[NC]+i] = learnt_clause[i];
	  // - aggiorno il numero dei letterali e il numero di clausole

	  if (USEGPU%2==1){ // solo se uso gpu
	    HANDLE_ERROR( cudaMemcpy( dev_mapped_formula+NL, formula+NL, lp * sizeof(int), cudaMemcpyHostToDevice ) );
	    HANDLE_ERROR( cudaMemcpy( dev_mapped_cp+NC+1, clause_pointer+NC+1, 1 * sizeof(int), cudaMemcpyHostToDevice ) );
	  }

	  NL += lp;
	  NC += 1;

	  if (verbose>1) printf("backjump from level	%d to level %d\n\n\n",level[0],btlevel);
	  level_to_jump=btlevel;	// non chronological backtracking
	  }
	  else {
	    if (verbose>1) printf("backtrack to level %d\n",level[0]);
	    level_to_jump=level[0];	// chronological backtracking
	  }
}


//**************************************************************************
//**************************************************************************
//****** AUXILIARY
//**************************************************************************
//**************************************************************************

__host__ inline void backtrackingf(int level_to_jump){

  // annulla trail stack e riferimenti a variabili
  //  printf("backtrack to %d\n",level_to_jump);

  backtracking++;
  int loop=trail[0]>0;
  while (loop){ // questo mette a posto trail e variabili. level sistemato sulla ricorsione
    int VARP=abs(trail[trail[0]-1]);
    loop=trail[0]>0 &&       
      ((level[VARP]>level_to_jump)   ||  (refs[VARP]>=0)  ||
       (level[VARP]<=level_to_jump && refs[VARP]==-2));
     
    if (level[VARP]>level_to_jump || refs[VARP]>=0 || refs[VARP]==-2){
      //printf("back var %d lev %d\n",VARP,level[VARP]);
      host_vars[VARP] = -1;		// restore "unknown" status
      level[VARP] = -1;
      refs[VARP] = -1;
      trail[0]--;
    }
    else{
      if (level[VARP]<=level_to_jump && refs[VARP]==-1){ // variabile gia' testata -> metto caso opposto
	//printf("switch var %d at lev %d -> %d\n",VARP,level[VARP],-trail[trail[0]-1]);
	refs[VARP] = -2;
	host_vars[VARP] = 1-host_vars[VARP]; // inverse status      
	trail[trail[0]-1]=-trail[trail[0]-1]; // inverto valore su trail
	level[0]=level[VARP];
      }    
    }
  }
} 

__host__ inline void backjumpingf(int level_to_jump){

  // riparte con ultima scelta fatta a level_to_jump
  // annulla trail stack e riferimenti a variabili
  //  printf("backtrack to %d\n",level_to_jump);

  backjump++;
  int loop=trail[0]>0;
  while (loop){ // questo mette a posto trail e variabili. level sistemato sulla ricorsione
    int VARP=abs(trail[trail[0]-1]);
    loop=trail[0]>0 &&       
      ((level[VARP]>level_to_jump)   ||  (refs[VARP]>=0)
       //|| (level[VARP]<=level_to_jump && refs[VARP]==-2)
       );
     
    if (level[VARP]>level_to_jump || refs[VARP]>=0){
      //printf("back var %d lev %d\n",VARP,level[VARP]);
      host_vars[VARP] = -1;		// restore "unknown" status
      level[VARP] = -1;
      refs[VARP] = -1;
      trail[0]--;
    }
    else{ // refs<0 && level=leveltojump
      if (level[VARP]<=level_to_jump && refs[VARP]==-1){ // variabile gia' testata -> metto caso opposto
	//printf("switch var %d at lev %d -> %d\n",VARP,level[VARP],-trail[trail[0]-1]);
	//refs[VARP] = -2;
	//host_vars[VARP] = 1-host_vars[VARP]; // inverse status      
	//trail[trail[0]-1]=-trail[trail[0]-1]; // inverto valore su trail
	level[0]=level[VARP];
      }    
    }
  }
  if (level_to_jump==0)// caso speciale se backjump annulla anche primo livello
    level[0]=0;
} 


//**************************************************************************
//**************************************************************************
//********** MAIN PROCEDURE: twolevel_DPLL *********************************
//********** a part of DPLL (recursive) is handled by the host.  ***********
//********** The final part is made in the device(s) ***********************
//**************************************************************************
//**************************************************************************
//**************************************************************************

__host__  void mask_propagation_cpu(){
  int dbg=0;
    do{ // CPU mask_prop
//INITandy
      mask_prop(&selected_var,&sat_val, &clauind);
//FINEandy
      if (dbg)
       printf("\nDEBUG mask_propagation_cpu:\t mask_prop  sat_val=%d selected_var=%d clauind=%d: ",sat_val,selected_var,clauind);fflush(stdout);
      if (dbg) {
	  printf("\nDEBUG mask_propagation_cpu:\t ");
        for (int i=clause_pointer[clauind];i<clause_pointer[clauind+1];i++)
	  printf(" %d ",formula[i]);
      }
      if (dbg) printf("\n");fflush(stdout);

      if (sat_val==1){
	//	unit++;
	int VAR = abs(selected_var);		// Look for its variable and sign
	if (dbg) printf("\nDEBUG mask_propagation_cpu:\t var prop %d, lev %d\n",selected_var,level[0]); fflush(stdout);
	host_vars[VAR] = selected_var > 0;
	level[VAR] = level[0];
	refs[VAR] = clauind; // devo sapere la clausola
	trail[trail[0]] = selected_var;
	trail[0]++;
      }    
    }
    while (sat_val==1);
      if (dbg) printf("--\n");fflush(stdout);
}

__host__ void mask_propagation_gpu(){
  int dbg=0;
    if (1==0 && dbg){
      printf("at lev %d\n",level[0]);
      for(int i=1;i<trail[0];i++)
	printf("%d@%dr%d,",trail[i],level[abs(trail[i])],refs[abs(trail[i])]);
      printf("\n");
    }
    if (dbg) printf("bl %d, ncl %d NC %d NL %d\n",PARblocks,nclausel,NC,NL);
    do{ // GPU mask_prop
      if (dbg) printf(".\n");
      HANDLE_ERROR( cudaMemcpy( dev_parma_vars, host_vars, NV * sizeof(int), cudaMemcpyHostToDevice ) );
      parmask_prop<<<PARblocks, THREADS>>>( mask_data, dev_parma_vars, dev_mapped_formula, dev_mapped_cp,NC);
      HANDLE_ERROR( cudaMemcpy( h_mask_data, mask_data, (3*PARblocks)*sizeof(int), cudaMemcpyDeviceToHost)); // puo' copiare anche 3*blocks+NV per risparmiare chiamate (se servono dati su vars)
      if (dbg) printf("..\n");
      CUDA_count++;
      // colleziona dati da blocchi
      int bestnum=0;
      int bestid=0;                         
      int bestcl=0;                         
      for (int i=0;i<PARblocks;i++){                                
	if (dbg) printf("%d: %d,%d,%d (%d)",i,h_mask_data[3*i],h_mask_data[3*i+1],h_mask_data[3*i+2],bestid);

	if ((h_mask_data[3*i]==-1 && bestnum!=-1) ||
	    (h_mask_data[3*i]==-1 && bestnum==-1 && bestcl>h_mask_data[3*i+2]) ||
	    bestnum==0 ||
	    (bestnum>0 && h_mask_data[3*i]>0 && 
	     (h_mask_data[3*i]<bestnum ||
	      (h_mask_data[3*i]==bestnum && abs(h_mask_data[3*i+1])<abs(bestid)) ||
	      (h_mask_data[3*i]==bestnum && abs(h_mask_data[3*i+1])==abs(bestid) && h_mask_data[3*i+2]<bestcl)
	      ))){
	  bestnum=h_mask_data[3*i];
	  bestid=h_mask_data[3*i+1];
	  bestcl=h_mask_data[3*i+2];
	}    

	/*
	 // TODO: aumenta propagazione in parallelo (una per ogni blocco possibile)
	if (h_mask_data[3*i]==1){
	  int VAR = abs(h_mask_data[3*i+1]);		// Look for its variable and sign
	  if (level[VAR]==-1){ // se non e' gia'stata dedotta da un blocco parallelo (e' sempre -1 grazie a backtracking)
	    if (dbg) printf("var prop %d, lev %d\n",h_mask_data[3*i+1],level[0]);
	    host_vars[VAR] = h_mask_data[3*i+1] > 0;
	    level[VAR] = level[0];
	    refs[VAR] = h_mask_data[3*i+2]; // devo sapere la clausola
	    trail[trail[0]] = h_mask_data[3*i+1];
	    trail[0]++;	
	  }	  
	}
	*/

      }
	if (dbg) printf("\nbest %d %d %d\n",bestnum,bestid,bestcl);
      //printf("\n");
      sat_val  =bestnum;
      selected_var=bestid;
      clauind=bestcl;
      if (dbg) printf("prop %d %d %d: ",sat_val,selected_var,clauind);
      if (dbg)
        for (int i=clause_pointer[clauind];i<clause_pointer[clauind+1];i++)
	  printf("%d ",formula[i]);
      if (dbg) printf("\n");

      if (sat_val==1){
	int VAR = abs(selected_var);		// Look for its variable and sign
	if (dbg) printf("var prop %d, lev %d\n",selected_var,level[0]);
	host_vars[VAR] = selected_var > 0;
	level[VAR] = level[0];
	refs[VAR] = clauind; // devo sapere la clausola
	trail[trail[0]] = selected_var;
	trail[0]++;		  
      }	
    }
    while (sat_val==1);
}


//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//******************   TWO LEVEL DPLL (no recursive)   *****************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************


__host__ int twolevel_DPLL(){
  int selected;
  int pos;
  int FV;
  short good=0;
  int dbg=0;  
  int isbackjumping=0; // a 1 se fatto backj e quindi devo continuare (anche se il bj ha portato il livello a 0 = ho imparato clausola unitaria, e con una UP posso ripartire da livello 1 con l'altro valore)
  do {    

    isbackjumping=0;

    if (dbg) printf("twolevel_DPLL lev %d\n ",level[0]);
    
    // mask_propagation
    if (USEGPU %2 == 0) {
      mask_propagation_cpu();
//printf("TEST 101000\n");
    }
    else 
      mask_propagation_gpu();

  if (dbg){ 
    printf("dopo maskprop lev %d host_vars: ",level[0]);
      for(int i=1;i<trail[0];i++)
	printf("%d@%dr%d,",trail[i],level[abs(trail[i])],refs[abs(trail[i])]);
      printf("\n");
      /*    for (int i=0;i<NV;i++)
      printf("%d>%d(%d,%d) ",i,host_vars[i],level[i],refs[i]);
    printf("\n");
      */
    printf("sat_val %d, selvar %d\n",sat_val,selected_var);     
  }   

//printf("TEST 102000\n");
  // new case
  if (level[0]<=0 && sat_val<0){ // failed, UP was enough, no need to learn, return fail!
    return 0;
  }
  
  //************* UNSATISFIABLE ASSIGNMENT
  if (sat_val < 0){ // At least one clause is false
//printf("TEST 103000\n");
    good = 0;
    refs[0] = clauind;	// conflict clause
    if (learning) {// CLAUSE LEARNING
      clause_learning(); 
      if (NC%1000 ==0) printf("NC: %d\n",NC);
      if (dbg) printf("cl: level to backjump to %d\n",level_to_jump);      
      if (dbg) printf("backjump %d\n",level_to_jump);
      backjumpingf(level_to_jump);
      isbackjumping=1;
//printf("TEST 104000\n");
    }
    else{
      if (dbg) printf("backtrack %d\n",level[0]);
      backtrackingf(level[0]); // se non imparo, apro fratello (tengo livello corrente)
    }

    // END CLAUSE LEARNING
  }  
  else if (sat_val == 0){  //************* FOUND A SOLUTION
      good = 1;  
  }      
  else    
  { // There is a non-ground literal - sat_val > 0
//printf("TEST 105000\n");
    pos = (selected_var > 0);  
    selected = abs(selected_var);  // Look for its variable and sign 
    FV = NV-trail[0]; // free vars
    if (USEGPU >= 2 && FV <= MaxV)   { //*** GPU CALL: 
//      printf("CUDA caller with %d learnt clauses\n",nclausel);
      good=CUDA_caller();
      if (!good)
	backtrackingf(level[0]); //backtrack
//printf("TEST 106000\n");
    }   
    else { ///////// vado con CPU
//printf("TEST 107020\n");
      level[0]++; // apro nuovo ramo
      if (dbg)
	printf("lev %d, 2 scelte: var %d val %d\n",level[0],selected,pos);
//printf("TEST 107030\n");
      level[selected] = level[0];
//printf("TEST 107040\n");
      host_vars[selected] = pos;
//printf("TEST 107050 trail[0]=%d  selected_var=%d\n",trail[0],selected_var);
      trail[trail[0]++] = selected_var;
//printf("TEST 108000\n");
    } // CPU
  } //sat_val >2


  }
  while(isbackjumping || (level[0]>0 && !good));
  return good;
}



//************************************************************************
//************************************************************************
//************************************************************************
// NEW: Version with watched literals (option 4)
//************************************************************************
//************************************************************************
//************************************************************************

int  unit_propagation(
   int* U, int* UNIT, int* V2C, 
   int* W, int* NEXT, int** C2V,
   int* top, int** varstack,
   int* nextselected){
   
   int n_p,temp,c,v, lit,previous;
  
   // printf("#### ENTERED IN UNIT PROPAGATION\n");

   *nextselected = 0;
   while(*U >= 0){
      // *** v is the current (deterministic) variable
      v = abs(UNIT[*U]);
      if (host_vars[v] < 0){
          host_vars[v] = (UNIT[*U] > 0);
          (*top)++;
          varstack[*top][0] = UNIT[*U]; //*** signed literal
          varstack[*top][1] = 2 ; //*** 2 -> not backtrackable 
	  //printf("CASE 4 - UP var[%d]\n",abs(UNIT[*U]));
      }    
      (*U)--; //*** v removed from the unit list.

      n_p = V2C[v];     
      while (n_p > -1){ //*** Visit the v list (starting from V2C[v])       
         c = W[n_p];
         lit = clause_pointer[c];
         //*** It is not sufficient to check only the "v" based  literal!
         //*** Could be sped up with a further (backtrackable) array
         temp=1;
         while(temp && (lit < clause_pointer[c+1]))
             if ( (host_vars[abs(formula[lit])] >= 0) && ((formula[lit] > 0) == host_vars[abs(formula[lit])]))
                 temp=0;
             else lit++;
         //*** check if v satisfies the clause
         if (!temp) {
            // printf(">>> Clause %d satisfied by current assignment\n",c);
            previous = n_p;
            n_p  = NEXT[previous];
        }   
        else { //*** clause NOT satisfied by the v assignment 
               //*** seek for one unassigned literal (restart from beginning of the clause)
             // printf(">>> Clause %d **NOT** satisfied by current assignment\n",c);
             lit = clause_pointer[c];
             while((lit < clause_pointer[c+1]) && (host_vars[abs(formula[lit])] >= 0))  lit++;
             // *** Clause c is false: backtracking needed.
             if (lit == clause_pointer[c+1]){ 
                 // printf(">>> Exit due to failure\n");
                 *U = -1; // Empty the Unit list
                 return 0;
              }      
             //*** We know that formula[lit] is satisfiable. We know abs(formula[lit]) is not v   
             // *** if the first non ground literal found is already watching c, seek another
             if (abs(formula[lit]) == C2V[c][0] || abs(formula[lit]) == C2V[c][1]){
                 temp=formula[lit];
                 lit++;
                 while((lit < clause_pointer[c+1]) && (host_vars[abs(formula[lit])] >= 0)) lit++;
                // *** if there is not another:  UNIT CASE: add it to unit and update pointers
                if (lit == clause_pointer[c+1]){  
                    // printf("Aggiungo %d a UNIT\n", temp);
                    (*U)++;
                    UNIT[*U] = temp;
                    previous = n_p;
                    n_p = NEXT[previous];
                 }                     
              } // if the first literal is watching   
              
              if (lit < clause_pointer[c+1]){ //*** Update in the non unit case
                  // **** SELECTION STRATEGY: the LAST found non ground watched literal different 
                     *nextselected = formula[lit]; // from v in the LAST non satisfied clause visited 
                     //printf("SELECTED literal %d\n",formula[lit]);
                     //*** LISTS REARRANGEMENTS
                    if (C2V[c][0]==v) C2V[c][0] = abs(formula[lit]);
                       else  C2V[c][1] = abs(formula[lit]);
                    temp = V2C[abs(formula[lit])];
                    if (n_p == V2C[v]){  //*** We are at the beginning of the list 
                       V2C[v] = NEXT[n_p];
                       NEXT[n_p] = temp;
                       V2C[abs(formula[lit])] = n_p;
                       n_p = V2C[v];
                    }     
                    else {
                       NEXT[previous] = NEXT[n_p];                             
                       V2C[abs(formula[lit])] = n_p;
                       NEXT[n_p] = temp;
                       n_p = NEXT[previous];
                    }
              } // Update in the non unit casa      
          } // end if clause not satisfied by "v" assignment
      } // visit the v list
    }  // while *U      

    return 1;
}        
        
//*** AUX. Data Structure Initialization

void insert(int c, int v, int p_W, int zerone, int* V2C, 
            int* W, int* NEXT, int** C2V){
     int ptemp = V2C[v];
     V2C[v]         = p_W;     W[p_W]         = c;
     NEXT[p_W]      = ptemp;   C2V[c][zerone] = v;
}     

//*** AUX. Choice of a variable
//*** unit_propagation in some cases do not select properly a literal
//*** (e.g. in the case of different connected components)

int first_seed(int* host_vars, int* V2C){
    int i=1,temp=1;
    
     while(temp)
          if( (i < NV) && (host_vars[i] < 0) && (V2C[i] >= 0) )
                temp = 0;
          else i++;       
     if (i < NV) 
         return i; //*** Found a non ground literals pointing
     else {
         i=1;
         temp=1;    
         while(temp)
              if( (i < NV) && (host_vars[i] < 0) )
                temp = 0;
               else i++;
         if (i < NV)  
               return i; //*** Found a non ground literal not pointing
         else return 0;
    } 
} 


//************************************************************************
// DPLL with watched literals
//************************************************************************

__host__ int watchedDPLL(){
  int i, U,sat,top, proceed, SEL=0, temp;
  int* V2C, *W, *NEXT, *UNIT, ** C2V, **varstack;
  
//*** DYNAMIC DATA STRUCTURES BORING DEFINITION
//*** From each variable V to a list of clauses guarded by V
  V2C  =  (int*) malloc(2 * NV * sizeof(int));
//*** Lists are stored here:  
  W    =  (int*) malloc(2 * NC * sizeof(int));
  NEXT =  (int*) malloc(2 * NC * sizeof(int));
//*** Temporary list of literals ready for unit-propagation:  
  UNIT = (int*) malloc(NV * sizeof(int));   
//*** Backward pointers: from a clause to the pair of current watched literals (matrix NC x 2)  
  C2V =  (int**) malloc(NC * sizeof(int*)); 
//*** The data structure for recursion  NC x 2
  varstack = (int**) malloc(NC * sizeof(int*));
//*** Rows allocation.
  for(i=0;i<NC;i++){
     varstack[i] =  (int*) malloc(2 * sizeof(int));
     C2V[i]  =  (int*) malloc(2 * sizeof(int));
  }
  
//*** RESET OF THE WATCHED STRUCTURE:

  for(i=0;i<2*NC;i++){ W[i]   = -1;  NEXT[i] = -1; }
  for(i=0;i<NC;i++){ C2V[i][0] = 0;  C2V[i][1] = 0; }
  for(i=0;i<NV;i++)  { V2C[i] = -1;  UNIT[i] = 0;  }
  U=-1;  top=-1;  sat=1; SEL=0;
  
//*** INITIALIZATION OF WATCHED MATRIX

  for(i=0;i<NC;i++){
      //*** FIRST LITERAL WATCHED
      insert(i,abs(formula[clause_pointer[i]]),2*i,0,V2C,W,NEXT,C2V);
      //*** CHECK THAT THERE ARE AT LEAST TWO LITERALS IN THE CLAUSE
      if (clause_pointer[i+1] > clause_pointer[i] + 1){
      //*** SECOND LITERAL WATCHED (IF ANY)
         insert(i,abs(formula[clause_pointer[i]+1]),2*i+1, 1, V2C,W, NEXT,C2V);
      } else { //*** READY TO APPLY UNIT PROPAGATION FOR THIS LITERAL (no repetition control!!! - this might cause error)
         U++;
         UNIT[U] = formula[clause_pointer[i]]; // Positive or negative
      }
   }   
   
  
  /*
   printf("**** DATA STRUCTURE *****************************************\n");  
   for(i=0;i<NC;i++)
     printf("Watched clausola %d = %d, %d\n", i, C2V[i][0], C2V[i][1]);
   for(i=0;i<2*NC;i++)
     printf("Linked structure i=%d, (%d,%d) \n", i, W[i], NEXT[i]);
   for(i=1;i<NV;i++)
     printf("Var pointer V2C[%d] = %d\n", i, V2C[i] );
   printf("**********************************************************\n");  
  */
      sat = unit_propagation(&U,UNIT,V2C,W,NEXT,C2V,&top,varstack,&SEL);
      
/*
   printf("EXITED U.P. with U=%d, SEL=%d, top=%d, sat=%d\n", U, SEL, top, sat);
   for(i=1; i <NV; i++)printf("v[%d]=%d",i,host_vars[i]);
   printf("\n");
*/

      if (sat == 0) return 0;    // *** EXIT WITH FAILURE      
      
      if (top == NV-2) return 1; // *** EXIT WITH SUCCESS (trivial instance)

      // Heuristic first seed, if not set by unit_propagation
      
      if (SEL == 0) SEL = first_seed(host_vars,V2C); 

      // *** Initialize the stack with a backtrackable var      
      top++;
      varstack[top][0]= SEL;   // E.g. v satisfied by 1 -> -1
      varstack[top][1]= (SEL>0)-2; // -v satisfied by 0 -> -2     
                       
//****************************************************************************
//*** MAIN LOOP
//****************************************************************************
      
      int dbg=0;

      while((top >=0) && (top < NV-1)){
	proceed = 0;
	while((top >= 0) && !proceed){  
	  if (varstack[top][1] > 1){ // *** no longer  backtrackable
	    if (dbg) printf("CASE 1 - pop of var[%d]-\n",abs(varstack[top][0]));
	    host_vars[abs(varstack[top][0])] = -1;
	    varstack[top][1] = -1;
	    top--;// Restore unknown status              
	  } else if (varstack[top][1] < 0) {// Never tried value. Try and update
	    if (dbg)  printf("CASE 2 - 1st attempt for var[%d]=%d\n",abs(varstack[top][0]),2+varstack[top][1]);
	    proceed = 1;
	    host_vars[abs(varstack[top][0])] = 2+varstack[top][1]; 
	    varstack[top][1] = host_vars[abs(varstack[top][0])];
	  } else if (varstack[top][1] < 2) {// Last chance
	    if (dbg)  printf("CASE 3 - 2nd attempt  for var[%d]=%d\n",abs(varstack[top][0]),1-varstack[top][1]);
	    proceed = 1;
	    host_vars[abs(varstack[top][0])] = 1-varstack[top][1];
	    varstack[top][1] = 2;
	  } 
        }// *** while (! proceed)
             
	if (top >= 0){
	  U++;
	  UNIT[U]= varstack[top][0];
	  //*** CALL TO UNIT PROPAGATION  
	  sat = unit_propagation(&U,UNIT,V2C,W,NEXT,C2V,&top,varstack,&SEL);
	  //printf("sat %d, top %d, NV %d\n",sat,top,NV);

	  // ALE: check sat, cosi' esce subito (novita' per non creare problemi all'aggancio con GPU)
	  if (sat && (top >=NV-2))
	    return 1;

	  if ( (NV-1-top-1<MaxV) && USEGPU==5){ // uso cudadpll_w
	    if (sat && (top < NV -2) && NV-1-top-1>0){ // almeno una FV (anche se io eviterei proprio di chiamare...)
	      //printf("CUDA CALLER: free vars %d, %d %d\n",NV-1-top-1,NV,top);
	      sat=CUDA_caller_watched();
	      if (sat)
		return sat; // altrimenti prosegue come se sat falso (backtracking)
	    }
	    else{ // qui entra se non sat (e backtrack), oppure sat, ma con 0 FV e quindi non chiama cudacaller
	      //	      printf("sat %d\n",sat);
	    }
	  }
	  else 
	      {
	      if (sat && (top < NV -1) && !SEL){
		//*** SEL becomes the next entry for U.P.
		i=1; temp=1;
		while(temp && (i < NV))
		  if( (host_vars[i] < 0) )  
		    temp = 0;
		  else i++;
		SEL=i;
	      } 
              
	      if (sat && SEL)  {
		top++;
		varstack[top][0]=SEL;
		varstack[top][1]=(SEL >0) -2;     
	      }
	      }
	} // if (top>=0)      
      } // *** main loop     
      printf("sat %d\n",sat);
      return sat;
}  


//************************************************************************
//************************************************************************
//*******************       main        **********************************
//************************************************************************
//************************************************************************

__host__ int  main(int argc, char** argv) {
    int retv = 0;
    cudaDeviceProp prop;
    int whichDevice;
    
//INITandy
    NUMPARAMETERS = 9;
    if (argc!=NUMPARAMETERS){
      printf("usage: %s mode N_vars_blocks(2LOG_BLKS) N_vars_thread(LOG_THRDS) Maxv learning(0/1) varSelectStrategyCPU(0-4) varSortGPU(0/1) filename\n",argv[0]);
      return -1;
    }    
//FINEandy
    
    // *** PARAMETER SETTINGS
    USEGPU      = atoi(argv[1]);
    LOG_BLKS    = atoi(argv[2]);
    LOG_THRDS   = atoi(argv[3]);
    CUDABLOCKS  = (1<<LOG_BLKS);   //*** pow(2,LOG_BLKS); 
    CUDATHREADS = (1<<LOG_THRDS);  //*** pow(2,LOG_THRDS);  
    MaxV        = atoi(argv[4]);  
    Delta       = max(1,MaxV - LOG_BLKS + 1); // *** In case MaxV < LOG_BLKS use default "1"  
    learning    = atoi(argv[5]);
//INITandy
    strategia_varSelHost    = atoi(argv[6]);
    strategia_maps_varSort    = atoi(argv[7]); //usa strategia_varSelHost anche per riordino di maps_vars (1 yes, <>1 no)
//FINEandy

    if (USEGPU>0){
      // CUDA timings initialization
      cudaEventCreate( &start );
      cudaEventCreate( &stop );
      // Checking of CUDA HW capabilities
      HANDLE_ERROR( cudaGetDevice( &whichDevice ) );
      HANDLE_ERROR( cudaGetDeviceProperties( &prop, whichDevice ) );
      if (prop.canMapHostMemory != 1) {
	            printf( "Device cannot map memory.\n" );
	  return 0;
      }
    }

    //*** Some "warning" and exit
    if(USEGPU>0 && CUDATHREADS > prop.maxThreadsPerBlock){
      printf("Cant handle so many vars per block (max %d < %d threads per block)\n",CUDATHREADS,prop.maxThreadsPerBlock);
      return -1;
    }
    printf("using LOG_BLKS=%d, LOG_THRDS=%d MaxV=%d \n",LOG_BLKS,LOG_THRDS,MaxV);
    if (Delta > DV_SIZE){
        printf("Delta too large, recompile kernel with %d in the size of arrays\n",Delta);
        return -1;
    }    
    
    //**** CORE COMPUTATION
     allocate_first();
    // *** Load a SAT formula in DIMACS format

     load_formula(argv[NUMPARAMETERS-1],&NV,&NC,&NL);

    if (USEGPU %2 == 0) {
    	printf("INFO: Variable selection strategy (host): ");
    	print_option(strategia_varSelHost); printf("\n");
    }

    //*** Print and use some HW and instance info     
     if (USEGPU>0){
       print_info(prop);
       PARblocks = IMIN( ( NC + THREADS - 1) / THREADS, 2*prop.multiProcessorCount );
     } 
    
     allocate_second();
    //*** Start running time (formula loading is not counted)    
     if (USEGPU %2 == 1 && USEGPU<4)
         printf("blocks %d, threads %d\n",PARblocks, THREADS);
     srand ( time(NULL) );
     printf("start\n");
     
     if (USEGPU>0){
       cudaEventRecord(start,0);
       cudaEventSynchronize(start);    
     }else{
#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
       global_time=clock();
#else
       gettimeofday(&time_start,NULL);
#endif
     }

//*********************************************************************
//******* Call the main procedure (store the result in vars[0]): 
//*********************************************************************
      
     if (USEGPU < 4){      
       retv = twolevel_DPLL();
       host_vars[0] = retv;  
     }
     else // for mode 4 and 5
       host_vars[0] = watchedDPLL();
    
    //*** CUDA timings:
    if (USEGPU>0){
      cudaEventRecord( stop, 0 );    
      cudaEventSynchronize( stop );    
      cudaEventElapsedTime( &deltatime, start, stop );
    }
    else{
#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
     clock_t final=clock()-global_time;
     deltatime=((float)final/CLK_TCK)*1000;
#else
     gettimeofday(&time_stop,NULL);
     deltatime=(0.0+time_stop.tv_sec+time_stop.tv_usec/1000000.0) - (0.0+time_start.tv_sec+time_start.tv_usec/1000000.0);
     deltatime*=1000;
#endif
    }

//printf("TEST 5000\n");
    /* DISPLAY RESULT AND RUNNING TIME: */
    print_result( host_vars, NV );    
    print_time( deltatime );
             
    deallocate();
    return 0;
}            

//********* END *******************************************************
