#include <iostream>
#include <cstdio>

using namespace std;

struct Mozo {
 int idMozo;
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
    int i = 0;
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
            return mozos[i].idMozo; 
        }
    }
    
    int nuevoID = cantMozos + 1; 
    mozos[cantMozos].idMozo = nuevoID;

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
            if (mozos[j].idMozo > mozos[j + 1].idMozo) {
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

void ordenarPlanillaDia(const char nombreArchivo[]) {
    FILE *f = fopen(nombreArchivo, "rb");
    if (f == NULL) return;

    comanda ventasDelDia[500];
    int cantVentas = 0;
    
    while (fread(&ventasDelDia[cantVentas], sizeof(comanda), 1, f) == 1) {
        cantVentas++;
    }
    fclose(f);

    for (int i = 0; i < cantVentas - 1; i++) {
        for (int j = 0; j < cantVentas - i - 1; j++) {
            if (ventasDelDia[j].idMozo > ventasDelDia[j + 1].idMozo) {
                comanda aux = ventasDelDia[j];
                ventasDelDia[j] = ventasDelDia[j + 1];
                ventasDelDia[j + 1] = aux;
            }
        }
    }

    f = fopen(nombreArchivo, "wb");
    fwrite(ventasDelDia, sizeof(comanda), cantVentas, f);
    fclose(f);
}

int main() {
    Mozo arregloMozos[50]; 
    int cantidadDeMozos = 0;
    char planillasGeneradas[100][50]; 
    int cantPlanillas = 0;

    FILE *archivoHistorico = fopen("comandas_historicas.dat", "rb");
    
    if (archivoHistorico == NULL) {
        cout << "Error: No se pudo abrir comandas_historicas.dat." << endl;
        return 1;
    }

    Comandahistorica ventaLeida;

    cout << "Iniciando proceso de normalizacion..." << endl;

    while (fread(&ventaLeida, sizeof(Comandahistorica), 1, archivoHistorico) == 1) {
        
        actualizarStock(ventaLeida.codigoProducto, ventaLeida.cantidad);

        int idMozoEncontrado = procesarMozo(arregloMozos, cantidadDeMozos, ventaLeida.nombreMozo, ventaLeida.comision);

        comanda nuevaComanda;
        nuevaComanda.idMozo = idMozoEncontrado;
        nuevaComanda.codigoProducto = ventaLeida.codigoProducto;
        nuevaComanda.cantidad = ventaLeida.cantidad;
        nuevaComanda.comision = ventaLeida.comision;

        char nombreArchivoDia[50];
        sprintf(nombreArchivoDia, "comandas_%s.dat", ventaLeida.fecha);

        
        bool archivoYaExiste = false;
        for (int i = 0; i < cantPlanillas; i++) {
            int j = 0;
            bool sonIguales = true;
            
            while (planillasGeneradas[i][j] != '\0' || nombreArchivoDia[j] != '\0') {
                if (planillasGeneradas[i][j] != nombreArchivoDia[j]) {
                    sonIguales = false; 
                    break;
                }
                j++;
            }
            
            if (sonIguales) {
                archivoYaExiste = true;
                break;
            }
        }
       if (!archivoYaExiste) {
            int k = 0;
            while (nombreArchivoDia[k] != '\0') {
                planillasGeneradas[cantPlanillas][k] = nombreArchivoDia[k];
                k++;
            }
            planillasGeneradas[cantPlanillas][k] = '\0'; 
            cantPlanillas++;
        }
        procesarPlanillaDiaria(ventaLeida.fecha, nuevaComanda);
    }

    fclose(archivoHistorico);

    generarArchivoMozos(arregloMozos, cantidadDeMozos);

    for (int i = 0; i < cantPlanillas; i++) {
        ordenarPlanillaDia(planillasGeneradas[i]);
    }

    cout << "Normalizacion completada con exito." << endl;

    return 0; 
}