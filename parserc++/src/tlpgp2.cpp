#include "tlpgp2.h"

extern int g_pid;

Tlpgp2::Tlpgp2(){}

Tlpgp2::Tlpgp2(Vertex * graph):m_graph(graph){}

Tlpgp2::~Tlpgp2(){}

string Tlpgp2::generateGraphSmt2(){
	cout<<"traduction in smt2 clause\n";
	//int pid = int(getpid());
	string namefile = to_string(g_pid)+"tlpgp2.smt2";
	ofstream file(namefile, ios::out | ios::trunc );
	if (file){
		file << "(set-option :produce-models true)\n";
		string assert = "(assert (and GoalsE0\n";
		string get_value="(get-value (";
		string name;
		int state =0;
		Vertex * actual = new Vertex(m_graph);
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(vector<DurativeAction *>::iterator it = actual->getActions()->begin() ; it != actual->getActions()->end() ; ++it){
				name=(*it)->getName()+"E"+to_string(state);
				for(vector<Variable >::iterator it2 = (*it)->getParameters()->begin() ;it2 != (*it)->getParameters()->end();++it2){
					name+=(*it2).getName();
				}
				file << "(declare-fun "<<name<<" () Bool )\n";
				get_value +=name+"\n";
				get_value +="t_"+name+"\n";
				file << "(declare-fun t_"<<name<<" () Int )\n";

				for(unsigned i = 0 ; i < (*it)->getPreconditions2().size() ; ++i){
					file << linkPrec(name,(*it)->getPreconditions2().at(i).second,(*it)->getPreconditions2().at(i).first,actual->getFather(),state+1,&assert);
				}
				for(unsigned i = 0 ; i < (*it)->getEffects().size() ; ++i){
					protectEffect(name,(*it)->getEffects().at(i).second,(*it)->getEffects().at(i).first,&assert);
				}
			}
		state++;
		}
		assert+= "InitsE"+to_string(state-1)+"\n";
		assert+= "(< t_InitsE"+to_string(state-1)+" 1 )\n";	//init must be at 0
		file<<assert<<"))\n(check-sat)\n";
		file<<get_value+" ) )\n(exit)\n";
		file.close();
	}
	cout<<"end traduction\n";
	return namefile;
}

string Tlpgp2::linkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state,string * assert){
	string ret="";
	string step2 ="( or  (not "+name+") ";
	string step3,step4;
	string namea,namef,link;
	string get_value="(get-value (";
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
						namea=actual->getActions()->at(j)->getName()+"E"+to_string(s);
						for(vector<Variable >::iterator it2 = actual->getActions()->at(j)->getParameters()->begin() ;it2 != actual->getActions()->at(j)->getParameters()->end();++it2){
							namea+=(*it2).getName();
						}
						link="Link_"+namea+"."+namef+"."+name;
						step2 +=link+" ";
						ret+="(declare-fun "+link+" () Bool )\n";
						step3 +="( or (not "+link+") "+namea+")\n( or (not "+link+") "+name+")\n";
						if (att.getSupported()->at(inter).getStartBracket() ){
							step3 +="( or (not "+link+") (>= (+ t_"+name+" "+to_string((int)att.getSupported()->at(inter).getStart())+")  (+ t_"+namea+" "+to_string((int)actual->getActions()->at(j)->getEffects().at(i).first.getSupported()->at(0).getStart())+" )))\n";
						} else {
							step3 +="( or (not "+link+") (> (+ t_"+name+" "+to_string((int)att.getSupported()->at(inter).getStart())+")  (+ t_"+namea+" "+to_string((int)actual->getActions()->at(j)->getEffects().at(i).first.getSupported()->at(0).getStart())+" )))\n";
						}
						step4 = protectCond(link,fluent,att,name);
					}
				}
			}
			s++;
			(*assert) +=step3+step4;
		}
		
	}
	(*assert) +=step2+")\n";
	return ret;
}


string Tlpgp2::protectCond(string link,Fluent * fluent,Attribute att,string name){
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
					ret+="(< (+ t_"+namea+" "+to_string((int)actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+" ) (+ t_"+name+" "+to_string((int)att.getSupported()->at(0).getStart())+")) ";
					ret+="(< (+ t_"+name+" "+to_string((int)att.getSupported()->at(0).getEnd())+" ) (+ t_"+namea+" "+to_string((int)actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+" )))";
				}
			}
		}
	state++;
	}	
	return ret;
}

void Tlpgp2::protectEffect(string name,Fluent * fluent,Attribute att,string * assert){
	Vertex * actual = new Vertex(m_graph);
	string namea;
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
					(*assert)+="(or (not "+namea+") (not "+name+")";
					(*assert)+="(< (+ t_"+namea+" "+to_string(actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getStart())+") (+ t_"+name+" "+to_string(att.getSupported()->at(0).getStart())+") )";
					(*assert)+="(> (+ t_"+namea+" "+to_string(actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getStart())+") (+ t_"+name+" "+to_string(att.getSupported()->at(0).getStart())+") )";
				}
			}
		}
	state++;
	}
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
/*
string Tlpgp2::generateGraphSmt2(){
	string namefile = "tlpgp2.smt2";
	ofstream file(namefile, ios::out | ios::trunc );
	if (file){
		Vertex * actual = m_graph;
		file << "(set-option :produce-models true)\n";
		string name,nameb,namet,namef,namea;
		string assert = "(assert (and \n";
		bool c = true;
		int state =0;
		float time;
		string get_value="(get-value (";
		vector<pair<string,float> >  destroyAction=vector<pair<string,float> > ();
		while(c){
			for(vector<DurativeAction *>::iterator it = actual->getActions()->begin() ; it != actual->getActions()->end() ; ++it){
				destroyAction=vector<pair<string,float> > ();
				name=(*it)->getName()+"E"+to_string(state);
				for(vector<Variable >::iterator it2 = (*it)->getParameters()->begin() ;it2 != (*it)->getParameters()->end();++it2){
					name+=(*it2).getName();
				}
				file << "(declare-fun "<<name<<" () Bool )\n";
				get_value +=name+"\n";
				get_value +="t_"+name+"\n";
				name = "t_"+name;
				file << "(declare-fun "<<name<<" () Int )\n";

			}
			for(vector<Edge *>::iterator it = actual->getEdgest()->begin() ; it !=  actual->getEdgest()->end() ; ++it){
				nameb = (*it)->getActionb()->getName()+"E"+to_string(state);
				for(vector<Variable >::iterator it2 = (*it)->getActionb()->getParameters()->begin() ;it2 != (*it)->getActionb()->getParameters()->end();++it2){
					nameb+=(*it2).getName();
				}
				namet = (*it)->getActiont()->getName()+"E"+to_string(state+1);
				for(vector<Variable >::iterator it2 = (*it)->getActiont()->getParameters()->begin() ;it2 != (*it)->getActiont()->getParameters()->end();++it2){
					namet+=(*it2).getName();
				}	
				namef= (*it)->getFluent()->getPredicate()->getName();
				for(vector<Member * >::iterator it2 = (*it)->getFluent()->getMembersList()->begin() ;it2 != (*it)->getFluent()->getMembersList()->end();++it2){
					namef+=(*it2)->getName();
				}	
				name = "link_"+nameb+"."+namef+"."+namet;
				file << "(declare-fun "<<name<<" () Bool )\n";
				//step 2
				assert+="( or  (not "+nameb+") "+name+" )\n";
				//step 3
				assert+="( or (not "+name+") "+nameb+" )\n";
				assert+="( or (not "+name+") "+namet+" )\n";
				assert+="( or (not "+name+") ( >= (- t_"+namet+" t_"+nameb+") 0 ) )\n";
				//step4
				destroyAction=findDestroyAction((*it)->getFluent());	
				for(vector<pair<string,float> >::iterator it2 = destroyAction.begin() ; it2 != destroyAction.end() ; ++it2){
					namea=(*it2).first;
					time = (*it2).second;
					assert+="(or (not "+name+") (not "+namea+") (> (- t_"+nameb+" t_"+namea+") "+to_string((int)time)+") (< (- t_"+namet+" t_"+namea+") "+to_string((int)time)+" ) )\n";
				}
			}
			if (actual->getEdgest()->size() != 0){
				actual = actual->getEdgest()->at(0)->getTop();
			} else {
				c =false;
			}
			state++;
		}
		//step 5 
		for(vector<Edge *>::iterator it = m_graph->getEdgest()->begin() ; it != m_graph->getEdgest()->end() ; ++it){
			name=(*it)->getActiont()->getName()+"E1";
			for(vector<Variable >::iterator it2 = (*it)->getActiont()->getParameters()->begin() ;it2 != (*it)->getActiont()->getParameters()->end();++it2){
				name+=(*it2).getName();
			}
			assert+="(>= t_"+name+" t_InitsE0 )\n";
		}
		for(vector<Edge *>::iterator it = actual->getEdgesb()->begin() ; it != actual->getEdgesb()->end() ; ++it){
			name=(*it)->getActionb()->getName()+"E"+to_string(state-2)+"";
			for(vector<Variable >::iterator it2 = (*it)->getActionb()->getParameters()->begin() ;it2 != (*it)->getActionb()->getParameters()->end();++it2){
				name+=(*it2).getName();
			}
			time=findTimeEff((*it)->getFluent(),(*it)->getActionb());
			assert+="(>= (- t_GoalsE"+to_string(state-1)+" t_"+name+") "+to_string((int)time)+")\n";
		}
		assert+="(>= t_GoalsE"+to_string(state-1)+" t_InitsE0 )\n";
		//step 1
		assert +=" InitsE0 GoalsE"+to_string(state-1)+" ) )\n";
		file<<assert;
		file<<"\n(check-sat)\n";
		file<<get_value+" ) )\n(exit)\n";
		file.close();
	} else {
		cerr<<"erreur a l'ouverture de "<<namefile<<endl;
	}
	return namefile;
}

vector<pair<string,float> > Tlpgp2::findDestroyAction(Fluent * f){
	int state =0;
	vector<pair<string,float> > ret = vector<pair<string,float> >();
	Vertex * actual = m_graph;
	bool c = true;
	string name;
	float t;
	while(c){
			for(vector<DurativeAction *>::iterator it = actual->getActions()->begin() ; it != actual->getActions()->end() ; ++it){		
if (isdestroy((*it)->getNotEffectsF(),f)){		
					name = (*it)->getName()+"E"+to_string(state);
					for(unsigned int i = 0 ; i < (*it)->getParameters()->size() ;++i){
						name+=(*it)->getParameters()->at(i).getName();
					}

					for(unsigned int i = 0 ; i <(*it)->getNotEffects().size() ;++i){
						if (f->getPredicate()->getName() == (*it)->getNotEffects().at(i).second->getPredicate()->getName()){
							if (compareVV(f->getMembersList(),(*it)->getNotEffects().at(i).second->getMembersList() )){
								t=(*it)->getNotEffects().at(i).first.getTime();
								ret.push_back(make_pair(name,t));
							}
						}
					}
				}
			}
		if (actual->getEdgest()->size() != 0){
			actual = actual->getEdgest()->at(0)->getTop();
		} else {
			c =false;
		}
		state++;
	}
	return ret;
}

bool Tlpgp2::isdestroy(vector<Fluent  *> v,Fluent * f){
	for(unsigned i =0 ; i<v.size();++i){
		if (f->getPredicate()->getName() == v.at(i)->getPredicate()->getName()){
			if (compareVV(f->getMembersList(),v.at(i)->getMembersList() )){
				return true;
			}
		}
	}
	return false;
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

float Tlpgp2::findTimeEff(Fluent * f,DurativeAction * a){
	float t;
	for(unsigned i = 0 ; i < a->getEffects().size() ; ++i){
		if (f->getPredicate()->getName() ==  a->getEffects().at(i).second->getPredicate()->getName()){
			if (compareVV(f->getMembersList(), a->getEffects().at(i).second->getMembersList() )){
				t=a->getEffects().at(i).first.getTime();
			}
		}
	}
	return t;
}*/
