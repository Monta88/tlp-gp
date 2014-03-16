// Generated by Bisonc++ V4.08.00 on Sat, 15 Mar 2014 16:42:51 +0100

#ifndef Parser_h_included
#define Parser_h_included

// $insert baseclass
#include "Parserbase.h"
// $insert scanner.h
#include "../scanner/Scanner.h"
// $insert preincludes
#include "../headers.h"


#undef Parser
class Parser: public ParserBase
{
    // $insert scannerobject
    Scanner d_scanner;
        
    public:
        int parse();
        
        void init() {
        	this->m_data = new Data();
        }
        
        void add(std::string str) {
        	this->m_data->Add(str);
        }
        
        void display() {
        	this->m_data->Display();
        }

    private:
        void error(char const *msg);    // called on (syntax) errors
        int lex();                      // returns the next token from the
                                        // lexical scanner. 
        void print();                   // use, e.g., d_token, d_loc

    // support functions for parse():
        void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
        void print__();
        void exceptionHandler__(std::exception const &exc);

    // our stuff:
        Data * m_data;
};


#endif
