#include "conexion/conexion.h"
#include "validaciones/entradas.h"
#include "controladores/gestionRed.h"
#include "controladores/gestionEstaciones.h"
#include "controladores/simulacionVenta.h"

void inicio();


template <typename T> T myMax(T x, T y)
{
    return (x > y) ? x : y;
}

int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);

    inicio();

    return 0;
}


void inicio(){
    //Inicio

    cout << "-----------------------------------" << endl;
    cout << "|               TERMAX             |" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Inicio" << endl;
    cout << "------" << endl;
    cout << "Opciones disponibles. " << endl;
    cout << "1. Gestion de red." << endl;
    cout << "2. Gestion de estaciones de servicio." << endl;
    cout << "3. Simular venta" << endl;


    short opcion;
    while (true){
        cout << "Ingrese la opcion: ";
        cin >> opcion;

        if (!validarCin()) continue;
        if (!validarPositivo(opcion)) continue;
        if (!validarRango(1, 3, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        gestionRed();
        break;
    case 2:
        gestionEstaciones();
        break;
    case 3:
        simulacionVenta();
        break;
    default:
        break;
    }

    inicio();
    return;
}
