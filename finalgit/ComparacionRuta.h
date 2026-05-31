#pragma once
#include "Grafo.h"
#include <queue>
class ComparacionRuta
{

public:
    enum TipoPeso
    {
        DISTANCIA,
        TIEMPO
    };
    static double DijkstraPeso( Grafo& grafo, int origen, int destino, TipoPeso criterio);
};

