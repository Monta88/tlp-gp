#include "tools.h"
Tools::Tools(){}
		
Tools::~Tools(){
	// TODO Auto-generated destructor stub
}

// return true is the 2 vector have the same types
bool Tools::compareVectorType(vector<Type*>* v1,vector<Type*>* v2){
	for (vector<Type*>::iterator it = v1->begin() ; it != v1->end() ; ++it){
		if (!isIn((*it),v2)) {
			return false;
		}
	}
	return true;
}

bool Tools::isIn(Type* t ,vector<Type*>* v){						
	for (vector<Type*>::iterator it = v->begin() ; it != v->end() ; ++it){
		if (((*it)->getName() == t->getName() )|| t->isOneOfParents((*it)->getName()))	{
			return true;
		}
	}
	return false;
}

/*bool Tools::isIn(Type* t ,vector<Type*>* v){		
	vector<Type *> * fatherType = getFatherType(*it);			
	for (vector<Type*>::iterator it = v->begin() ; it != v->end() ; ++it){
		for (vector<Type*>::iterator it2 = getFatherType->begin() ; it2 != getFatherType->end() ; ++it2){ 
			if ((*it)->getName() == (*it2)->getName())	{
			return true;
		}
	}
	return false;
}

vector<Type *>* Tools::getFatherType(Type * t){
	vector<Type * > * ret = new vector<Type *>();	
	ret->push_back(t);
	Type *type =t;
	while(t->
}*/

bool Tools::solveur(){
	pid_t pid = fork();
	if (pid == 0){
	    // child

		// redirect stdout to a file, and stdin from a another file
		freopen ("tlpgp2Res.txt","w",stdout);

		// execute mathsat
		system("./mathsat < tlpgp2.smt2");
		exit(0);
	}
	else{
		//parent
	wait(NULL);
	ifstream file("tlpgp2Res.txt", ios::in); 
 	string line;
	const char * cline;
	string action,number;
	bool m,n,o;
	vector<pair<string,int> > plan=vector<pair<string,int> >();
        if(file) {
		getline(file, line);
		if (line == "sat"){
		       	while(getline(file, line))  {
				cline = line.c_str();
				n = true;
				for (unsigned int i=0 ; i < line.size() ; ++i){
					if (cline[i] == '(' && cline[i+1] == 't' && cline[i+2] == '_'){
						i = line.size();
						n = false;
					}
				}
				m = true;
				action="";
				if ( n ){
					for (unsigned int i=0 ; i < line.size() ; ++i){
						if ( m ){
							if ( cline[i] != '(' && cline[i] != ' '){
								action+=cline[i];
							}
							if ( cline[i] == 'E' ){
								//cout<<"lol\n";
								m = false ;
							}
						} else {
							if ( cline[i] == ' '){
								if (cline[i+1] == 't'){
									i = line.size();
									getline(file, line);
									number="";
									o = true;
									cline = line.c_str();
									for (unsigned int j=0 ; j < line.size() ; ++j){
										if ( o ){
											if (cline[j] == ' ' && (cline[j+1] == '0' || cline[j+1] == '1' || cline[j+1] == '2' || cline[j+1] == '3' || cline[j+1] == '4' || cline[j+1] == '5' || cline[j+1] == '6' || cline[j+1] == '7' || cline[j+1] == '8' || cline[j+1] == '9')){
												o = false;
											}
											if (cline[j] == ' ' && cline[j+1] == '(' && cline[j+2] != 't'){
												j=j+3;
												o = false;
											}
										} else {
											if ( cline[j] != ')'){
												number+=cline[j];
											} else {
												j = line.size();
											}
										}
									}
									plan.push_back(make_pair(action,atoi(number.c_str())));
								} else {
									i = line.size();						
								}							
							} else {
								action+=cline[i];
							}
						}
					}
				}
			}
		} else {
			return false;
		}
	cout<<"solution : \n";
	for(vector<pair<string,int> >::iterator it = plan.begin() ; it != plan.end() ; ++it){
		cout<<(*it).first<<" at "<<(*it).second<<"\n";
	}
	file.close();
        }
        else{
                cerr << "can't open the answer solver" << endl;
	}
	return true;
	}
 
}

