/*
 * sat.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: nathan
 */

#include "sat.h"

sat::sat() {
	// TODO Auto-generated constructor stub


}

sat::~sat() {
	// TODO Auto-generated destructor stub
	m_file.close();
}

void sat::initialize(){
	m_file.open("~temp.smt2", ios::out );

	if (m_file.is_open()){
		m_file << "(set-option :produce-models true)";
		m_file << "(declare-fun t_Init () Int)";
		m_file << "\n(declare-fun t_Goal () Int)";

		cout << m_testString << endl;
		m_file << m_testString ;

		m_file << "(check-sat)" ;
		m_file << "(get-value (t_Goal t_Init))" ;

		m_file.close();
	}
	else{
		cout<< "(sat) ERROR: can not open temp file" <<endl;
		exit(5);
	}
}

bool sat::solve() {
	//exec solver

	return true;
}
