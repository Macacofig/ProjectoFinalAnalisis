#include "Grafo.h"
#include <unordered_map>
#include <fstream>
/*
DIVIDIR LINEA CSV 

Permite de una linea del csv dividirlo en columnas
*/
vector<string> splitCSVLine(string& line) {

    vector<string> tokens;

    string token;

    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    return tokens;
}

int main()
{
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


    /*
    VARIABLES DE CONTROL
    cargar los nodos para entrada al grafo
    */

    int TotalNodosCargados = 0;

    /*
    ABRIR ARCHIVO NODES.CSV
    */

    ifstream ArchivoNodos("nodes.csv");

    if (!ArchivoNodos.is_open()) {
        cout << "Error al abrir nodes.csv\n";
        return 1;
    }

    string LineaAgarradaDelCSV;

    /*
    IGNORAR CABECERA
    agarramos la cabecera y el puntero del csv apuntara al primer nodo
    */

    getline(ArchivoNodos, LineaAgarradaDelCSV);

    /*
    LEER NODOS
    */

    while (getline(ArchivoNodos, LineaAgarradaDelCSV)) {

        vector<string> columnas = splitCSVLine(LineaAgarradaDelCSV);

        /*
        CSV: node_id,lat,lon
        */

        long long IdNodo = stoll(columnas[0]); // convertir de string a long long

        // total inicia en 0 por lo tanto el primer index sera 0, trabajamos con vectores
        int Index = TotalNodosCargados; 

        //En el unorderedmap la clave sera el id(encontrarlo) y el valor index(para el grafo)
        IdRealAIndex[IdNodo] = Index;

        //Agregamos el id real a nuestro vector index(encontrarlo) y el valor id real(para el unordered)
        IndexAIdReal.push_back(IdNodo);

        TotalNodosCargados++; 
    }

    ArchivoNodos.close();

}

