#include "Arista.h"

Arista::Arista(int NodoDestino, double DistanciaMetros, double VelocidadMaxima)
{
	this->NodoDestino = NodoDestino;
	this->DistanciaMetros = DistanciaMetros;
	this->VelocidadMaxima = VelocidadMaxima;
}

int Arista::GetNodoDestino()
{
	return NodoDestino;
}

double Arista::GetDistanciaMestros()
{
	return DistanciaMetros;
}

double Arista::GetVelocidadMaxima()
{
	return VelocidadMaxima;
}
