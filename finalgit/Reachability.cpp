#include "Reachability.h"

int Reachability::AlcanceVehicular(int nodoOrigen, Grafo& grafo, double limiteMetros)
{
    int n = grafo.GetTotalNodos();

    // DISTANCIAS MINIMAS

    vector<double> distancia(
        n,
        numeric_limits<double>::infinity()
    );

    distancia[nodoOrigen] = 0;

    /*

    PRIORITY QUEUE

    (distancia acumulada, nodo)

    siempre sale primero el menor

    */

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    pq.push({ 0, nodoOrigen });

    vector<vector<Arista>>& lista =
        grafo.GetListaAdyacenciasOriginal();

    while (!pq.empty())
    {
        double distanciaActual = pq.top().first;
        int nodoActual = pq.top().second;

        pq.pop();

        /*

        SI YA PASAMOS EL LIMITE
        NO SEGUIMOS

        */

        if (distanciaActual > limiteMetros)
            continue;

        /*

        RECORRER VECINOS

        */

        for (Arista& arista : lista[nodoActual])
        {
            int vecino =
                arista.GetNodoDestino();

            double nuevaDistancia =
                distanciaActual +
                arista.GetDistanciaMestros();

            /*

            RELAJACION

            */

            if (nuevaDistancia < distancia[vecino] && nuevaDistancia <= limiteMetros)
            {
                distancia[vecino] = nuevaDistancia;

                pq.push({ nuevaDistancia, vecino});
            }
        }
    }

    /*

    CONTAR NODOS ALCANZABLES

    */

    int alcanzables = 0;

    for (double d : distancia)
    {
        if (d <= limiteMetros)
            alcanzables++;
    }

    return alcanzables;
}
