
#include "tlpgp2.h"

extern int g_pid;

Tlpgp2::Tlpgp2(){}

Tlpgp2::Tlpgp2(Vertex * graph):m_graph(graph){}

Tlpgp2::~Tlpgp2(){}

string Tlpgp2::generateSmt2(){
	cout<<"traduction in smt2 clause\n";
	string namefile = to_string(g_pid)+"tlpgp2.smt2";
	ofstream file(namefile, ios::out | ios::trunc );
	DurativeAction * action;
	m_nbClause = 0;
	m_nbVariableProp = 0;
	m_nbVariableReal = 0;
	realGraph();
	if (file){
		file << "(set-option :produce-models true)\n";
		
		string assert;
		string name;
		int state =0;
		Vertex * actual = new Vertex(m_graph);
		
		//first we have to create the variables
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				action = actual->getActions()->at(j);
				name=actual->getActions()->at(j)->getName()+"E"+to_string(state);
				for(unsigned i = 0 ; i < action->getParameters()->size() ; ++i){
					name+=action->getParameters()->at(i).getName();
				}
				file << "(declare-fun "<<name<<" () Bool )\n";
				file << "(declare-fun t_"<<name<<" () Int )\n";
				m_nbVariableProp++;
				m_nbVariableReal++;
				for(unsigned i = 0 ; i < action->getPreconditions2().size() ; ++i){
					file << namelinkPrec(name,action->getPreconditions2().at(i).second,action->getPreconditions2().at(i).first,actual->getFather(),state+1);
				}
			}
		state++;
		}
		// now the clause tlpgp2
		file << "(assert (and GoalsE0\n";
		actual = new Vertex(m_graph);
		state =0;
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				action = actual->getActions()->at(j);
				name=actual->getActions()->at(j)->getName()+"E"+to_string(state);
				for(unsigned i = 0 ; i < action->getParameters()->size() ; ++i){
					name+=action->getParameters()->at(i).getName();
				}
				file <<"(>= t_"+name+" 0 )\n";
				m_nbClause++;
				for(unsigned i = 0 ; i < action->getPreconditions2().size() ; ++i){
					file << linkPrec(name,action->getPreconditions2().at(i).second,action->getPreconditions2().at(i).first,actual->getFather(),state+1);
				}
				for(unsigned i = 0 ; i < action->getEffects().size() ; ++i){
					//step 4.2
					file << protectEffect(name,action,action->getEffects().at(i).second,action->getEffects().at(i).first,state);
				}
			}
		state++;
		}
		file<< "InitsE"+to_string(state-1)+"\n";
		file<<"(< t_InitsE"+to_string(state-1)+" 1 )\n))\n";	//init must be at 0
		m_nbClause+=2;
		// and to finish what we want the solver return
		
		file<<"(check-sat)\n";
		file<<"(get-value (";
		actual = new Vertex(m_graph);
		state =0;
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				action = actual->getActions()->at(j);
				name=actual->getActions()->at(j)->getName()+"E"+to_string(state);
				for(unsigned i = 0 ; i < action->getParameters()->size() ; ++i){
					name+=action->getParameters()->at(i).getName();
				}
				file<<name+"\n";
				file<<"t_"+name+"\n";
			}
		state++;
		}
		
		file<<" ) )\n(exit)\n";
		file.close();
	}
	cout<<"end traduction nb clauses : "<<m_nbClause<<" nb Variables propositionnels : "<<m_nbVariableProp<<" nb Variables réelle :"<<m_nbVariableReal<<"\n";
	return namefile;
}

string Tlpgp2::namelinkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state){
	string ret="";
	string namea,namef,link;
	namef= fluent->getPredicate()->getName();
	for(vector<Member * >::iterator it =fluent->getMembersList()->begin() ;it != fluent->getMembersList()->end();++it){
		namef+=(*it)->getName();
	}
	Vertex * actual = new Vertex(vertex);
	int s = state;
	for (unsigned inter = 0 ; inter < att.getSupported()->size() ; ++inter ){
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				for (unsigned i = 0 ; i < actual->getActions()->at(j)->getEffects().size() ; ++i){	
					if ( compareVV(actual->getActions()->at(j)->getEffects().at(i).second->getMembersList(),fluent->getMembersList()) && (actual->getActions()->at(j)->getEffects().at(i).second->getPredicate()->getName() == fluent->getPredicate()->getName()) ){

						namea=actual->getActions()->at(j)->getName()+"E"+to_string(s);
						for(vector<Variable >::iterator it2 = actual->getActions()->at(j)->getParameters()->begin() ;it2 != actual->getActions()->at(j)->getParameters()->end();++it2){
							namea+=(*it2).getName();
						}
						link="Link_"+namea+"."+namef+"."+name;
						ret+="(declare-fun "+link+" () Bool )\n";
						m_nbVariableProp++;
					}
				}
			}
			s++;
		}
		
	}
	return ret;
}

string Tlpgp2::linkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state){
	string ret;
	string step2 ="( or  (not "+name+") ";
	string step3,step4;
	string namea,namef,link;
	namef= fluent->getPredicate()->getName();
	for(vector<Member * >::iterator it =fluent->getMembersList()->begin() ;it != fluent->getMembersList()->end();++it){
		namef+=(*it)->getName();
	}
	Vertex * actual = new Vertex(vertex);
	int s = state;
	for (unsigned inter = 0 ; inter < att.getSupported()->size() ; ++inter ){
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				for (unsigned i = 0 ; i < actual->getActions()->at(j)->getEffects().size() ; ++i){	
					if ( compareVV(actual->getActions()->at(j)->getEffects().at(i).second->getMembersList(),fluent->getMembersList()) && (actual->getActions()->at(j)->getEffects().at(i).second->getPredicate()->getName() == fluent->getPredicate()->getName()) ){
						step3 = "";
						step4= "";
						namea=actual->getActions()->at(j)->getName()+"E"+to_string(s);
						for(vector<Variable >::iterator it2 = actual->getActions()->at(j)->getParameters()->begin() ;it2 != actual->getActions()->at(j)->getParameters()->end();++it2){
							namea+=(*it2).getName();
						}
						link="Link_"+namea+"."+namef+"."+name;
						step2 +=link+" ";
						step3 +="( or (not "+link+") "+namea+")\n( or (not "+link+") "+name+")\n";
						if (att.getSupported()->at(inter).getStartBracket() ){
							step3 +="( or (not "+link+") (>= (+ t_"+name+" "+to_string((int)att.getSupported()->at(inter).getStart())+")  (+ t_"+namea+" "+to_string((int)actual->getActions()->at(j)->getEffects().at(i).first.getSupported()->at(0).getStart())+" )))\n";
						} else {
							step3 +="( or (not "+link+") (> (+ t_"+name+" "+to_string((int)att.getSupported()->at(inter).getStart())+")  (+ t_"+namea+" "+to_string((int)actual->getActions()->at(j)->getEffects().at(i).first.getSupported()->at(0).getStart())+" )))\n";		
						}//step 4.1

						ret +=protectCond(link,fluent,att,namea,name);

						ret +=step3;
						m_nbClause+=3;
						
						
					}
				}
			}
			s++;
		}
		
	}
	ret +=step2+")\n";
	m_nbClause++;
	return ret;
}


string Tlpgp2::protectCond(string link,Fluent * fluent,Attribute att,string nameb,string namet){
	string ret="";
	string namea;
	Vertex * actual = new Vertex(m_graph);
	int state = 0;
	while(actual->getFather() != NULL ){
		actual = actual->getFather();
		for(unsigned int i =0 ; i < actual->getActions()->size() ; ++i ){
			for (unsigned int j =0 ; j < actual->getActions()->at(i)->getNotEffects().size() ; ++j){
				if ( compareVV(actual->getActions()->at(i)->getNotEffects().at(j).second->getMembersList(),fluent->getMembersList()) && (actual->getActions()->at(i)->getNotEffects().at(j).second->getPredicate()->getName() == fluent->getPredicate()->getName()) ){
					namea=actual->getActions()->at(i)->getName()+"E"+to_string(state);
					for(vector<Variable >::iterator it2 = actual->getActions()->at(i)->getParameters()->begin() ;it2 != actual->getActions()->at(i)->getParameters()->end();++it2){
						namea+=(*it2).getName();
					}
					ret+="(or (not "+link+") (not "+namea+") ";
					ret+="(< (+ t_"+nameb+" "+to_string((int)actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+" ) (+ t_"+namea+" "+to_string((int)att.getSupported()->at(0).getStart())+")) ";
					ret+="(> (+ t_"+namet+" "+to_string((int)att.getSupported()->at(0).getStart())+" ) (+ t_"+namea+" "+to_string((int)actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+" )))\n";
					m_nbClause++;
				}
			}
		}
	state++;
	}	
	return ret;
}

string Tlpgp2::protectEffect(string name,DurativeAction * a,Fluent * fluent,Attribute att,int statep){
	string ret="";
	Vertex * actual = new Vertex(m_graph);
	string namea;
	int state = 0;
	while(actual->getFather() != NULL ){
		actual = actual->getFather();
		for(unsigned int i =0 ; i < actual->getActions()->size() ; ++i ){
			if ( ! (a->getName() == actual->getActions()->at(i)->getName() && statep == state)){
				for (unsigned int j =0 ; j < actual->getActions()->at(i)->getNotEffects().size() ; ++j){
					if ( compareVV(actual->getActions()->at(i)->getNotEffects().at(j).second->getMembersList(),fluent->getMembersList()) && (actual->getActions()->at(i)->getNotEffects().at(j).second->getPredicate()->getName() == fluent->getPredicate()->getName()) ){
						namea=actual->getActions()->at(i)->getName()+"E"+to_string(state);
						for(vector<Variable >::iterator it2 = actual->getActions()->at(i)->getParameters()->begin() ;it2 != actual->getActions()->at(i)->getParameters()->end();++it2){
							namea+=(*it2).getName();
						}	
						ret+="(or (not "+namea+") (not "+name+") ";
						ret+="(not (= (+ t_"+namea+" "+to_string(actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+") (+ t_"+name+" "+to_string(att.getSupported()->at(0).getStart())+") ) ) )\n";
						m_nbClause++;
					}
				}
			} 
		}
	state++;
	}
	return ret;
}

bool Tlpgp2::compareVV(vector<Member * >* v1 ,vector<Member * >*v2){
	if (v1->size () != v2->size()){
		return false;
	}
	for(unsigned i = 0 ; i < v1->size(); ++i ){
		if ( ! (v1->at(i)->getName() == v2->at(i)->getName())){
			return false;
		}		
	}
	return true;
}

void Tlpgp2::realGraph(){
	m_realGraph = new Vertex(NULL);
	Vertex * next = new Vertex(NULL);
	Vertex * ractual = new Vertex(m_realGraph);
	Vertex * actual = new Vertex(m_graph);
	actual = actual->getFather();
	ractual = ractual->getFather();
	ractual->addActions(actual->getActions());
	while( actual->getFather() != NULL ){
		actual = actual->getFather();
		for(unsigned i = 0 ; i < ractual->getActions()->size() ; ++i){
			for(unsigned j = 0 ; j < ractual->getActions()->at(i)->getPreconditions2().size() ; ++j){
				if (ractual->getFather() == NULL){
					ractual->addFather(next);
					next = new Vertex(NULL);
				}
				actionsNeeded(ractual->getActions()->at(i)->getPreconditions2().at(j).second,ractual->getActions()->at(i)->getPreconditions2().at(j).first,actual,*(ractual->getFather()));
			}
		}
		ractual = ractual->getFather();	
	}
}

void Tlpgp2::actionsNeeded(Fluent * fluent ,Attribute att, Vertex * vertex, Vertex rvertex){
	Tools tool = Tools();
	Vertex * next = new Vertex(NULL);
	Vertex * actual = new Vertex(vertex);
	Vertex * ractual = new Vertex(&rvertex);
	for (unsigned inter = 0 ; inter < att.getSupported()->size() ; ++inter ){
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			if (ractual->getFather() != NULL){
				ractual = ractual->getFather();
			} else {
				ractual->addFather(next);
				ractual = ractual->getFather();
				next = new Vertex(NULL);
			}
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				for (unsigned i = 0 ; i < actual->getActions()->at(j)->getEffects().size() ; ++i){
					if ( compareVV(actual->getActions()->at(j)->getEffects().at(i).second->getMembersList(),fluent->getMembersList()) && (actual->getActions()->at(j)->getEffects().at(i).second->getPredicate()->getName() == fluent->getPredicate()->getName()) ){
						if ( !tool.compareActionVaction(actual->getActions()->at(j),ractual->getActions()) ){
								ractual->addAction(actual->getActions()->at(j));
						}
					}
				}
			}
		}
		
	}
}

