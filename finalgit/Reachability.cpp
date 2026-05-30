#include "Reachability.h"

int Reachability::AlcanceVehicular(int nodoOrigen, Grafo& grafo, double limiteMetros)
{
    int n = grafo.GetTotalNodos();

    // DISTANCIAS MINIMAS desde el nodo origen al destino

    vector<double> distancia(n,numeric_limits<double>::infinity()); 

    distancia[nodoOrigen] = 0;

    /*

    PRIORITY QUEUE // logV insertar, eliminar

    (distancia acumulada, nodo)

    siempre sale primero el menor

    usara un vector

    greater hace que sea min heap
    */

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    pq.push({ 0, nodoOrigen });

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    while (!pq.empty())
    {
        double distanciaActual = pq.top().first;
        int nodoActual = pq.top().second;

        pq.pop();

        // SI YA PASAMOS EL LIMITE NO SEGUIMOS

        if (distanciaActual > limiteMetros)
            continue;

        // RECORRER VECINOS

        for (Arista& arista : lista[nodoActual])
        {
            int vecino = arista.GetNodoDestino();

            double nuevaDistancia = distanciaActual + arista.GetDistanciaMestros();

            // RELAJACION

            //encontre un mejor camino Y la distancia encontrada esta dentro del limite

            if (nuevaDistancia < distancia[vecino] && nuevaDistancia <= limiteMetros)
            {
                distancia[vecino] = nuevaDistancia;

                pq.push({ nuevaDistancia, vecino});
            }
        }
    }

    /*

    CONTAR NODOS ALCANZABLES

    algunos nodos quedan con inf

    */

    int alcanzables = 0;

    for (double d : distancia)
    {
        if (d <= limiteMetros)
            alcanzables++;
    }

    return alcanzables;
}
