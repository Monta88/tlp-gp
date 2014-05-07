/*!
 * \file sat.h
 * \brief contains what is needed to communicate with a sat solver, using smt2
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 20, 2014
 */

#ifndef SAT_H_
#define SAT_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using namespace std;

class Sat {
public:
	Sat();
	virtual ~Sat();
	void initialize();

	bool solve();
private:
	const char* m_solverPath;
	ofstream m_smt2temp;
	FILE *m_smt2Cfile;
	const char* m_outputPath;
	const char* m_smt2tempPath;

	string m_testString = "(declare-fun t_A1 () Int)"
"(declare-fun t_A2 () Int)"
"(declare-fun t_A3 () Int)"
"(declare-fun t_B2 () Int)"
"(declare-fun t_B3 () Int)"
"(declare-fun t_C3 () Int)"
"(declare-fun t_Init () Int)"
"(declare-fun t_Goal () Int)"
"(declare-fun t_A1 () Int)"
"(declare-fun t_A2 () Int)"
"(declare-fun t_A3 () Int)"
"(declare-fun t_B2 () Int)"
"(declare-fun t_B3 () Int)"
"(declare-fun t_C3 () Int)"
"(declare-fun  Init () Bool)"
"(declare-fun  Goal () Bool)"
"(declare-fun  A1 () Bool)"
"(declare-fun  A2 () Bool)"
"(declare-fun  A3 () Bool)"
"(declare-fun  B2 () Bool)"
"(declare-fun  B3 () Bool)"
"(declare-fun  C3 () Bool)"
"(declare-fun  Link_A1.a.B2 () Bool)"
"(declare-fun  Link_A1.a.B3 () Bool)"
"(declare-fun  Link_A2.a.B3 () Bool)"
"(declare-fun  Link_B2.c.C3 () Bool)"
"(declare-fun  Link_A1.b.Goal () Bool)"
"(declare-fun  Link_A2.b.Goal () Bool)"
"(declare-fun  Link_A3.b.Goal () Bool)"
"(declare-fun  Link_B2.d.Goal () Bool)"
"(declare-fun  Link_B3.d.Goal () Bool)"
"(declare-fun  Link_C3.e.Goal () Bool)"
"(assert (and"
" Init Goal"
" (or  (not B2) Link_A1.a.B2)"
" (or  (not B3) Link_A1.a.B3 Link_A2.a.B3)"
" (or  (not C3) Link_B2.c.C3)"
" (or  (not Goal) Link_A1.b.Goal Link_A2.b.Goal Link_A3.b.Goal)"
" (or  (not Goal) Link_B2.d.Goal Link_B3.d.Goal)"
" (or  (not Goal) Link_C3.e.Goal)"
" (or  (not Link_A1.a.B2) A1)"
" (or  (not Link_A1.a.B2) B2)"
" (or  (not Link_A1.a.B2) ( >= (- t_B2 t_A1) 0))"
" (or  (not Link_A1.a.B3) A1)"
" (or  (not Link_A1.a.B3) B3)"
" (or  (not Link_A1.a.B3) ( >= (- t_B3 t_A1) 0))"
" (or  (not Link_A2.a.B3) A2)"
" (or  (not Link_A2.a.B3) B3)"
" (or  (not Link_A2.a.B3) ( >= (- t_B3 t_A2) 0))"
" (or  (not Link_B2.c.C3) B2)"
" (or  (not Link_B2.c.C3) C3)"
" (or  (not Link_B2.c.C3) ( >= (- t_C3 t_B2) 0))"
" (or  (not Link_A1.b.Goal) A1)"
" (or  (not Link_A1.b.Goal) Goal)"
" (or  (not Link_A1.b.Goal) ( >= (- t_Goal t_A1) 5))"
" (or  (not Link_A2.b.Goal) A2)"
" (or  (not Link_A2.b.Goal) Goal)"
" (or  (not Link_A2.b.Goal) ( >= (- t_Goal t_A2) 5))"
" (or  (not Link_A3.b.Goal) A3)"
" (or  (not Link_A3.b.Goal) Goal)"
" (or  (not Link_A3.b.Goal) ( >= (- t_Goal t_A3) 5))"
" (or  (not Link_B2.d.Goal) B2)"
" (or  (not Link_B2.d.Goal) Goal)"
" (or  (not Link_B2.d.Goal) ( >= (- t_Goal t_B2) 4))"
" (or  (not Link_B3.d.Goal) B3)"
" (or  (not Link_B3.d.Goal) Goal)"
" (or  (not Link_B3.d.Goal) ( >= (- t_Goal t_B3) 4))"
" (or  (not Link_C3.e.Goal) C3)"
" (or  (not Link_C3.e.Goal) Goal)"
" (or  (not Link_C3.e.Goal) ( >= (- t_Goal t_C3) 1))"
" (or  (not Link_A1.a.B2) (not A1) (> (- t_A1 t_A1) 5) (< (- t_B2 t_A1) 5))"
" (or  (not Link_A1.a.B2) (not A2) (> (- t_A1 t_A2) 5) (< (- t_B2 t_A2) 5))"
" (or  (not Link_A1.a.B2) (not A3) (> (- t_A1 t_A3) 5) (< (- t_B2 t_A3) 5))"
" (or  (not Link_A1.a.B3) (not A1) (> (- t_A1 t_A1) 5) (< (- t_B3 t_A1) 5))"
" (or  (not Link_A1.a.B3) (not A2) (> (- t_A1 t_A2) 5) (< (- t_B3 t_A2) 5))"
" (or  (not Link_A1.a.B3) (not A3) (> (- t_A1 t_A3) 5) (< (- t_B3 t_A3) 5))"
" (or  (not Link_A2.a.B3) (not A1) (> (- t_A2 t_A1) 5) (< (- t_B3 t_A1) 5))"
" (or  (not Link_A2.a.B3) (not A2) (> (- t_A2 t_A2) 5) (< (- t_B3 t_A2) 5))"
" (or  (not Link_A2.a.B3) (not A3) (> (- t_A2 t_A3) 5) (< (- t_B3 t_A3) 5))"
" (or  (not Link_B2.c.C3) (not B2) (> (- t_B2 t_B2) 4) (< (- t_C3 t_B2) 4))"
" (or  (not Link_B2.c.C3) (not B3) (> (- t_B2 t_B3) 4) (< (- t_C3 t_B3) 4))"
" (or  (not Link_B2.d.Goal) (not A1) (> (- t_B2 t_A1) 1) (< (- t_Goal t_A1) 5))"
" (or  (not Link_B2.d.Goal) (not A2) (> (- t_B2 t_A2) 1) (< (- t_Goal t_A2) 5))"
" (or  (not Link_B2.d.Goal) (not A3) (> (- t_B2 t_A3) 1) (< (- t_Goal t_A3) 5))"
" (or  (not Link_B3.d.Goal) (not A1) (> (- t_B3 t_A1) 1) (< (- t_Goal t_A1) 5))"
" (or  (not Link_B3.d.Goal) (not A2) (> (- t_B3 t_A2) 1) (< (- t_Goal t_A2) 5))"
" (or  (not Link_B3.d.Goal) (not A3) (> (- t_B3 t_A3) 1) (< (- t_Goal t_A3) 5))"
" (or  (not A1) (not A2) (distinct (- t_A1 t_A2) 5))"
" (or  (not A1) (not A3) (distinct (- t_A1 t_A3) 5))"
" (or  (not A1) (not C3) (distinct (- t_C3 t_A1) 4))"
" (or  (not A2) (not A1) (distinct (- t_A2 t_A1) 5))"
" (or  (not A2) (not A3) (distinct (- t_A2 t_A3) 5))"
" (or  (not A2) (not C3) (distinct (- t_C3 t_A2) 4))"
" (or  (not A3) (not A1) (distinct (- t_A3 t_A1) 5))"
" (or  (not A3) (not A2) (distinct (- t_A3 t_A2) 5))"
" (or  (not A3) (not C3) (distinct (- t_C3 t_A3) 4))"
" (or  (not B2) (not B3) (distinct (- t_B2 t_B3) 4))"
" (or  (not B3) (not B2) (distinct (- t_B3 t_B2) 4))"
" (or  (not B2) (not A1) (distinct (- t_B2 t_A1) 1))"
" (or  (not B2) (not A2) (distinct (- t_B2 t_A2) 1))"
" (or  (not B2) (not A3) (distinct (- t_B2 t_A3) 1))"
" (or  (not B3) (not A1) (distinct (- t_B3 t_A1) 1))"
" (or  (not B3) (not A2) (distinct (- t_B3 t_A2) 1))"
" (or  (not B3) (not A3) (distinct (- t_B3 t_A3) 1))"
" (>= t_Goal t_Init)"
" (>= t_A1 t_Init)"
" (>= t_A2 t_Init)"
" (>= t_A3 t_Init)"
" (>= t_B2 t_Init)"
" (>= t_B3 t_Init)"
" (>= t_C3 t_Init)"
" (>= (- t_Goal t_A1) 5)"
" (>= (- t_Goal t_A2) 5)"
" (>= (- t_Goal t_A3) 5)"
" (>= (- t_Goal t_B2) 4)"
" (>= (- t_Goal t_B3) 4)"
" (>= (- t_Goal t_C3) 1)"
"))" ;
};

#endif /* SAT_H_ */
