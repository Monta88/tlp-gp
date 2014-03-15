#pragma once


#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include <iostream> 
#include <string> 
#include <vector>
#include "variable.h"
#include "intervalle.h"
using namespace std;


class Attributs
{
public:
	Attributs();
	void add_supported(Intervalle inter);
	void add_forbiden(Intervalle inter);
	virtual ~Attributs();
private:
	vector<Intervalle> forbidens; 	//A dicuster
	vector<Intervalle> supporteds;	//
};
#endif // ATTRIBUT_H
