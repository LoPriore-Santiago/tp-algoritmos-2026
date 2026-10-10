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

const float TASA_COMISION = 0.10f; 

const int K = 2; 

void encriptar(char password[]) {
    int K=2;  
    int i=0;
    while (password[i] != '\0') {
        password[i] = password[i] + K; 
        i++;
    }
}
int procesarMozo(Mozo mozos[], int &cantMozos, const char nombre[], float comision) {
  for (int i=0; i < cantMozos; i++) {
        if (mozos[i].nombre == nombre) {
            mozos[i].totalcomision += comision;
            return mozos[i].id;
        }
        int nuevoID = cantMozos +1;
        mozos[cantMozos].id = nuevoID;
       
    }
    return 0;
}
void generarArchivoMozos(Mozo mozos[], int cantMozos) {
}
int procesarMozo(Mozo mozos[], int &cantMozos, const char nombre[], float comision) {
    
    
    for (int i = 0; i < cantMozos; i++) {
        
        int j = 0;
        while (mozos[i].nombre[j] != '\0' && nombre[j] != '\0' && mozos[i].nombre[j] == nombre[j]) {
            j++;
        }
        if (mozos[i].nombre[j] == '\0' && nombre[j] == '\0') {
            mozos[i].totalcomision += comision; 
            return mozos[i].id; 
        }
    }