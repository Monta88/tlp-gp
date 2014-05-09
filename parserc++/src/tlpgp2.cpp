#include "tlpgp2.h"

extern int g_pid;

Tlpgp2::Tlpgp2(){}

Tlpgp2::Tlpgp2(Vertex * graph):m_graph(graph){}

Tlpgp2::~Tlpgp2(){}

string Tlpgp2::generateGraphSmt2(){
	cout<<"traduction in smt2 clause\n";
	string namefile = to_string(g_pid)+"tlpgp2.smt2";
	ofstream file(namefile, ios::out | ios::trunc );
	DurativeAction * action;
	if (file){
		file << "(set-option :produce-models true)\n";
		assert = "(assert (and GoalsE0\n";
		string get_value="(get-value (";
		string name;
		int state =0;
		Vertex * actual = new Vertex(m_graph);
		while( actual->getFather() != NULL ){	
			actual = actual->getFather();
			for(unsigned j = 0 ; j < actual->getActions()->size() ; ++j){
				action = actual->getActions()->at(j);
				name=actual->getActions()->at(j)->getName()+"E"+to_string(state);
				for(unsigned i = 0 ; i < action->getParameters()->size() ; ++i){
					name+=action->getParameters()->at(i).getName();
				}
				file << "(declare-fun "<<name<<" () Bool )\n";
				get_value +=name+"\n";
				get_value +="t_"+name+"\n";
				file << "(declare-fun t_"<<name<<" () Int )\n";
				assert+="(>= t_"+name+" 0 )\n";

				for(unsigned i = 0 ; i < action->getPreconditions2().size() ; ++i){
					file << linkPrec(name,action->getPreconditions2().at(i).second,action->getPreconditions2().at(i).first,actual->getFather(),state+1);
				}
				for(unsigned i = 0 ; i < action->getEffects().size() ; ++i){
					//step 4.2
					protectEffect(name,action,action->getEffects().at(i).second,action->getEffects().at(i).first,state);
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

string Tlpgp2::linkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state){
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
						step4= "";
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
						}//step 4.1

						step4 = protectCond(link,fluent,att,namea,name);

						assert +=step3;
						assert +=step4;
						
					}
				}
			}
			s++;
		}
		
	}
	assert +=step2+")\n";
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
				}
			}
		}
	state++;
	}	
	return ret;
}

void Tlpgp2::protectEffect(string name,DurativeAction * a,Fluent * fluent,Attribute att,int statep){
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
						assert+="(or (not "+namea+") (not "+name+") ";
						assert+="(not (= (+ t_"+namea+" "+to_string(actual->getActions()->at(i)->getNotEffects().at(j).first.getSupported()->at(0).getEnd())+") (+ t_"+name+" "+to_string(att.getSupported()->at(0).getStart())+") ) ) )\n";
					}
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

