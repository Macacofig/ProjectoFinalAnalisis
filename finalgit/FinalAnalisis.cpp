#include "Grafo.h"
#include "GraphLoader.h"

#include "BFSCDC.h" // componentes debilmente conexos
#include "DSU.h" // componentes debilmente conexos

#include "Reachability.h" // Alcance Vehicular
#include "BelmanFord.h"

#include "KruskalMST.h" // Emergencia
#include "PrimMST.h" // Emergencia

#include "ComparacionRuta.h"

#include <chrono>
using namespace std;

GraphLoader cargadorGrafos;

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

    vector<bool> NodosVisitado(cargadorGrafos.getGrafoNoDirigido().GetTotalNodos(), false);

    //VARIABLES DE RESULTADO

    int TotalComponentesConexas = 0; // cantidad de nodos que devulve bfs

    int TamanoComponenteGigante = 0; // la cantidad mas grande devuelta por bfs

    //recorrer todos los nodos del grafo no dirigido

    for (int NodoActual = 0; NodoActual < cargadorGrafos.getGrafoNoDirigido().GetTotalNodos(); NodoActual++)
    {
        //si el nodo no fue visitado hacemos bfs

        if (!NodosVisitado[NodoActual]) {

            vector<int> componenteActual; // MST
            //nuevo componente encontrado

            TotalComponentesConexas++;

            int TamanoComponenteActual = BFSCDC::BFSComponenteConexa(NodoActual, NodosVisitado, cargadorGrafos.getGrafoNoDirigido(),componenteActual);

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

    int n = cargadorGrafos.getGrafoNoDirigido().GetTotalNodos();

    DSU dsu(n);

    vector<vector<Arista>>& listaAdyacencias = cargadorGrafos.getGrafoNoDirigido().GetListaAdyacenciasOriginal();

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

    int alcanzables = Reachability::AlcanceVehicular(nodoOrigen, cargadorGrafos.getGrafoDirgido(), 5000);

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

    int alcanzablesb = BelmanFord::AlcanceVehicular(cargadorGrafos.getGrafoDirgido(), nodoOrigen, 5000);

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
    cout << "\nRUTA EMERGENCIA MINIMA\n";

    /*
    KRUSKAL O(ElogE)
        1.1) Del grafoNoDirigido extraemos todas las aristas para tener solo una lista de aristas
        1.2) Las aristas extraidas tienen que pertenecer al componente gigante (Revisar MST en BFS para entender como se extrae)
        1.3) Las aristas pueden ser duplicadas, si llegamos a ese caso ignoramos 1
        2) Ordenamos las aristas por peso (distancia)
        3) Aplicamos DSU : Esto ayuda a la deteccion de ciclo de forma eficiente, si un par de datos tienen el mismo padre ya estan conectados
        3.1) Recorremos aristas ordenadas ; extraemos (origen,destino,distancia)
        3.2) Si origen y destino no pertenecen al mismo grupo hacemos que formen el mismo grupo
        3.3) Agregamos la distancia a la distanciatotalrecorrida
        3.4) Si las aristas que tenemos es Vertices - 1 cortamos el bucle (MST tiene V - 1)
    */
    vector<bool> nodosComponenteGigante(cargadorGrafos.getGrafoNoDirigido().GetTotalNodos(), false);

    for (int nodo : componenteGiganteNodos)
    {
        nodosComponenteGigante[nodo] = true;
    }
    auto inicio = chrono::high_resolution_clock::now();
    double distanciaTotalMetros = KruskalMST::ConstruirMST(cargadorGrafos.getGrafoNoDirigido(),nodosComponenteGigante, componenteGiganteNodos.size());
    auto fin = chrono::high_resolution_clock::now();

    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "\nKRUSKAL MST\n";
    cout << "Tiempo: " << duracion.count() << " ms\n";
    cout << "Distancia total MST: " << distanciaTotalMetros / 1000.0 << " km\n";


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

    auto inicioprim = chrono::high_resolution_clock::now();

    double distanciaPrim = PrimMST::ConstruirMST(cargadorGrafos.getGrafoNoDirigido(),nodosComponenteGigante);

    auto finprim = chrono::high_resolution_clock::now();

    auto duracionprim = chrono::duration_cast<chrono::milliseconds>(finprim - inicioprim);

    
    cout << "\nPRIM MST\n";
    cout << "Tiempo: " << duracionprim.count() << " ms\n";

    cout << "Distancia total: " << distanciaPrim / 1000.0 << " km\n";
}

void DiametroVial(vector<int>& componenteGiganteNodos)
{
    // Del componente grande, obtener la mayor distancia minima entre 2 nodos
    /*
        DIJKSTRA -> O((V + E) log V)
        V: Vertices
        E: Aristas
        TOTAL -> O(Vg · (Vg + Eg) log Vg)
        Vg : Vertices componente Gigante
        Eg : Aristas componente gigante

        1) Entrada a cada nodo del componenteGigante
        2) A cada nodo aplicar dijsktra que devuelve la lista de distancias del origen a cada nodo
        2.1) Nodo entra a dijkstra como priority_queue
        2.2) Lo extraemos y verificamos si su distancia es mejor a la guardada
        2.3) Si es mejor entramos a su lista de adyacencias, si no la obviamos
        2.4) A cada vecino del NodoActual hacemos la verificacion 2.2, si su distancia menor a la guardada cambiamos, y la colocamos
        en la priority_queue
        3) Si la distancia es INF no lo utilizamos
        4) Si no verificamos si la distancia a ese nodo es mayor a la guardada para reemplazar valores
    */
    
    cout << "\nDIAMETRO VIAL\n";
    auto iniciodij = chrono::high_resolution_clock::now();

    double distanciaguardar = 0;
    int origenguardar = -1;
    int destinoguardar = -1;

    //RECORRER TODOS LOS NODOS DE LA COMPONENTE GIGANTE
    for (int origen : componenteGiganteNodos)
    {
        // A cada nodo aplicamos dijkstra, devolviendo una lista de distacancias minimas a un nodoDestino desde el origenActual
        vector<double> dist = Reachability::DiametroVial(origen, cargadorGrafos.getGrafoNoDirigido());

        // Buscar el nodo más lejano alcanzable desde este origen.

        for (int destino : componenteGiganteNodos)
        {
            // Ignorar nodos no alcanzables
            if (dist[destino] == numeric_limits<double>::infinity())
            {
                continue;
            }

            if (dist[destino] > distanciaguardar)
            {
                distanciaguardar = dist[destino];

                origenguardar = origen;

                destinoguardar = destino;
            }
        }
    }
    auto findij = chrono::high_resolution_clock::now();

    auto duraciondij = chrono::duration_cast<chrono::milliseconds>(findij - iniciodij);
    cout << "\DIJSKTRA\n";
    cout << "Tiempo: " << duraciondij.count() << " ms\n";
    cout << "Del Nodo "<<origenguardar<< " al Nodo "<<destinoguardar<<" existe una distancia de : " << distanciaguardar;
    

    /*
        DOUBLE SWEEP

        1) Elegimos cualquier nodo

        2) Ejecutamos Dijkstra

        3) Encontramos el nodo más lejano A

        4) Ejecutamos Dijkstra desde A

        5) Encontramos el nodo más lejano B

        6) La distancia A-B es una aproximación del diámetro vial

        Complejidad:

        O((Vg + Eg) log Vg)

        Solo ejecutamos Dijkstra dos veces
    */

    cout << "\nDouble Sweep\n";

    auto inicio = chrono::high_resolution_clock::now();

    //Tomar cualquier nodo de la componente gigante.

    int nodoInicial = componenteGiganteNodos[0];

    //Primer sweep

    pair<double, int> primerSweep =
        Reachability::DiametroVialDoubleSweep(nodoInicial,cargadorGrafos.getGrafoNoDirigido());

    int extremoA = primerSweep.second;

    //Segundo sweep

    pair<double,int> segundoSweep =
        Reachability::DiametroVialDoubleSweep(extremoA,cargadorGrafos.getGrafoNoDirigido());

    int extremoB =segundoSweep.second;

    double diametro =segundoSweep.first;

    auto fin =chrono::high_resolution_clock::now();

    auto duracion =chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "Tiempo: "<< duracion.count() << " ms\n";

    cout << "Nodo extremo 1: "<< extremoA << " / ";

    cout << "Nodo extremo 2: "<< extremoB << " / ";
     
    cout << "Distancia: "<< diametro << " metros\n";

    cout << "Distancia: "<< diametro / 1000.0 << " km\n";

}

void RutaPorHorario()
{
    cout << "\nBONUS - Ruta por Tipo de Horario\n";

    int origen;
    int destino;

    cout << "Nodo origen: ";
    cin >> origen;

    cout << "Nodo destino: ";
    cin >> destino;

    auto inicio = chrono::high_resolution_clock::now();

    ComparacionRuta::ResultadoRuta rutaDistancia =
        ComparacionRuta::DijkstraPeso(
            cargadorGrafos.getGrafoDirgido(),
            origen,
            destino,
            ComparacionRuta::DISTANCIA
        );

    ComparacionRuta::ResultadoRuta rutaTiempo =
        ComparacionRuta::DijkstraPeso(
            cargadorGrafos.getGrafoDirgido(),
            origen,
            destino,
            ComparacionRuta::TIEMPO
        );

    auto fin =chrono::high_resolution_clock::now();

    auto duracion =chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "\nRESULTADOS\n";

    cout << "\nRuta minima por distancia\n";
    cout << "Distancia: "<< rutaDistancia.costo / 1000.0<< " km\n";
    cout << "Nodos recorridos: " << rutaDistancia.cantidadNodos << endl;
    /*
    cout << "Recorridos: " << endl;
    for (auto& nodo : rutaDistancia.camino)
    {
        cout << nodo << "|";
    }*/
    cout << endl;
    cout << "\nRuta minima por tiempo\n";
    cout << "Tiempo estimado: "<< rutaTiempo.costo<< " segundos\n";
    cout << "Distancia: " << rutaTiempo.dist / 1000.0 << " km\n";
    cout << "Nodos recorridos: " << rutaTiempo.cantidadNodos << endl;
    
    /*cout << "Recorridos: " << endl;
    for (auto& nodo : rutaTiempo.camino)
    {
        cout << nodo << "|";
    }*/
    cout << "\nTiempo de ejecucion: "<< duracion.count() << " ms\n";
}
int main()
{
    //cargadorGrafos.cargarGrafoDesdeCSV("nodes.csv", "edges.csv");
    //cargadorGrafos.cargarGrafoDesdeCSV("nodes_clean.csv", "edges_clean.csv");
    cargadorGrafos.cargarGrafoDesdeCSV("nodes_smart.csv", "edges_smart.csv");
    cout << "GRAFO CARGADO CORRECTAMENTE\n";
    cout << "Total de nodos: " << cargadorGrafos.getGrafoDirgido().GetTotalNodos() << "\n";
    cout << "Total de aristas dirigido: " << cargadorGrafos.getGrafoDirgido().GetTotalAristas() << "\n";
    cout << "Total de nodos: " << cargadorGrafos.getGrafoNoDirigido().GetTotalNodos() << "\n";
    cout << "Total de aristas no dirigido: " << cargadorGrafos.getGrafoNoDirigido().GetTotalAristas() << "\n";
 
    vector<int> componenteGiganteNodos;
    //ComponentesDebilmenteConexos(componenteGiganteNodos);
    //AlcanceVehicular();
    //RutaEmergenciaMinima(componenteGiganteNodos);
    //DiametroVial(componenteGiganteNodos);
    RutaPorHorario();

    
}