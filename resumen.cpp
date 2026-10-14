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

int main() {
    char nombreSemana[50];
    cout << "Nombre de la planilla semanal (ej: comandas_semana_s1-06.dat): ";
    cin >> nombreSemana;

    FILE* fSemana = fopen(nombreSemana, "rb");
    if (fSemana == NULL) {
        cout << "No se pudo abrir " << nombreSemana << endl;
        return 1;
    }
    
    FILE* fMozos = fopen("mozos.dat", "rb");
    if (fMozos == NULL)
        cout << "Aviso: no se encontro mozos.dat, se muestran solo los numeros." << endl;

    Comanda c;
    int mozoActual = -1;      
    int prodMozo = 0;
    float comMozo = 0;
    int totalBuffet = 0;

    cout << endl << "===== RESUMEN DE LA SEMANA =====" << endl;

   
    while (fread(&c, sizeof(c), 1, fSemana) == 1) {
        if (c.idMozo != mozoActual) {
            if (mozoActual != -1)                      
                mostrarMozo(fMozos, mozoActual, prodMozo, comMozo);
            mozoActual = c.idMozo;                   
            prodMozo = 0;
            comMozo = 0;
        }
        prodMozo += c.cantidad;
        comMozo += c.comision;
        totalBuffet += c.cantidad;
    }
    
    if (mozoActual != -1)
        mostrarMozo(fMozos, mozoActual, prodMozo, comMozo);
    else
        cout << "La planilla no tiene ventas." << endl;

    cout << "-----------------------------------" << endl;
    cout << "TOTAL DE PRODUCTOS VENDIDOS POR EL BUFFET: " << totalBuffet << endl;

    fclose(fSemana);
    if (fMozos != NULL) fclose(fMozos);
    return 0;
}
