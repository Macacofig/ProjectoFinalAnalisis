#include "KruskalMST.h"

double KruskalMST::ConstruirMST(Grafo& grafo, vector<bool>& nodosComponenteGigante, int totalNodosGigante)
{
    /*

        LISTA DE ARISTAS

        Kruskal trabaja mejor
        sobre una lista única de aristas
        porque necesita ordenarlas globalmente
    */

    vector<EdgeMST> edges;

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    int TotalNodos = grafo.GetTotalNodos();

    /*
        EXTRAER ARISTAS -> O(E)

        Solo usamos aristas cuyos nodos
        pertenezcan a la componente gigante
    */

    for (int NodoActual = 0; NodoActual < TotalNodos; NodoActual++)
    {
        // ignorar nodos fuera de la componente gigante

        if (!nodosComponenteGigante[NodoActual])
            continue;

        for (Arista& arista : lista[NodoActual])
        {
            int NodoDestino = arista.GetNodoDestino();

            // ignorar si el destino no pertenece al componente gigante

            if (!nodosComponenteGigante[NodoDestino])
                continue;

            /*
                EVITAR DUPLICADOS

                Como el grafo es no dirigido:

                Nodo1 puede ir a Nodo2
                Nodo2 puede ir a Nodo1

                solo guardamos uno
            */

            if (NodoActual < NodoDestino)
            {
                double peso = arista.GetDistanciaMestros();

                edges.push_back(EdgeMST{ NodoActual, NodoDestino, peso });
            }
        }
    }

    /*

        ORDENAR ARISTAS POR PESO -> O(ElogE)

        Kruskal siempre toma primero
        la arista más barata

    */

    sort( edges.begin(),edges.end(),
        [](EdgeMST& a, EdgeMST& b) 
        // No hay una funcion declarada en el codigo que haga la comparacion, [] permite no escribir esa funcion pero si poner la logica
        {
            return a.peso < b.peso;
        }
    );

    /*

        DSU O(E) -> Trabajo solo con aristas

        Nos ayuda a detectar ciclos

    */

    DSU dsu(TotalNodos);

    double distanciaTotalMetros = 0;

    int aristasUsadas = 0;

    /*

        RECORRER ARISTAS ORDENADAS

    */

    for (EdgeMST& edge : edges)
    {
        int NodoOrigen = edge.from;

        int NodoDestino = edge.to;

        double peso = edge.peso;

        /*

            SI TIENEN DISTINTA RAÍZ
            NO FORMAN CICLO

            find(NodoOrigen):
            devuelve la raíz del componente de NodoOrigen

            find(NodoDestino):
            devuelve la raíz del componente de NodoDestino

            Si tienen distinta raíz:
            NO están conectados todavía
        */

        if (dsu.find(NodoOrigen) != dsu.find(NodoDestino))
        {
            // unir componentes

            dsu.unite(NodoOrigen, NodoDestino);

            // agregar al MST (agregamos la distancia)

            distanciaTotalMetros += peso;

            aristasUsadas++;

            // MST tiene Vertices - 1, si llegamos a eso salimos del bucle
            if (aristasUsadas == totalNodosGigante - 1)
            {
                break;
            }
        }
    }



    return distanciaTotalMetros;
}