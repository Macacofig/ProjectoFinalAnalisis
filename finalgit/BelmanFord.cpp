#include "BelmanFord.h"

int BelmanFord::AlcanceVehicular(Grafo& grafo, int origen, double limiteMetros)
    {
        int n = grafo.GetTotalNodos();

        // INFINITO

        const double INF = numeric_limits<double>::infinity();

        /*

        DISTANCIAS

        dist[i] =
        mejor distancia conocida
        desde origen hasta i

        */

        vector<double> dist(n, INF);

        dist[origen] = 0;

        /*

        RELAJACION

        Repetimos V-1 veces:
        intentamos mejorar las distancias

        */
        vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();
        for (int i = 0; i < n - 1 ; i++)
        {
            bool huboCambio = false;

            for (int u = 0; u < n; u++) // todos los nodos
            {
                for (Arista& arista : lista[u]) // aristas de cada nodo
                {
                    int v = arista.GetNodoDestino();

                    double peso = arista.GetDistanciaMestros();

                    // Si no tiene ruta conocida no lo usamos INF + peso (error)

                    if (dist[u] == INF)
                        continue;

                    double nuevaDist = dist[u] + peso;

                    /*

                    RELAJAR ARISTA

                    Si encontramos un camino más corto actualizamos

                    */

                    if (nuevaDist < dist[v] && nuevaDist <= limiteMetros)
                    {
                        dist[v] = nuevaDist;

                        huboCambio = true;
                    }
                }
            }
            //Si en una pasada no hubo cambios, ya terminamos

            if (!huboCambio)
                break;
        }

        /*

        CONTAR NODOS ALCANZABLES

        */

        int alcanzables = 0;

        for (double d : dist)
        {
            if (d <= limiteMetros)
            {
                alcanzables++;
            }
        }

        return alcanzables;
    }
