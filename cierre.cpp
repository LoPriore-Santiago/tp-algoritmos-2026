#include <iostream>
#include <cstdio>    
#include <vector>    
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
int main(){

vector<comanda> todas; 

//ejemplo : abrir varios archuvis diarios 
int cantarchivos;
cout << "ingrese la cantidad de archivos diarios" << endl;
cin >> cantarchivos;

for (int i=0; i<cantarchivos;i++){
    char nombreArchivo[40];

    cout << "ingrese el nombre del archivo diario";
    cin.width(sizeof(nombreArchivo));
    cin >> nombreArchivo;

    FILE * f =fopen(nombreArchivo,"rb");
    if(f!=NULL){
        comanda c;
        while (fread(&c,sizeof(comanda),1,f)==1){
            todas.push_back(c);
        }
     fclose(f);
    }
    else {
    cout << "Error: no se pudo abrir el archivo" << endl;
    return 1;
    }
}
//ordenar por mozo, metodo de burbuja 
int n= todas.size();

for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {

        if (todas[j].idMozo > todas[j + 1].idMozo) {

            comanda aux = todas[j];
            todas[j] = todas[j + 1];
            todas[j + 1] = aux;

        }
    }
}
//guardar en archivo semanal
char nombreSemanal[40];

cout << "Ingrese el nombre del archivo semanal: ";
cin.width(sizeof(nombreSemanal));
cin >> nombreSemanal;

FILE *out = fopen(nombreSemanal, "wb");

if (out == NULL) {
    cout << "Error al crear el archivo semanal" << endl;
    return 1;
}
for (auto &c : todas) {
    if (fwrite(&c, sizeof(comanda), 1, out) != 1) {
        cout << "Error al escribir la comanda" << endl;
        fclose(out);
        return 1;
    }
}

fclose(out);
cout << "cierre semanal generado " << endl;

return 0;
}