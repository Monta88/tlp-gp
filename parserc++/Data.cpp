#include "Data.h"

#include <algorithm>
#include <iostream>

Data::Data() {
	m_domain = Domain();
	m_requirements = vector<int> ();
	m_type_list = vector<string> ();
	m_constant_list = vector<string> ();
	m_predicate_list = vector<string> ();
	m_types = vector<Type*> ();
	m_constants = vector<Constant*> ();
	m_predicates = vector<Fluent*> ();
	m_types.push_back(new Type("object"));
	m_type_list.push_back("object");
}

Data::~Data() {}

void Data::addDomain(string * name) {
	m_domain = Domain(*name);
}

bool Data::addRequirement(int req) {
	if (isRequirement(req)) {
		return false;
	}
	m_requirements.push_back(req);
	return true;
}

bool Data::isRequirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

bool Data::addTypes(std::vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> parents = vector<Type*> ();
		for (vector<string>::reverse_iterator it_parent = (*it)->getTypes()->rbegin(); it_parent != (*it)->getTypes()->rend(); ++it_parent) {
			if (!isType(*it_parent)) {
				// ERROR : one of the parent types does not exist
				return false;
			}
			parents.push_back(getType(*it_parent));
		}
		for (vector<string>::reverse_iterator it_type = (*it)->getList()->rbegin(); it_type != (*it)->getList()->rend(); ++it_type) {
			if (isType(*it_type)) {
				// ERROR : the type already exists
				return false;
			}
			m_type_list.push_back(*it_type);
			m_types.push_back(new Type(*it_type, parents));
		}
	}
	return true;
}

bool Data::isType(string type) {
	return (find(m_type_list.begin(), m_type_list.end(), type) != m_type_list.end());
}

Type * Data::getType(string name) {
	for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
		if ((*it)->getName() == name) {
			return (*it);
		}
	}
	// CRITICAL ! should not happen
	// ERROR
	return NULL;
}

bool Data::addConstants(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				// ERROR : one of the types does not exist
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_constant = (*it)->getList()->rbegin(); it_constant != (*it)->getList()->rend(); ++it_constant) {
			if (isConstant(*it_constant)) {
				// ERROR : the constant already exists
				return false;
			}
			m_constant_list.push_back(*it_constant);
			m_constants.push_back(new Constant(*it_constant, types));
		}
	}
	return true;
}

bool Data::isConstant(string constant) {
	return (find(m_constant_list.begin(), m_constant_list.end(), constant) != m_constant_list.end());
}

bool Data::addPredicate(std::string * name, std::vector<TypedList*> * typedList_list) {
	// the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file
	if (isPredicate(*name)) {
		// ERROR : the predicate already exists
		return false;
	}
	vector< vector<Type*> > types_list = vector< vector<Type*> > ();
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				// ERROR : one of the types does not exist
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	m_predicates.push_back(new Fluent(*name, types_list));
	return true;
}

bool Data::isPredicate(string predicate) {
	return (find(m_predicate_list.begin(), m_predicate_list.end(), predicate) != m_predicate_list.end());
}

void Data::display() {
	cout << "Domain name : " << m_domain.getName() << endl;
	
	cout << "Types : " << endl;
	for(vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
	
	cout << "Constants : " << endl;
	for(vector<Constant*>::iterator it = m_constants.begin(); it != m_constants.end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
	
	cout << "Predicates : " << endl;
	for(vector<Fluent*>::iterator it = m_predicates.begin(); it != m_predicates.end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
}

