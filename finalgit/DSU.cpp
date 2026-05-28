#include "DSU.h"

/*
    Constructor:
    Inicialmente cada nodo es su propio padre
    cada nodo es su propia componente
*/
DSU::DSU(int n)
{
    parent.resize(n);
    size.resize(n, 1);

    for (int i = 0; i < n; i++) {
        parent[i] = i; // cada nodo es raíz de sí mismo
    }
}

/*
    FIND con PATH COMPRESSION

    Busca el representante (raíz) del conjunto

    0 -> 1 -> 2 -> 2
    0 su padre es 1
    1 su padre es 2
    2 su padre es 2
    Optimización:
    - mientras subimos al padre, vamos “aplanando” el árbol
    - esto hace que futuras búsquedas sean casi O(1)
*/
int DSU::find(int x)
{
    if (parent[x] == x)
        return x; // x es raíz
    // compresión de camino
    parent[x] = find(parent[x]); // path compression -> despues de un finde 0 su padre es 2
    return parent[x];
}

/*
    UNION (unir dos conjuntos)

    Pasos:
    1. encontrar raíces de ambos nodos
    2. si ya son iguales ? ya están en el mismo grupo
    3. unir el árbol pequeño al grande (union by size)
*/
void DSU::unite(int a, int b)
{
    a = find(a);
    b = find(b);
    // ya están en el mismo componente
    if (a == b) return;
    // unir por tamaño (optimización)
    if (size[a] < size[b])
    {
        parent[a] = b; // a cuelga de b
        size[b] += size[a]; // actualiza tamaño
    }
    else
    {
        parent[b] = a; // b cuelga de a
        size[a] += size[b];
    }
}

/*
    Obtener tamaño del componente de x

    Solo miras la raíz y consultas su size
*/
int DSU::getComponentSize(int x)
{
    return size[find(x)];
}
