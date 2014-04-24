/*
 * sat.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: nathan
 */

#include "sat.h"

Sat::Sat() {
	// TODO Auto-generated constructor stub
	m_solverPath = "./mathsat";
	ofstream m_smt2temp();
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
		m_smt2temp << "(declare-fun t_Init () Int)";
		m_smt2temp << "\n(declare-fun t_Goal () Int)";

		// TODO Add the real constraints
		//cout << m_testString << endl;
		m_smt2temp << m_testString ;

	}
	else{
		cout<< "(sat) ERROR: can not open temp file" <<endl;
		exit(5);
	}
}

bool Sat::solve() {
	bool isSat=false;
	string resultLine = "";

	// add the correct instructions to get an ouput from mathsat
	m_smt2temp << "(check-sat)" ; // check-sat before get-value !
	m_smt2temp << "(get-value (t_Goal t_Init))" ;
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
		wait(null);
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
