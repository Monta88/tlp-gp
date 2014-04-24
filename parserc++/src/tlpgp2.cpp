#include "tlpgp2.h"

Tlpgp2::Tlpgp2(){}

Tlpgp2::Tlpgp2(Vertex * vertex):m_graph(vertex){}

Tlpgp2::~Tlpgp2(){}

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
		file<<get_value+"t_InitsE0 t_GoalsE"+to_string(state-1)+" ) )\n(exit)\n";
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
}
