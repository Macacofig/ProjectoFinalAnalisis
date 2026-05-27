#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Arista
{
private:
    int NodoDestino;
    double DistanciaMetros;
    double VelocidadMaxima;
public:
    Arista(int NodoDestino, double DistanciaMetros, double VelocidadMaxima);
    
    int GetNodoDestino();
    double GetDistanciaMestros();
    double GetVelocidadMaxima();
};

