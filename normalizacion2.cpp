#include <iostream>
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
    char fecha[11]; //"DD-MM-AAAA"
    char nombreMozo [50]; // el nombre completo, repetido en cada venta
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

const float TASA_COMISION = 0.10f; //la comision de cada venta es el 10% de lo vendido

const int K = 2; 