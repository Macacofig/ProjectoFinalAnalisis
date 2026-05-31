#pragma once
#include "Grafo.h"
#include <vector>
#include <queue>
#include <limits>
class Reachability
{
private:

public:
    static int AlcanceVehicular(int nodoOrigen, Grafo& grafo, double limiteMetros);
    static vector<double> DiametroVial(int nodoOrigen, Grafo& grafo);
    static pair<double,int> DiametroVialDoubleSweep(int nodoOrigen,Grafo& grafo);
};
