#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

struct Mozo {
    int   idMozo;
    char  nombre[50];
    char  password[20];
    float totalComision;
};

struct Comanda {
    int   idMozo;
    int   codigoProducto;
    int   cantidad;
    float comision;
};


int cantidadRegistros(FILE* f) {
    Mozo r;
    int posActual = ftell(f), cantReg;
    fseek(f, 0, SEEK_END);
    cantReg = ftell(f) / sizeof(r);
    fseek(f, posActual, SEEK_SET);
    return cantReg;
}


int buscarMozo(FILE* f, int id, Mozo& m) {
    if (f == NULL) return -1;
    int p = 0;
    int u = cantidadRegistros(f) - 1;
    int med;
    while (p <= u) {
        med = (p + u) / 2;
        fseek(f, med * sizeof(m), SEEK_SET);
        fread(&m, sizeof(m), 1, f);
        if (m.idMozo == id) return med;
        else if (id > m.idMozo) p = med + 1;
        else u = med - 1;
    }
    return -1;
}


void mostrarMozo(FILE* fMozos, int id, int productos, float comision) {
    Mozo m;
    cout << "Mozo " << id << " - ";
    if (buscarMozo(fMozos, id, m) != -1)
        cout << m.nombre;
    else
        cout << "(mozo no encontrado en mozos.dat)";
    cout << " | Productos vendidos: " << productos
         << " | Comision a pagar: $" << comision << endl;
}
