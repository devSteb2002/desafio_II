#include "validaciones/entradas.h"
#include "clases/estacion.h"

void menuSimluacion(QSqlDatabase& db);

void simulacionVenta(){
    conexion bdObject; // instanciar la clase conexion
    QSqlDatabase& db = bdObject.getBd(); // obtener el objeto db

    menuSimluacion(db);

    bdObject.~conexion();
}

void menuSimluacion(QSqlDatabase& db){

    cout << "-------------------------------------" << endl;
    cout << "|         SIMULACION DE VENTAS      |" << endl;
    cout << "-------------------------------------" << endl;

    unsigned int tamaño;

    estacion estacion(db);
    unsigned int *idsEstaciones = estacion.obtenerEstaciones(tamaño);

    if (idsEstaciones != nullptr){

        unsigned int id;
        while (true){ //pedir codigo
            cout << "Ingrese un codigo de estacion: ";
            cin >> id;

            if (!validarCin()) continue;
            if (!validarPositivo(id)) continue;
            if (!validarNumeroEnArreglo(idsEstaciones, tamaño ,id)) continue; // validar que el codigo exista

            break;
        }



    }



}
