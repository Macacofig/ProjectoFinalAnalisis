#include "ComparacionRuta.h"

double ComparacionRuta::DijkstraPeso(Grafo& grafo, int origen, int destino, TipoPeso criterio)
{
    int n = grafo.GetTotalNodos();

    const double INF = numeric_limits<double>::infinity();

    vector<double> dist(n, INF);

    dist[origen] = 0;

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    pq.push({ 0, origen });

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    while (!pq.empty())
    {
        double distanciaActual = pq.top().first;

        int nodoActual = pq.top().second;

        pq.pop();

        if (distanciaActual > dist[nodoActual])
            continue;

        /*
            OPTIMIZACIÓN

            Si llegamos al destino ya
            encontramos el mejor costo.
        */

        if (nodoActual == destino)
            break;

        for (Arista& arista : lista[nodoActual])
        {
            int vecino = arista.GetNodoDestino();

            double peso;

            // Elegir criterio

            if (criterio == DISTANCIA)
            {
                
                peso = arista.GetDistanciaMestros();
            }
            else
            {
                if (arista.GetVelocidadMaxima() <= 0)
                    continue;
                /*
                    tiempo(segundos)

                    distancia(m) /
                    velocidad(m/s)

                    velocidad(km/h)/3.6
                */
                double velocidadMS = static_cast<double>(arista.GetVelocidadMaxima()) / 3.6;

                peso = arista.GetDistanciaMestros() / velocidadMS;
            }

            double nuevaDist = dist[nodoActual] + peso;

            if (nuevaDist < dist[vecino])
            {
                dist[vecino] = nuevaDist;

                pq.push({ nuevaDist, vecino});
            }
        }
    }
    return dist[destino];
}
