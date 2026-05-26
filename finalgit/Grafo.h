#pragma once
#include "Arista.h"

class Grafo
{
private:
    vector<vector<Arista>> ListaAdyacencias;

public:
    Grafo(int totalNodes);

    void AgregarArista(int NodoOrigen, int NodoDestino, double DistanciaMetros, double VelocidadMaxima);

    vector<vector<Arista>>& GetListaAdyacenciasOriginal();

    int GetTotalNodos();

    int GetTotalAristas();
};
