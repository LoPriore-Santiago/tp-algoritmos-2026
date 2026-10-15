#include <iostream>
#include <cstdio>
#include <cstring>


using namespace std;

// Estructuras
struct Producto{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int id;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const float TASA_COMISION = 0.10f;

// Funciones auxiliares 

// Retorna el codigo del producto buscado, usamos busqueda binaria debido a que en el enunciado especifica que faltan codigos en el medio.(no podemos usar PUP)
long buscarProducto( const char* nombreArchivo, int codigoProducto, Producto& producto){

    FILE* f =  fopen(nombreArchivo, "rb");
    
    if (f == NULL){
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(Producto);       // cantidad de productos que hay en ""inventario.dat"
    long inicio = 0;                           
    long fin = n - 1;
    long pos = -1;

        while(inicio <= fin && pos == -1){
            long mitad = (inicio + fin) / 2;
            fseek(f, mitad * sizeof(Producto), SEEK_SET);   // posicionamos el puntero al medio
            fread(&producto, sizeof(Producto), 1, f);  
                if(producto.codigo == codigoProducto){
                    pos = mitad;
                }else if(producto.codigo < codigoProducto){   
                    inicio = mitad + 1;
                }else{
                    fin = mitad - 1;
                }
        }
        fclose(f);
        return pos;
};

//Pedimos el id del mozo y buscamos si existe
bool buscarMozo(const char* nombreArchivo, int idMozo, Mozo& mozo){
    FILE* f = fopen(nombreArchivo, "rb");

    if (f == NULL){
        return false;
    }

    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(Mozo);  // cantidad de mozos queh hay en "mozos.dat"

    long inicio = 0;
    long fin = n - 1;
    bool encontrado = false;

        while(inicio <= fin && !encontrado){
            long mitad = (inicio + fin) / 2;

            fseek(f, mitad * sizeof(Mozo), SEEK_SET);   // posicionamos el puntero al medio
            fread(&mozo, sizeof(Mozo), 1, f);  
                if(mozo.id == idMozo){
                    encontrado = true;
                }else if(mozo.id < idMozo){   
                    inicio = mitad + 1;
                }else{
                    fin = mitad - 1;
                }
        }
        fclose(f);
        return encontrado;
};

bool actualizarStock(long posicion, int cantidadVendida){

    FILE* inventario = fopen("inventario.dat", "rb+");

    if(inventario == NULL){
        return false;
    }

    Producto producto;

    fseek(inventario, posicion * sizeof(Producto), SEEK_SET);

    if(fread(&producto, sizeof(Producto), 1, inventario) != 1) {
        fclose(inventario);
        cout << "Error al leer el producto del inventario" << endl;
        return false;
    }
    // caso en que se vende una cantidad mayor al stock disponible
    if(cantidadVendida > producto.stockActual){
        fclose(inventario);
        cout << "Error: No hay stock suficiente. Stock disponible: " << producto.stockActual << endl;
        return false;
    }
    producto.stockActual = producto.stockActual - cantidadVendida;

    fseek(inventario, posicion * sizeof(Producto), SEEK_SET);
    
    if(fwrite(&producto, sizeof(Producto), 1, inventario) != 1){
        fclose(inventario); 
        cout << "Error al actualizar stock" << endl;
        return false;
    }

    fclose(inventario);
    return true;

}

// Ordenamos la planilla por mozo con Ordenamiento Burbuja
bool ordenarPlanilla(const char* nombreArchivo) {

    FILE* f = fopen(nombreArchivo, "rb+");  //rb+ para leer las comandas, comparar sus datos y escribirlas nuevamente cuando haya que intercambiarlas.

    if(f == NULL) {
        return false;
    }

    fseek(f, 0, SEEK_END);
    long cantidad = ftell(f) / sizeof(Comanda);  // ver cantidad de comandas

    for(long i = 0; i < cantidad - 1; i++) {                   // recorre las pasadas del burbujeo
        for(long j = 0; j < cantidad - i - 1; j++) {           // recorre las comandas que hay que comparar 

            Comanda actual;
            Comanda siguiente;

            fseek(f, j * sizeof(Comanda), SEEK_SET);          // nos posiciona en el registro que queremos leer
            fread(&actual, sizeof(Comanda), 1, f);            // copia ese registro a una variable de tipo Comanda.

            fseek(f, (j + 1) * sizeof(Comanda), SEEK_SET);
            fread(&siguiente, sizeof(Comanda), 1, f);

            if(actual.idMozo > siguiente.idMozo){

                fseek(f, j * sizeof(Comanda), SEEK_SET);
                fwrite(&siguiente, sizeof(Comanda), 1, f);

                fseek(f, (j + 1) * sizeof(Comanda), SEEK_SET);
                fwrite(&actual, sizeof(Comanda), 1, f);
            }
        }
    }
    fclose(f);
    return true;

}




float calcularComision(float precio, int cantidad)
{
    return precio * cantidad * TASA_COMISION;
};

void encriptar(char password[]) {   // 
    int K=2;  
    int i=0;
    while (password[i] != '\0') {
        password[i] = password[i] + K; 
        i++;
    }
}

// compara 2 cadenas de caracteres y retorna true si son iguales, false si son distitntas
bool validarPassword(const char passwordIngresada[], const char passwordGuardada[]) {

    return strcmp(passwordIngresada, passwordGuardada) == 0;
}



int main() {
    // Pedir fecha
    char fecha[20];

    cout << "Ingrese la fecha del dia (dd-mm-aaaa): ";
    cin.width(sizeof(fecha));
    cin >> fecha;

    // Generar nombre de la planilla
    char nombreComanda[40];
    sprintf(nombreComanda, "comandas_%s.dat", fecha);

    // Abrir o crear la planilla para agregar ventas
    FILE* f = fopen(nombreComanda, "ab");  // usamos ab ya que permite agregar registros sin borrar los que ya existen

    if (f == NULL) {
        cout << "No se pudo abrir o crear la planilla" << endl;
        return 1;
    }


    char opcion = '1';

    while (opcion != '0') {
        cout << "Ingrese 1 para cargar una venta" << endl;
        cout << "Ingrese 0 para terminar" << endl;
        cin >> opcion;

        if (opcion == '1') {
            int idMozo;

            cout << "Ingrese el ID del mozo: ";
            cin >> idMozo;

            Mozo mozo;

            if (!buscarMozo("mozos.dat", idMozo, mozo)) {
                cout << "Mozo no encontrado" << endl;
            }
            else {
                char passwordIngresada[20];

                cout << "Ingrese su contraseña: ";
                cin.width(sizeof(passwordIngresada));
                cin >> passwordIngresada;

                encriptar(passwordIngresada);

                if (!validarPassword(passwordIngresada, mozo.password)) {
                    cout << "Contraseña incorrecta. Acceso denegado" << endl;
                }
                else {
                    cout << "Contraseña correcta. Acceso autorizado" << endl;

                    int codigoProducto;

                    cout << "Ingrese el codigo del producto: ";
                    cin >> codigoProducto;

                    Producto producto;

                    long posicionProducto = buscarProducto("inventario.dat", codigoProducto, producto);

                    if (posicionProducto == -1) {
                        cout << "Producto no encontrado" << endl;
                    }
                    else if (producto.stockActual <= 0) {
                        cout << "Producto sin stock" << endl;
                    }
                    else {
                        cout << "Producto: " << producto.descripcion << endl;
                        cout << "Precio: " << producto.precio << endl;
                        cout << "Stock disponible: " << producto.stockActual << endl;

                        int cantidad;

                        cout << "Ingrese la cantidad vendida: ";
                        cin >> cantidad;

                        if (cantidad <= 0) {
                            cout << "Cantidad invalida" << endl;
                        }
                        else if (cantidad > producto.stockActual) {      // evitamos que se venda mas de lo que hay
                            cout << "No hay stock suficiente" << endl;
                        }
                        else {
                            float comision = calcularComision(producto.precio, cantidad);

                            // Actualizamos el stock antes de guardar la comanda.
                            if (actualizarStock(posicionProducto, cantidad)) {

                                Comanda nuevaComanda;

                                nuevaComanda.idMozo = idMozo;
                                nuevaComanda.codigoProducto = codigoProducto;
                                nuevaComanda.cantidad = cantidad;
                                nuevaComanda.comision = comision;

                                if (fwrite(&nuevaComanda,
                                        sizeof(Comanda), 1, f) == 1) {
                                    cout << "Comanda registrada correctamente" << endl;
                                    cout << "Comision generada: " << comision << endl;
                                }
                                else {
                                    cout << "Error al registrar la comanda" << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (opcion != '0') {
            cout << "Opcion invalida. Ingrese 1 o 0." << endl;
        }
    }


    fclose(f);

    if(ordenarPlanilla(nombreComanda)) {
        cout << "Planilla ordenada correctamente" << endl;
    }
    else {
        cout << "Error al ordenar la planilla" << endl;
    }

    cout << "Carga de ventas finalizada" << endl;

    return 0;
}