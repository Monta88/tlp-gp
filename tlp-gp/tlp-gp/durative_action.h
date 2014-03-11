#ifndef D_ACTION_H
#define D_ACTION_H
#include <iostream>
#include <string>
#include "action.h"
using namespace std;

class Duractive_Action:Action
{
    public:
        Duractive_Action(string name,float duration);        
        virtual ~Duractive_Action();
    protected:
	float duration;
    private:
};

#endif // ACTION_H
