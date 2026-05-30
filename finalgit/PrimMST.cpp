#include "PrimMST.h"

double PrimMST::ConstruirMST(Grafo& grafo, vector<bool>& nodosComponenteGigante)
{
    int n = grafo.GetTotalNodos();

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    /*
        visited[i]
        indica si el nodo ya pertenece al MST
    */

    vector<bool> visitado(n, false);

    /*
        priority queue

        (peso, nodo)

        min heap
    */

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    /*
        buscar cualquier nodo
        de la componente gigante
    */

    int inicio = -1;

    for (int i = 0; i < n; i++)
    {
        if (nodosComponenteGigante[i])
        {
            inicio = i;
            break;
        }
    }

    /*
        arrancar Prim
    */
    // distancia , nodo
    pq.push({ 0, inicio });

    double distanciaTotal = 0;

    while (!pq.empty())
    {
        double distancia = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        /*
            si ya estaba dentro del MST
            ignoramos
        */

        if (visitado[u])
            continue;

        visitado[u] = true;

        /*
            agregar distancia al MST
        */

        distanciaTotal += distancia;

        /*
            explorar vecinos
        */

        for (Arista& arista : lista[u]) // E -> Cada arista
        {
            int NodoDestino = arista.GetNodoDestino();

            double distanciaVecino = arista.GetDistanciaMestros();

            if (!nodosComponenteGigante[NodoDestino])
                continue;

            if (!visitado[NodoDestino])
            {
                pq.push({ distanciaVecino, NodoDestino }); // O(log V) -> cantidad vertices : V altura
            }
        }
    }

    return distanciaTotal;
}