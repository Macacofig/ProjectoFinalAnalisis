#include "Grafo.h"
#include "GraphLoader.h"

#include "BFSCDC.h" // componentes debilmente conexos
#include "DSU.h" // componentes debilmente conexos
#include "Reachability.h" // Alcance Vehicular

#include <chrono>
using namespace std;

GraphLoader cargador;

void ComponentesDebilmenteConexos()
{  
    //solamente verificar si hay conexion entre nodos por eso se usa el grafo no dirigido

    /*

    BFS -> O(V + E)
        1) De un nodo inicial vamos a todos su vecinos
        2) De esos vecinos igual a sus vecinos
        3) eso sera un componente conexo y retorna la cantidad de nodos visitados
        4) asi con nada nodo del grafo, respetando que los que ya estan visitados no se cuentan
    */
    auto inicio_bfs = chrono::high_resolution_clock::now();

    vector<bool> NodosVisitado(cargador.getGrafoNoDirigido().GetTotalNodos(), false);

    //VARIABLES DE RESULTADO

    int TotalComponentesConexas = 0; // cantidad de nodos que devulve bfs

    int TamanoComponenteGigante = 0; // la cantidad mas grande devuelta por bfs

    //recorrer todos los nodos del grafo no dirigido

    for (int NodoActual = 0; NodoActual < cargador.getGrafoNoDirigido().GetTotalNodos(); NodoActual++)
    {
        //si el nodo no fue visitado hacemos bfs

        if (!NodosVisitado[NodoActual]) {


            //nuevo componente encontrado

            TotalComponentesConexas++;

            int TamanoComponenteActual = BFSCDC::BFSComponenteConexa(NodoActual, NodosVisitado, cargador.getGrafoNoDirigido());

            // ACTUALIZAR COMPONENTE GIGANTE

            if (TamanoComponenteActual > TamanoComponenteGigante)
            {
                TamanoComponenteGigante = TamanoComponenteActual;
            }
        }
    }
    auto fin_bfs = chrono::high_resolution_clock::now();

    auto duracion_bfs =
        chrono::duration_cast<chrono::milliseconds>(fin_bfs - inicio_bfs);
    cout << "COMPONENTES DEBILMENTE CONEXAS\n";

    cout << "\nTIEMPO BFS: " << duracion_bfs.count() << " ms\n";
    cout << "Total de componentes conexas: " << TotalComponentesConexas << "\n";
    cout << "Tamano de la componente gigante: " << TamanoComponenteGigante << "\n";
    cout << "Cantidad de islas viales: " << TotalComponentesConexas - 1 << "\n";

    /*

    DSU -> O((V+E) alpha (V))  -> O(V+E)
    alpha(V) Función inversa de Ackermann -> 1
        1) Crear nodos donde cada uno es su propio padre (componentes diferentes)
        2) Unir componentes del dsu segun lo que dice la lista de adyacencias original del grafo
        3) Para contar cantidad de nodos de componente si un nodo tiene el mismo padre se agrega al tam del componente
        4) Para encontrar cantidad total de componentes -> si el nodo es su mismo padre es un componente
        si otro nodo tiene el otro padre que no es el no cuenta

    */
    auto inicio_dsu = chrono::high_resolution_clock::now();

    int n = cargador.getGrafoNoDirigido().GetTotalNodos();

    DSU dsu(n);

    vector<vector<Arista>>& lista = cargador.getGrafoNoDirigido().GetListaAdyacenciasOriginal();

    // 1. UNIR TODAS LAS ARISTAS O(E)
    for (int u = 0; u < n; u++)
    {
        for (auto& e : lista[u])
        {
            int v = e.GetNodoDestino();
            dsu.unite(u, v);
        }
    }

    // 2. CONTAR COMPONENTES O(V)
    vector<int> compSize(n, 0);
    int componenteGigante = 0;
    for (int i = 0; i < n; i++)
    {
        int root = dsu.find(i);
        compSize[root]++;
        componenteGigante = max(componenteGigante, compSize[root]);
    }

    int total = 0;
    for (int i = 0; i < n; i++)
    {
        if (dsu.find(i) == i) total++;
    }

    auto fin_dsu = chrono::high_resolution_clock::now();

    auto duracion_dsu =
        chrono::duration_cast<chrono::milliseconds>(fin_dsu - inicio_dsu);

    cout << "\nTIEMPO DSU: " << duracion_dsu.count() << " ms\n";

    cout << "COMPONENTES DEBILMENTE CONEXAS (DSU)\n";
    cout << "Total de componentes: " << total << "\n";
    cout << "Componente gigante: " << componenteGigante << "\n";
    cout << "Islas viales: " << total - 1 << "\n";
}
int main()
{
    cargador.cargarGrafoDesdeCSV("nodes.csv", "edges.csv");
    cout << "GRAFO CARGADO CORRECTAMENTE\n";
    cout << "Total de nodos: " << cargador.getGrafoDirgido().GetTotalNodos() << "\n";
    cout << "Total de aristas dirigido: " << cargador.getGrafoDirgido().GetTotalAristas() << "\n";
    cout << "Total de nodos: " << cargador.getGrafoNoDirigido().GetTotalNodos() << "\n";
    cout << "Total de aristas no dirigido: " << cargador.getGrafoNoDirigido().GetTotalAristas() << "\n";
 
    //Dado un nodo origen verificar nodos alcanzables en menos de 5km

    /*

    Dijkstra O((V+E)logV)
        1) 
    */
    cout << "\nALCANCE VEHICULAR\n";
    auto inicio_dsu = chrono::high_resolution_clock::now();
    int nodoOrigen = 0;
    
    int alcanzables =
        Reachability::AlcanceVehicular(
            nodoOrigen,
            cargador.getGrafoDirgido(),
            5000
        );

    
    auto fin_dsu = chrono::high_resolution_clock::now();

    auto duracion_dsu =
        chrono::duration_cast<chrono::milliseconds>(fin_dsu - inicio_dsu);

    cout << "\nTIEMPO DIJSKTRA: " << duracion_dsu.count() << " ms\n";
    cout << "Nodos alcanzables en 5 km: "
        << alcanzables
        << "\n";
    
}