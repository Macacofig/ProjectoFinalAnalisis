#pragma once

#include "Grafo.h"
#include <queue>
#include <vector>
class PrimMST
{
public:

    static double ConstruirMST(Grafo& grafo, vector<bool>& nodosComponenteGigante);
};
