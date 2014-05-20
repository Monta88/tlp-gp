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
	//m_smt2temp.close();
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

void Sat::addFun(string name){
	//m_smt2temp << "(declare-fun "<<name<<" () Bool )\n";
	m_smt2temp << "(declare-fun t_"<<name<<" () Int )\n";
}

void Sat::postDeclareFun(){
	m_smt2temp << "(assert (and Init Goal" ;
	m_smt2temp << "\n(>= t_Goal 0 )" ;
	m_smt2temp << "\n(>= t_Goal t_Init)";
}

bool Sat::solve() {
	bool isSat=false;
	string resultLine = "";

	// add the correct instructions to get an ouput from mathsat
	m_smt2temp << "))\n(check-sat)" ; // check-sat before get-value !
	m_smt2temp << "\n(get-value (t_Goal t_Init))" ;
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
	m_smt2temp.close();

	// read the first line of the output file which contains sat or unsat
	ifstream outputFile (m_outputPath);
	getline(outputFile,resultLine);
	isSat = resultLine=="sat";

	if(isSat)
		cout << "\nSAT" << endl;
	else
		cout << "\nNOT SAT" << endl;

	return isSat;
}
