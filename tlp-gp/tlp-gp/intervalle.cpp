#include "intervalle.hpp"

Intervalle::Intervalle(float start,float end):start(start),end(end)
{
}

void Intervalle::add_argu(Variable * argu)
{
	arguments.push_back(argu);
}

Intervalle::~Intervalle()
{
}
