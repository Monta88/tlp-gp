/*
 * Data.h
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#ifndef DATA_H_
#define DATA_H_

namespace std {

class Data {
public:
	Data();
	virtual ~Data();
	void Add(string str);
private:
	vector<string> data ;
};

} /* namespace std */

#endif /* DATA_H_ */
