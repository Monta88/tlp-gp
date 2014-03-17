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

	void add_Domain(std::string str) {
		this->m_data->add_Domain(str);
	}

	bool add_Requirement(int req) {
		return this->m_data->add_Requirement(req);
	}

	bool is_Requirement(int req) {
		return this->m_data->is_Requirement(req);
	}
	
	bool add_Constants(std::vector<NamedList*> * named_list) {
		return this->m_data->add_Constants(named_list);
	}
	
	bool add_Predicate(std::string * name, std::vector<NamedList*> * named_list) {
		return this->m_data->add_Predicate(name, named_list);
	}
	
	void display() {
		this->m_data->display();
	}

	void lexical_error(std::string msg) {
		std::cerr << msg << std::endl;
	}
	
	std::string to_string(std::vector<NamedList*> * named_list) {
		std::string str = "NamedList :\n";
		for (std::vector<NamedList*>::iterator it = named_list->begin(); it != named_list->end(); ++it)
			str += (*it)->to_string() + "\n";
		return str;
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