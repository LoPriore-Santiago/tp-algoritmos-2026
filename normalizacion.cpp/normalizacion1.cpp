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
      fwrite(&r, sizeof(Producto), 1, inventario);
      break;
    
    }
 } 

  fclose(inventario);
}

void procesarPlanillaDiaria(const char fecha[], comanda nuevaComanda) {

    char nombreArchivo [50];

    sprintf(nombreArchivo, "comandas_%s.dat", fecha);

    FILE *archivoDia = fopen (nombreArchivo, "ab");

    if (archivoDia == NULL) {
        cout << "Error al abrir la planilla del día" << endl;
        return;
    }

    fwrite(&nuevaComanda, sizeof(comanda), 1, archivoDia);
    fclose(archivoDia);

}

int main (){

    FILE *archivoHistorico = fopen("comandas_historicas.dat", "rb");
    
    if (archivoHistorico == NULL) {
        cout << "Error: No se pudo abrir comandas_historicas.dat" << endl;
        return 1;
    }

    Comandahistorica ventaLeida;

while (fread(&ventaLeida, sizeof(Comandahistorica), 1, archivoHistorico) == 1) {
    
}

}