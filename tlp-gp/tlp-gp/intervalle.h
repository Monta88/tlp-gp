#ifndef INTERVALLE_H
#define INTERVALLE_H
#include <iostream>
#include <string>
#include <vector>
#include "variable.h"

//Si le fluent est produit au moment a alor start=a et end=a
using namespace std;

class Intervalle
{
    public:
        Intervalle(float start,float end);     
	void add_argu(Variable const * argu);
        virtual ~Intervalle();
    protected:
        float start;
	float end;
	vector <Variable*> arguments;
    private:
};

#endif // INTERVALLE_H
