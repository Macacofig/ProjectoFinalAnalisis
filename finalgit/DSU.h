#pragma once
#include <vector>
using namespace std;

/*
    DSU (Disjoint Set Union) - Union-Find

    Sirve para mantener grupos de nodos conectados

    Operaciones clave:
    - find(x): encuentra el representante del grupo de x
    - unite(a,b): une los grupos de a y b
*/
class DSU
{
private:
    vector<int> parent; // parent[i] = padre del nodo i en el árbol DSU
    vector<int> size; // size[i] = tamaño del componente cuya raíz es i

public:
    DSU(int n);
    int find(int x);
    void unite(int a, int b);
    int getComponentSize(int x);
};

