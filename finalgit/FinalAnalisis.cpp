#include "Grafo.h"
#include "BFSCDC.h" // componentes debilmente conexos
#include "GraphLoader.h"

using namespace std;

int main()
{
    GraphLoader cargador;

    cargador.cargarGrafoDesdeCSV("nodes.csv", "edges.csv");
    cout << "GRAFO CARGADO CORRECTAMENTE\n";
    cout << "Total de nodos: " << cargador.getGrafoDirgido().GetTotalNodos() << "\n";
    cout << "Total de aristas dirigido: " << cargador.getGrafoDirgido().GetTotalAristas() << "\n";
    cout << "Total de nodos: " << cargador.getGrafoNoDirigido().GetTotalNodos() << "\n";
    cout << "Total de aristas no dirigido: " << cargador.getGrafoNoDirigido().GetTotalAristas() << "\n";

    /*---------------------------------------------------*/
    /*---------------------------------------------------*/

    //COMPONENTES DEBILMENTE CONEXOS  -> O(V + E)

    /*
    solamente verificar si hay conexion entre nodos por eso se usa el grafo no dirigido

        1) De un nodo inicial vamos a todos su vecinos
        2) De esos vecinos igual a sus vecinos
        3) eso sera un componente conexo y retorna la cantidad de nodos visitados
        4) asi con nada nodo del grafo, respetando que los que ya estan visitados no se cuentan
    */

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

    cout << "COMPONENTES DEBILMENTE CONEXAS\n";
    cout << "Total de componentes conexas: " << TotalComponentesConexas << "\n";
    cout << "Tamano de la componente gigante: " << TamanoComponenteGigante << "\n";
    cout << "Cantidad de islas viales: " << TotalComponentesConexas - 1 << "\n";

}