#pragma once
#include "Grafo.h"
#include <vector>
#include <queue>
#include <limits>
class Reachability
{
public:
    static int AlcanceVehicular(int nodoOrigen, Grafo& grafo, double limiteMetros);
};
