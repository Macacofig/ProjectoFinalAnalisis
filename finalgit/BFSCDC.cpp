#include "BFSCDC.h"

//BFS PARA ENCONTRAR EL TAMANIO  DE UN COMPONENTE CONEXO
//Visitados no se resetea -> porque queremos pasar una sola vez todo el grafo
//si pasamos por un nodo ya visitado esque ya pertenece a un componente; lo pasamos
int BFSCDC::BFSComponenteConexa(int NodoInicial, vector<bool>& NodoVisitado, Grafo& grafoNoDirigido, vector<int>& nodosComponente )
//usar el mismo vector de visitados creado
//Usar el mismo grafo creado
{
    queue<int> ColaNodosPendientes; // Necesitamos ir buscando de acuerdo al orden de descubrimiento
    //primero en entrar primero en salir
    ColaNodosPendientes.push(NodoInicial);

    NodoVisitado[NodoInicial] = true;

    int TamanoComponente = 0; //contador de nodos del componente

    //OBTENER LISTA DE ADYACENCIA
    // utilizar la lista original ya que no la modificamos
    vector<vector<Arista>>& ListaAdyacencia = grafoNoDirigido.GetListaAdyacenciasOriginal();

    while (!ColaNodosPendientes.empty())
    {
        //SACAR NODO ACTUAL

        int NodoActual = ColaNodosPendientes.front();

        ColaNodosPendientes.pop();

        //CONTAR NODO

        TamanoComponente++;

        // guardar nodo del componente -> MST

        nodosComponente.push_back(NodoActual);

        //RECORRER VECINOS

        for (Arista& AristaActual : ListaAdyacencia[NodoActual])
        {
            int NodoVecino = AristaActual.GetNodoDestino();

            //SI EL VECINO NO FUE VISITADO

            if (!NodoVisitado[NodoVecino])
            {

                NodoVisitado[NodoVecino] = true;

                ColaNodosPendientes.push(NodoVecino);
            }
        }
    }

    return TamanoComponente;
}
