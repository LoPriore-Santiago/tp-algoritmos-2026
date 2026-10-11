#include <iostream>
#include <cstdio>
#include <cstring>



using namespace std;

struct Mozo {
 int id;
 char nombre [50];
 char password [20];
 float totalcomision;
 };
struct Producto{
    int codigo; 
   char descripcion [50];
    float precio; 
    int stockActual; 
};

struct Comandahistorica {
    char fecha[11]; //
    char nombreMozo [50]; 
    int codigoProducto;
    int cantidad;
    float comision;
};

struct comanda {
int idMozo;
int codigoProducto;
int cantidad;
float comision;
};

void actualizarStock (int codigoProducto, int cantidadVendida){
    FILE *inventario = fopen("inventario.dat", "rb+");
    if (inventario == NULL){
        cout << "Error: no se encontraro productos." << endl;
        return;
    }

 Producto r;

 while (fread(&r, sizeof(Producto), 1, inventario) == 1) {
    if (r.codigo == codigoProducto) {
     r.stockActual = r.stockActual - cantidadVendida;
      fseek(inventario, -(long)sizeof(Producto), SEEK_CUR);
      fwrite(&R, sizeof(Producto), 1, inventario);
      break;
    
    }
 } 

  fclose(inventario);
}

void procesarPlanillaDiaria(const char fecha[], comanda nuevaComanda) {

    char nombreArchivo [50];

    

    FILE *archivoDia = fopen (nombreArchivo, "ab");

    if (archivoDia == NULL) {
        cout << "Error al abrir la planilla del día" << endl;
        return;
    }


}