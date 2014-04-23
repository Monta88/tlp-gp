/*
 * sat.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: nathan
 */

#include "sat.h"

Sat::Sat() {
	// TODO Auto-generated constructor stub
	FILE *m_smt2file = nullptr;
	string solver = "";
	ofstream m_file;
	string m_smt2String ="";
}

Sat::~Sat() {
	// TODO Auto-generated destructor stub
	m_file.close();
}

void Sat::initialize(){
	m_file.open("~temp.smt2", ios::out );

	if (m_file.is_open()){
		m_file << "(set-option :produce-models true)";
		m_file << "(declare-fun t_Init () Int)";
		m_file << "\n(declare-fun t_Goal () Int)";

		// TODO Add the real constraints
		//cout << m_testString << endl;
		m_file << m_testString ;

		m_smt2String += "(declare-fun t_Init () Int)";
		m_smt2String += "(declare-fun t_Goal () Int)";
		m_smt2String += "(set-option :produce-models true)";
		m_smt2String += m_testString;

	}
	else{
		cout<< "(sat) ERROR: can not open temp file" <<endl;
		exit(5);
	}
}

bool Sat::solve() {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	// add the correct instructions to get an ouput from mathsat
	m_file << "(check-sat)" ;
	m_file << "(get-value (t_Goal t_Init))" ;
	m_file.flush(); // synchronize the file

	m_smt2String += "(check-sat)";
	m_smt2String += "(get-value (t_Goal t_Init))";
	//cout << m_smt2String << endl;

	// convert iostream to a C FILE
	m_smt2file = fopen("~temp.smt2", "r");
	if (m_smt2file == NULL) {
		cerr << " cannot be opened." << endl;
		exit(2);
	}

	// create a fork because it is the easyest way to capture the output of "exec"
	if (fork() == 0){
	    // child

		// redirect stdout to a file, and stdin from a another file
		freopen ("~output.txt","w",stdout);
		dup2(fileno(m_smt2file), STDIN_FILENO);
		fclose(m_smt2file);

		// execute mathsat
		execl("./mathsat",nullptr);
	}
	else{
		//parent
		wait(null);
	}

	// close all
	m_file.close();
	fclose (stdout);

	// read the first line of the output file which contains sat or unsat

	return true;
}
