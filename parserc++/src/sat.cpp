/*!
 * \file sat.cpp
 * \brief contains what is needed to communicate with a sat solver, using smt2
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 20, 2014
 */

#include "sat.h"

Sat::Sat() {
	// TODO Auto-generated constructor stub
	m_solverPath = "./mathsat";
	ofstream m_smt2temp;
	FILE *m_smt2Cfile = nullptr;
	m_outputPath = "~output.txt";
	m_smt2tempPath = "~temp.smt2";
}

Sat::~Sat() {
	// TODO Auto-generated destructor stub
	m_smt2temp.close();
}

void Sat::initialize(){
	m_smt2temp.open("~temp.smt2", ios::out );

	if (m_smt2temp.is_open()){
		m_smt2temp << "(set-option :produce-models true)";
		m_smt2temp << "\n(declare-fun t_Init () Int)";
		m_smt2temp << "\n(declare-fun t_Goal () Int)";
		m_smt2temp << "\n(declare-fun  Init () Bool)";
		m_smt2temp << "\n(declare-fun  Goal () Bool)";

		//TODO remove for real declare-fun
		//m_smt2temp << "\n(declare-fun a () Bool )";
		m_smt2temp << "\n(declare-fun t_a () Int )";
		//m_smt2temp << "\n(declare-fun b () Bool )";
		m_smt2temp << "\n(declare-fun t_b () Int )";

	}
	else{
		cout<< "(sat) ERROR: can not open temp file" <<endl;
		exit(5);
	}
}

void Sat::addConstraint(Constraint *constraint){
	m_smt2temp <<  "\n(" << constraint->getComparison() << " (+ t_"
			<< constraint->getNameLeft() << " " << constraint->getTimeLeft() << ") (+ t_"
			<< constraint->getNameRight() << " " << constraint->getTimeRight() << "))" ;
}

/*
 * [τ s (A)+δ 1 , τ s (B)+δ 2 ] et
[τ s (C)+δ 3 , τ s (D)+δ 4 ] sur lesquels une proposition et sa négation sont respectivement vraies
peuvent être disjonctives. Dans le cas général, ces contraintes sont de la forme
(τ s (B)+δ 2 ≤ τ s (C)+δ 3 ) ∨ (τ s (D)+δ 4 ≤ τ s (A)+δ 1 ).
 *
 */
void Sat::addIntervalConstraint(IntervalAgenda interleft, IntervalAgenda interright){
	//m_smt2temp << "\n (or (" << interleft.getLeftAction()
}

void Sat::addDisjonctiveConstraint(Constraint *left, Constraint *right){
	m_smt2temp <<  "\n(or (" << left->getComparison() << " (+ t_"
				<< left->getNameLeft() << " " << left->getTimeLeft() << ") (+ t_"
				<< left->getNameRight() << " " << left->getTimeRight() << "))"
				<<  "(" << right->getComparison() << " (+ t_"
				<< right->getNameLeft() << " " << right->getTimeLeft() << ") (+ t_"
				<< right->getNameRight() << " " << right->getTimeRight() << ")))" ;
}

void Sat::addConstraints(vector<Constraint> *constraints){
	for(auto it_c = constraints->begin(); it_c != constraints->end(); ++it_c){
		addConstraint(&*it_c);
	}
}

void Sat::addFun(string name){
	//m_smt2temp << "(declare-fun "<<name<<" () Bool )\n";
	m_smt2temp << "(declare-fun t_"<<name<<" () Int )\n";
}

void Sat::postDeclareFun(){
	//m_smt2temp << "(assert (and Init Goal" ;
	m_smt2temp << "(assert (and (>= t_Goal t_Init)" ;
	m_smt2temp << "\n(>= t_Goal 0 )" ;
	//m_smt2temp << "\n(>= t_Goal t_Init)";
}

bool Sat::solve() {
	bool isSat=false;
	string resultLine = "";
	string control ="\n))(check-sat)(get-value (t_Goal t_Init))";

	// add the correct instructions to get an ouput from mathsat
	//m_smt2temp << "\n))(check-sat)" ; // check-sat before get-value !
	m_smt2temp << control ;
	m_smt2temp.flush(); // synchronize the file

	// convert iostream to a C FILE
	m_smt2Cfile = fopen(m_smt2tempPath, "r");
	if (m_smt2Cfile == NULL) {
		cerr << "smt2Cfile cannot be opened." << endl;
		exit(2);
	}

	// create a fork because it is the easyest way to capture the output of "exec"
	if (fork() == 0){
		// child

		// redirect stdout to a file, and stdin from a another file
		freopen (m_outputPath,"w",stdout);
		dup2(fileno(m_smt2Cfile), STDIN_FILENO);
		fclose(m_smt2Cfile);

		// execute mathsat
		execl(m_solverPath,nullptr);
	}
	else{
		//parent
		wait(NULL);
	}

	// close files
	//m_smt2temp.close();

	// read the first line of the output file which contains sat or unsat
	ifstream outputFile (m_outputPath);
	getline(outputFile,resultLine);
	isSat = resultLine=="sat";

	if(isSat)
		cout << "\nSAT" << endl;
	else
		cout << "\nNOT SAT" << endl;

	//we need to remove the "control" line because we will call sat.solve several times
	long pos = m_smt2temp.tellp();
	m_smt2temp.seekp(pos-control.length());
	m_smt2temp.write ("",0);

	return isSat;
}
