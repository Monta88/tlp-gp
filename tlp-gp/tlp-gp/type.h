#ifndef TYPE_H
#define TYPE_H
#include <iostream>
#include <string>
using namespace std;

class Type
{
    public:
        Type(string name);        
	string getType();
        virtual ~Type();
    protected:
        string name;
    private:
};

#endif // TYPE_H
