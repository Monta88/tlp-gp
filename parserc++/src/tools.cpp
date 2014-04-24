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
		if ((*it)->getName() == t->getName())	{
			return true;
		}
	}
	return false;
}

void Tools::solveur(){
	pid_t pid = fork();
	if (pid == 0){
	    // child

		// redirect stdout to a file, and stdin from a another file
		freopen ("tlpgp2Res.txt","w",stdout);

		// execute mathsat
		system("./mathsat < tlpgp2.smt2");
	}
	else{
		//parent
	wait(pid);
	ifstream file("tlpgp2Res.txt", ios::in); 
 	string line;
	const char * cline;
	string action;
	bool m;
	string plan="";
        if(file) {
		getline(file, line);
		if (line == "sat"){
		       	while(getline(file, line))  {
				m = true;
				action="";
				cline = line.c_str();
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
								plan +=action+"    |    ";
								i = line.size();
							}
						} else {
							action+=cline[i];
						}
					}
				}
			}
		} else {
			cout<<"solution not found\n";	
		}
	file.close();
        }
        else{
                cerr << "can't open the answer solver" << endl;
	}
	cout<<"solution "<<plan<<"\n"<<"\nend\n";
	}
 
}

