#pragma once
#include "Grafo.h"
#include <algorithm>
#include <math.h>
#include "DSU.h"

class KruskalMST
{
private:

    struct EdgeMST
    {
        int from;
        int to;
        double peso;
    };
public:
    static double ConstruirMST(Grafo& grafo, vector<bool>& nodosComponenteGigante, int totalNodosGigante);
};

