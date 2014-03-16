#include "attribute.h"


Attribute::Attribute() {}

Attribute::~Attribute() {}

void Attribute::add_supported(Interval inter) {
	m_supporteds.push_back(inter);
}

void Attribute::add_forbiden(Interval inter) {
	m_forbidens.push_back(inter);
}

