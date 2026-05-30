#pragma once
#include <vector>
#include "Grafo.h"
#include <queue>
using namespace std;


class BFSCDC
{
public:
    static int BFSComponenteConexa(int NodoInicial, vector<bool>& NodoVisitado, Grafo& grafoNoDirigido, vector<int>& nodosComponente);
};

