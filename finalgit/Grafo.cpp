#include "Grafo.h"

Grafo::Grafo(int totalNodes)
{
	ListaAdyacencias.resize(totalNodes);
}

void Grafo::AgregarArista(int NodoOrigen, int NodoDestino, double DistanciaMetros, double VelocidadMaxima)
{
	Arista arista(NodoDestino, DistanciaMetros, VelocidadMaxima);
    ListaAdyacencias[NodoOrigen].push_back(arista);
}

vector<vector<Arista>>& Grafo::GetListaAdyacenciasOriginal()
{
	return ListaAdyacencias;
}

int Grafo::GetTotalNodos()
{
	return ListaAdyacencias.size();
}

int Grafo::GetTotalAristas() // O (N) -> Cantidad de nodos diferentes registrados
{
    int TotalAristas = 0;

    for (const auto& vecinos : ListaAdyacencias) 
    {
        TotalAristas += vecinos.size();
    }

    return TotalAristas;
}
