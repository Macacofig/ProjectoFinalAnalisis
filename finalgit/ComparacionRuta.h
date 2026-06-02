#pragma once
#include "Grafo.h"
#include <queue>
#include <algorithm>
class ComparacionRuta
{
public:
    struct ResultadoRuta
    {
        double costo;

        vector<int> camino;

        int cantidadNodos;

        double dist=0;
    };
    enum TipoPeso
    {
        DISTANCIA,
        TIEMPO
    };
    static ResultadoRuta DijkstraPeso( Grafo& grafo, int origen, int destino, TipoPeso criterio);
};

