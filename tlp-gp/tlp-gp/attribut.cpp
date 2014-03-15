#include "attribut.h"

Attributs::Attributs()
{
}

void Attributs::add_supported(Intervalle inter)
{
	supporteds.push_back(inter);
}
void Attributs::add_forbiden(Intervalle inter)
{
	forbidens.push_back(inter);
}

Attributs::~Attributs()
{
}
