#pragma once
#include "Grafo.h"
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

class GraphLoader
{

private:
    Grafo grafoDirigido;
    Grafo grafoNoDirigido;

    /*
    ESTRUCTURAS PARA MAPEAR IDS
    de un node_id real (80485621) a (1) -> Mejor trabajo
    */

    // node_id real : índice para el grafo
    unordered_map<long long, int> IdRealAIndex; // busqueda rapida O(1)
    // IdRealAIndex[80485621] devuelve 1

    // índice para el grafo : node_id real
    vector<long long> IndexAIdReal;
    // IndexAIdReal[1] devuelve 80485621
public:
    GraphLoader();
    void cargarGrafoDesdeCSV(string nodesFile,string edgesFile);

    vector<string> splitCSVLine(string& line);

    Grafo& getGrafoDirgido();
    Grafo& getGrafoNoDirigido();
};

