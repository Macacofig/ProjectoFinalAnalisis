#include "Grafo.h"
#include "GraphLoader.h"

#include "BFSCDC.h" // componentes debilmente conexos
#include "DSU.h" // componentes debilmente conexos

#include "Reachability.h" // Alcance Vehicular
#include "BelmanFord.h"

#include "KruskalMST.h" // Emergencia
#include "PrimMST.h" // Emergencia

#include <chrono>
using namespace std;

GraphLoader cargador;

void ComponentesDebilmenteConexos(vector<int>& componenteGiganteNodos)
{  
    cout << "\nCOMPONENTES DEBILMENTE CONEXAS\n";
    //solamente verificar si hay conexion entre nodos por eso se usa el grafo no dirigido

    /*

    BFS -> O(V + E)
        1) De un nodo inicial vamos a todos su vecinos
        2) De esos vecinos igual a sus vecinos
        3) eso sera un componente conexo y retorna la cantidad de nodos visitados
        4) asi con cada nodo del grafo, respetando que los que ya estan visitados no se cuentan
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

            vector<int> componenteActual; // MST
            //nuevo componente encontrado

            TotalComponentesConexas++;

            int TamanoComponenteActual = BFSCDC::BFSComponenteConexa(NodoActual, NodosVisitado, cargador.getGrafoNoDirigido(),componenteActual);

            // ACTUALIZAR COMPONENTE GIGANTE

            if (TamanoComponenteActual > TamanoComponenteGigante)
            {
                TamanoComponenteGigante = TamanoComponenteActual;
                componenteGiganteNodos = componenteActual;
            }
        }
    }
    auto fin_bfs = chrono::high_resolution_clock::now();

    auto duracion_bfs = chrono::duration_cast<chrono::milliseconds>(fin_bfs - inicio_bfs);
    

    cout << "\nTIEMPO BFS: " << duracion_bfs.count() << " ms\n";
    cout << "Total de componentes conexas: " << TotalComponentesConexas << "\n";
    cout << "Tamano de la componente gigante: " << TamanoComponenteGigante << "\n";
    cout << "Cantidad de islas viales: " << TotalComponentesConexas - 1 << "\n";

    /*

    DSU -> O(V+E)
        1) Crear nodos donde cada uno es su propio padre (componentes diferentes)
        2) Unir componentes del dsu segun lo que dice la lista de adyacencias original del grafo
        3) Para contar cantidad de nodos de componente si un nodo tiene el mismo padre se agrega al tam del componente
        4) Para encontrar cantidad total de componentes -> si el nodo es su mismo padre es un componente
        si otro nodo tiene el otro padre que no es el no cuenta

    */
    auto inicio_dsu = chrono::high_resolution_clock::now();

    int n = cargador.getGrafoNoDirigido().GetTotalNodos();

    DSU dsu(n);

    vector<vector<Arista>>& listaAdyacencias = cargador.getGrafoNoDirigido().GetListaAdyacenciasOriginal();

    // 1. UNIR TODAS LAS ARISTAS O(E)
    for (int u = 0; u < n; u++)
    {
        for (auto& e : listaAdyacencias[u])
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

    auto duracion_dsu = chrono::duration_cast<chrono::milliseconds>(fin_dsu - inicio_dsu);

    cout << "\nTIEMPO DSU: " << duracion_dsu.count() << " ms\n";

    cout << "COMPONENTES DEBILMENTE CONEXAS (DSU)\n";
    cout << "Total de componentes: " << total << "\n";
    cout << "Componente gigante: " << componenteGigante << "\n";
    cout << "Islas viales: " << total - 1 << "\n";
}

void AlcanceVehicular()
{
    //Dado un nodo origen verificar nodos alcanzables en menos de 5kmq (5000 metros) 

    /*

    Dijkstra O((V+E)logV)

    V pops
    E push
    LogV (insercion, eliminacion)
    donde V (nodo) , E (aristas) estan sujetas a un k
    k = el limite de distancia que un camino puede tener para llagar a un nodo

        1) Inicializamos un vector de n casillas con INF como dato : Este vector guarda todos las distancias minimos <= 5000
        2) Utilizar un priority queue para obtener siepre el primer elemento agregado, con greater para que ordene por menor
        3) Mientras que el priority queue no este vacio (nodos pendientes)
        4) Agarramos el primer nodo verificamos que su distancia no sobrepase al limite y entramos a sus vecinos
        5) A cada nodo vecino agarramos el nodo y su distancia acumulada
        6.1) Si la distancia acumulada es menor a la distancia que ya tenia el nodo al que queremos ir guardamos esa distancia (mejor camino encontrado)
        6.2) Pero tambien debe cumplir que esa nueva distancia entre dentro del limite
    */
    cout << "\nALCANCE VEHICULAR\n";
    int nodoOrigen = 0;

    auto inicio_dsu = chrono::high_resolution_clock::now();

    int alcanzables = Reachability::AlcanceVehicular(nodoOrigen, cargador.getGrafoDirgido(), 5000);

    auto fin_dsu = chrono::high_resolution_clock::now();

    auto duracion_dsu = chrono::duration_cast<chrono::milliseconds>(fin_dsu - inicio_dsu);

    cout << "\nTIEMPO DIJSKTRA: " << duracion_dsu.count() << " ms\n";
    cout << "Nodos alcanzables en 5 km: " << alcanzables << "\n";

    /*

    BellmanFord O(V*E)
        1) Inicializar vector de distancias de tamaño n (nodos) todos con dist inf
        2) EL primer nodo (origen) cambiamos distancia a 0
        3) Repetimos proceso de analisis Vertices - 1 veces o hasta ue no realicemos un cambio
        4.1) Entramos a cada nodo del grafo y del nodo a su lista de adyacencias (vecinos)
        4.2) De cada vecino obtenemos nodo destino y distancia para llegar a ese nodo, ya tenemos nodoOrigen
        5) Si el nodo en el que estamos tiene distancia INF no lo usamos 
        6.1) Si la distancia acumulada es menor a la distancia que ya tenia el nodo al que queremos ir guardamos esa distancia (mejor camino encontrado)
        6.2) Pero tambien debe cumplir que esa nueva distancia entre dentro del limite
        7) si no realizamos ningun cambio salimos del bucle de repeticion del paso 3
    */
    auto inicio = chrono::high_resolution_clock::now();

    int alcanzablesb = BelmanFord::AlcanceVehicular(cargador.getGrafoDirgido(), nodoOrigen, 5000);

    auto fin = chrono::high_resolution_clock::now();

    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "\nBELLMAN FORD\n";

    cout << "Tiempo: " << duracion.count() << " ms\n";
    cout << "Nodos alcanzables en 5km: " << alcanzablesb << "\n";
    
}


void RutaEmergenciaMinima(vector<int>& componenteGiganteNodos)
{
    // reportar la distancia total cubierta en km en un MST
    /*
    Un MST:
    - conecta todos los nodos
    - usa la menor distancia total posible
    - NO forma ciclos
    */
    cout << "Ruta Emergencia Minima\n";

    /*
    KRUSKAL O(ElogE)
        1.1) Del grafoNoDirigido extraemos todas las aristas para tener solo una lista de aristas
        1.2) Las aristas extraidas tienen que pertenecer al componente gigante (Revisar MST en BFS para entender como se extrae)
        1.3) Las aristas pueden ser duplicadas, si llegamos a ese caso ignoramos 1
        2) Ordenamos las aristas por peso (distancia)
        3) Aplicamos DSU : Explicar Porque *****
        3.1) Recorremos aristas ordenadas ; extraemos (origen,destino,distancia)
        3.2) Si origen y destino no pertenecen al mismo grupo hacemos que formen el mismo grupo
        3.3) Agregamos la distancia a la distanciatotalrecorrida
        3.4) Si las aristas que tenemos es Vertices - 1 cortamos el bucle (MST tiene V - 1)
    */
    vector<bool> nodosComponenteGigante(cargador.getGrafoNoDirigido().GetTotalNodos(), false);

    for (int nodo : componenteGiganteNodos)
    {
        nodosComponenteGigante[nodo] = true;
    }
    auto inicio = chrono::high_resolution_clock::now();
    double distanciaTotalMetros = KruskalMST::ConstruirMST(cargador.getGrafoNoDirigido(),nodosComponenteGigante, componenteGiganteNodos.size());
    auto fin = chrono::high_resolution_clock::now();

    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "\nKRUSKAL MST\n";
    cout << "Tiempo: " << duracion.count() << " ms\n";
    cout << "Distancia total MST: " << distanciaTotalMetros / 1000.0 << " km\n";


    auto inicioprim = chrono::high_resolution_clock::now();

    double distanciaPrim = PrimMST::ConstruirMST(cargador.getGrafoNoDirigido(),nodosComponenteGigante);

    auto finprim = chrono::high_resolution_clock::now();

    auto duracionprim = chrono::duration_cast<chrono::milliseconds>(finprim - inicioprim);

    /*
    PRIM O(ElogV)
        1) Agarramos la lista de adyacencias del grafo
        2) Unos un priority_queue minimo (distancia, NodoDestino)
        3) Buscamos el primer nodo que coincida con el componente gigante, lo encontramos y directo aplicamos PRIM
        4) Mientras el priority_queue no este vacio
        4.1) Extraemos el primer dato del p_q (distancia, NodoDestino), y eliminamos ese nodo del p_q
        4.2) Si el nodo ya fue visitado (ya esta dentro del MST) lo ignroamos y vamos al siguiente, si no lo marcamos como visitado
        4.3) Agregamos la distancia a la distancia total
        4.4) Del nodo vamos a todos sus vecinos
        4.5) De cada vecino agarramos (NodoDestino , distancia)
        4.6) Repetimos el mismo control que en el 4.2,pero no lo marcamos como visitado. Además verificamos que ese nodo sea parte del componente gigante
        4.7) Agregamos el vecino al p_q
    */
    cout << "\nPRIM MST\n";
    cout << "Tiempo: " << duracionprim.count() << " ms\n";
    cout << "Distancia total: " << distanciaPrim / 1000.0 << " km\n";
}
int main()
{
    cargador.cargarGrafoDesdeCSV("nodes.csv", "edges.csv");
    cout << "GRAFO CARGADO CORRECTAMENTE\n";
    cout << "Total de nodos: " << cargador.getGrafoDirgido().GetTotalNodos() << "\n";
    cout << "Total de aristas dirigido: " << cargador.getGrafoDirgido().GetTotalAristas() << "\n";
    cout << "Total de nodos: " << cargador.getGrafoNoDirigido().GetTotalNodos() << "\n";
    cout << "Total de aristas no dirigido: " << cargador.getGrafoNoDirigido().GetTotalAristas() << "\n";
 
    vector<int> componenteGiganteNodos;
    ComponentesDebilmenteConexos(componenteGiganteNodos);
    AlcanceVehicular();
    RutaEmergenciaMinima(componenteGiganteNodos);

    

    
}