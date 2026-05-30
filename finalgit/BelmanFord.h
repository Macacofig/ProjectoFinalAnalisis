#pragma once
#include <vector>
#include <limits>
#include "Grafo.h"
class BelmanFord
{
public:
    static int AlcanceVehicular(Grafo& grafo, int origen, double limiteMetros);
};

