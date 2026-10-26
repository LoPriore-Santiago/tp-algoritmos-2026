#include <iostream>
#include <cstdio>    
#include <string> 
using namespace std;

/*cierre.cpp Junta las planillas de todos los días de la semana en una sola
planilla semanal, ordenada por mozo.
comandas_dd-mm-aaaa.dat (varios) →
comandas_semana_sX-mm.dat
*/
struct comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const int MAX_ARCHIVOS = 7;

int main(){

FILE*archivos[MAX_ARCHIVOS];
comanda actuales[MAX_ARCHIVOS];
bool hayDatos[MAX_ARCHIVOS];

int cantarchivos;
cout << "ingrese la cantidad de archivos diarios" << endl;
cin >> cantarchivos;

if (cin.fail() || cantarchivos <= 0 || cantarchivos > MAX_ARCHIVOS){ 
    cout << "Cantidad de archivos invalida" << endl;
    return 1;
}

// abrir varios archivos diarios 

for (int i=0; i<cantarchivos;i++){
    char nombreArchivo[40];

    cout << "ingrese el nombre del archivo diario";
    cin.width(sizeof(nombreArchivo));
    cin >> nombreArchivo;

    archivos[i]=fopen(nombreArchivo, "rb" );

    if(archivos[i]==NULL){
        cout << "error: no se pudo abrir el archivo" << endl;
        for(int j=0;j<i;j++){
            fclose(archivos[j]);
        }

        return 1;

 }

//leer la primera comanda de cada archivo
hayDatos[i]= fread(&actuales[i],sizeof(comanda),1,archivos[i])==1;
}

// Pedir semana y mes para generar el archivo semanal
int semana,mes;
string nombreSemanal;

cout << "Ingrese el numero de semana: ";
if (!(cin >> semana)) {
    cout << "Semana invalida" << endl;

    for(int i=0;i<cantarchivos;i++){
        fclose(archivos[i]);
    }
    return 1;
}
cout << "Ingrese el mes: ";
if (!(cin >> mes)) {
    cout << "Mes invalido" << endl;

    for(int i=0;i<cantarchivos;i++){
        fclose(archivos[i]);

    }
    return 1;
}
if (semana < 1 || mes < 1 || mes > 12) {
    cout << "Error: semana o mes invalido" << endl;
    
    for (int i=0;i<cantarchivos;i++){
        fclose(archivos[i]);
    }
    return 1;
}

nombreSemanal = "comandas_semana_s" + to_string(semana) + "-" + (mes<10? "0" : "") + to_string(mes)+ ".dat";



FILE *out = fopen(nombreSemanal.c_str(), "wb");
if (out == NULL) {
    cout << "Error al crear el archivo semanal" << endl;

    for(int i=0;i<cantarchivos;i++){
        fclose(archivos[i]);
    }
    return 1;
}
//apareo de multiples archivos
while(true){
    int menor=-1;
    //buscar comanda con menor ID de mozo
    for(int i=0;i<cantarchivos;i++){
        if(hayDatos[i]){

            if(menor ==-1 || actuales[i].idMozo < actuales[menor].idMozo){
                menor = i ;
            }
        }
    }

    // Si no quedan comandas en ningun archivo, termina el apareo
    if(menor ==-1){
        break;
    }

    //escribir comanda seleccionada

    if(fwrite(&actuales[menor],sizeof(comanda),1,out)!=1){
        cout << "error al escribir la comanda" << endl;
        fclose(out);

        for(int i=0; i<cantarchivos;i++){
            fclose(archivos[i]);
        }
        return 1;
    }

    //avanzar solamente en el archivo seleccionado

    hayDatos[menor]=fread(&actuales[menor],sizeof(comanda),1,archivos[menor])==1;

}  
// Cerrar los archivos diarios
for (int i = 0; i < cantarchivos; i++) {
     fclose(archivos[i]);
}

fclose(out);
cout << "cierre semanal generado " << endl;

return 0;
}