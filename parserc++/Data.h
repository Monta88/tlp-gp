/*
 * Data.h
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void Add(std::string str);
	void Display();
private:
	std::vector<string> m_data ;
};

#endif /* DATA_H_ */
