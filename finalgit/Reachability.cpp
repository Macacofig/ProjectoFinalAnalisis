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

vector<double> Reachability::DiametroVial(int nodoOrigen, Grafo& grafo)
{
    int n = grafo.GetTotalNodos();

    const double INF = numeric_limits<double>::infinity();

    // Valor utilizado para representar nodos aún no alcanzados
    vector<double> dist(n, INF);
    
    /*
        dist[i]

        Distancia mínima conocida desde
        nodoOrigen hasta i.
    */
    dist[nodoOrigen] = 0;

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    pq.push({ 0, nodoOrigen });

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    /*
        ALGORITMO DE DIJKSTRA

        Mientras existan nodos pendientes
        en la cola de prioridad.
    */
    while (!pq.empty())
    {
        // Obtener el distancia hacia el nodo y el nodo
        double distanciaActual = pq.top().first;

        int nodoActual = pq.top().second;

        pq.pop();

        /*
           Si ya existe una distancia mejor
           registrada para este nodo,
           ignoramos esta entrada.

           Esto evita procesar caminos obsoletos.
       */
        if (distanciaActual > dist[nodoActual])
            continue;

        /*
            Explorar todos los vecinos
            del nodo actual.
        */
        for (Arista& arista : lista[nodoActual])
        {
            int vecino = arista.GetNodoDestino();

            double peso = arista.GetDistanciaMestros();

            double nuevaDist = dist[nodoActual] + peso;

            /*
                Si encontramos un camino más corto,
                actualizamos la distancia y lo
                insertamos nuevamente en la cola.
            */
            if (nuevaDist < dist[vecino])
            {
                dist[vecino] = nuevaDist;
                pq.push({nuevaDist,vecino});
            }
        }
    }

    return dist;
}

pair<double, int> Reachability::DiametroVialDoubleSweep(int nodoOrigen, Grafo& grafo)
{
    int n = grafo.GetTotalNodos();

    const double INF = numeric_limits<double>::infinity();

    // iniciar todos las distancias en infinito 
    vector<double> dist(n, INF);

    dist[nodoOrigen] = 0;

    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    //agregamos el primer nodo
    pq.push({ 0, nodoOrigen });

    vector<vector<Arista>>& lista = grafo.GetListaAdyacenciasOriginal();

    //mientras que la cola con prioridad no este vacia
    while (!pq.empty())
    {
        // agarramos distancia y nodo
        double distanciaActual = pq.top().first;

        int nodoActual = pq.top().second;

        //sacamos de la cola de prioridad
        pq.pop();

        // si la distancia que obtenmos es mayor a la que ya tiene, lo obviamos, no tiene sentido ir por ese camino
        if (distanciaActual > dist[nodoActual])
            continue;

        // Agarramos todos su vecinos
        for (Arista& arista : lista[nodoActual])
        {
            int vecino =
                arista.GetNodoDestino();

            double peso =
                arista.GetDistanciaMestros();

            double nuevaDist =
                dist[nodoActual] + peso;

            // hacemos el mismo analisis de distancia
            if (nuevaDist < dist[vecino])
            {
                dist[vecino] = nuevaDist;

                pq.push({
                    nuevaDist,
                    vecino
                    });
            }
        }
    }

    /*
        Buscar el nodo mas lejano
        alcanzable desde nodoOrigen
    */

    double distanciaMaxima = 0;

    //asumimos que el nodo origen tiene la distancia mas larga del nodo origen -> Esto es para inicializar, no es verdad
    int nodoMasLejano = nodoOrigen;

    // Vamos a todos las distancias de dist
    // dist es un vector de distancias donde cada casilla guarda una distancia y la posicion es al nodo que podemos llegar
    for (int i = 0; i < n; i++)
    {
        // si es infinito no se puede ir a ese nodo 
        if (dist[i] == INF)
            continue;

        // si la distancia es mayor a la registrada la cambiamos, y guardamos el nodo destino
        if (dist[i] > distanciaMaxima)
        {
            distanciaMaxima = dist[i];
            nodoMasLejano = i;
        }
    }

    return {
        distanciaMaxima,
        nodoMasLejano
    };
}