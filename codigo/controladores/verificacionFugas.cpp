#include "validaciones/entradas.h"
#include "clases/tanque.h"
#include "controladores/verificacionFugas.h"

void menuVerificacion(QSqlDatabase& db);

void verificacionFugas() {
    conexion bdObject;
    QSqlDatabase& db = bdObject.getBd();

    menuVerificacion(db);

    //bdObject.~conexion();

}

void menuVerificacion(QSqlDatabase &db) {

    cout << "-------------------------" << endl;
    cout << "| Verificacion de fugas |" << endl;
    cout << "-------------------------" << endl;


    unsigned int tamano;

    estacion estacion(db);

    unsigned int* estaciones = estacion.obtenerEstaciones(tamano);

    if (estaciones != nullptr) {

        unsigned int idEstacion;

        while (true) {
            cout << "Ingrese el ID de la estacion a verificar la fuga: ";
            cin >> idEstacion;

            if (!validarCin()) continue;  // Validar entrada
            if (!validarPositivo(idEstacion)) continue;  // Validar que el número sea positivo
            if (validarNumeroEnArreglo(estaciones, tamano, idEstacion)) {
                break;  // Si es válido, salir del ciclo
            } else {
                cout << "ID no válido. Intente nuevamente." << endl;
            }
        }

        estacion.setId(idEstacion);

        Tanque tanque(&estacion, &db);
        tanque.verificarFugas();

    } else {
        cout << "Error al obtener las estaciones." << endl;
    }

}
