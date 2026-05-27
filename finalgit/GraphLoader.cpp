#include "GraphLoader.h"

GraphLoader::GraphLoader()
    :grafoDirigido(0),grafoNoDirigido(0)
{
}
/*
Permite de una linea del csv dividirlo en columnas
*/
vector<string> GraphLoader::splitCSVLine(string& line)
{
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ','))
    {
        tokens.push_back(token);
    }

    return tokens;
}

Grafo& GraphLoader::getGrafoDirgido()
{
    return grafoDirigido;
}

Grafo& GraphLoader::getGrafoNoDirigido()
{
    return grafoNoDirigido;
}

void GraphLoader::cargarGrafoDesdeCSV(string nodesFile, string edgesFile)
{
    ifstream nodes(nodesFile);

    string line;

    /*
    IGNORAR CABECERA
    agarramos la cabecera y el puntero del csv apuntara al primer nodo
    */
    getline(nodes, line); 

    int index = 0; //cantidad de nodos

    while (getline(nodes, line))
    {
        //CSV: node_id,lat,lon
        vector<string>  cols = splitCSVLine(line);

        long long id = stoll(cols[0]); //cambiar long long

        //En el unorderedmap la clave sera el id(encontrarlo) y el valor index(para el grafo)
        IdRealAIndex[id] = index;
        
        //Agregamos el id real a nuestro vector index(encontrarlo) y el valor id real(para el unordered)
        IndexAIdReal.push_back(id);

        index++;
    }

    nodes.close();

    grafoDirigido = Grafo(index); // index = total nodos ingresados
    grafoNoDirigido = Grafo(index);

    ifstream edges(edgesFile);

    getline(edges, line);

    while (getline(edges, line))
    {
        auto cols = splitCSVLine(line);
        
        // CSV: osm_id,from_id,to_id,distance_m,fclass,oneway,maxspeed
        long long origen = stoll(cols[1]); //cambiar long long
        long long destino = stoll(cols[2]); 

        double dist = stod(cols[3]); //cambiar double
        int oneway = stoi(cols[5]); //cambiar entero
        double speed = stod(cols[6]);

        // at() : Busca la clave (origen) y devuelve el valor asociado (index).
        // origen ->(80485621) devuelve (1) ese 1 va al origenguardado
        int origenguardado = IdRealAIndex.at(origen); 
        int destinoguardado = IdRealAIndex.at(destino);

        grafoDirigido.AgregarArista(origenguardado, destinoguardado, dist, speed);

        /*
        SI LA CALLE ES BIDIRECCIONAL
        AGREGAR LA ARISTA CONTRARIA
        oneway = 0 hay doble ruta
        oneway = 1 solo una ruta
        */
        if (oneway == 0)
        {
            grafoDirigido.AgregarArista(destinoguardado, origenguardado, dist, speed);
        }

        grafoNoDirigido.AgregarArista(origenguardado, destinoguardado, dist, speed);
        grafoNoDirigido.AgregarArista(destinoguardado, origenguardado, dist, speed);
    }

    edges.close();
}
