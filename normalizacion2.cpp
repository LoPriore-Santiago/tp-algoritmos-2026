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
    int nuevoID = cantMozos + 1; 
    mozos[cantMozos].id = nuevoID;

    int k = 0;
    while (nombre[k] != '\0') {
        mozos[cantMozos].nombre[k] = nombre[k];
        k++;
    }
    mozos[cantMozos].nombre[k] = '\0'; 
    mozos[cantMozos].totalcomision = comision;
    mozos[cantMozos].password[0] = '0' + nuevoID; 
    mozos[cantMozos].password[1] = '\0'; 
    encriptar(mozos[cantMozos].password);
    cantMozos++; 
    return nuevoID; 
}
void generarArchivoMozos(Mozo mozos[], int cantMozos) {
    for (int i = 0; i < cantMozos - 1; i++) {
        for (int j = 0; j < cantMozos - i - 1; j++) {
            if (mozos[j].id > mozos[j + 1].id) {
                Mozo aux = mozos[j];
                mozos[j] = mozos[j + 1];
                mozos[j + 1] = aux;
            }
        }
    }
    FILE *archivo = fopen("mozos.dat", "wb");
    if (archivo == NULL) {
        cout << "Error al abrir el archivo mozos.dat para escribir." << endl;
        return;
    }
    fwrite(mozos, sizeof(Mozo), cantMozos, archivo);
    fclose(archivo);
}