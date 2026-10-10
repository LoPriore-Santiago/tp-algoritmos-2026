#include <iostream>
#include <cstdio>
#include <cstring>


using namespace std;

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