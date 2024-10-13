#include "controladores/gestionEstaciones.h"
#include "clases/surtidor.h"
#include "validaciones/entradas.h"
#include "clases/estacion.h"

void agregarSurtidor(QSqlDatabase& db);
void menuEstaciones(QSqlDatabase& db);

void gestionEstaciones() {

    conexion bdObject;
    QSqlDatabase& db = bdObject.getBd();

    menuEstaciones(db);
}

void menuEstaciones(QSqlDatabase &db) {

    cout << "-------------------------------------" << endl;
    cout << "| Gestion de estaciones de servicio |" << endl;
    cout << "-------------------------------------" << endl;
    cout << "1. Agregar surtidor." << endl;

    short opcion;

    while(true) {
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        if (!validarCin()) continue;
        if (!validarPositivo(opcion)) continue;
        if (!validarRango(1, 1, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        agregarSurtidor(db);
        break;
    default:
        break;
    }

}

void agregarSurtidor(QSqlDatabase &db) {
    string modelo;
    unsigned int idEstacion;
    bool activo = true, validarModelo = false;

    while (true) {
        if (!validarModelo) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ingrese el modelo del surtidor: ";
            getline(cin, modelo);

            if (!validarVarchar(100, modelo)) continue;
            validarModelo = true;
        } else  {
            cout << "Ingrese la estacion a la que pertenece (id de la estacion): ";
            cin >> idEstacion;

            if (!validarCin()) continue;
            break;
        }
    }

    estacion estacionActual(db);
    Surtidor surtidor(db, estacionActual);

    surtidor.setModelo(modelo);
    surtidor.setActivo(activo);

    try {

        unsigned int idSurtidor = surtidor.agregarSurtidor(idEstacion);
        surtidor.setId(idSurtidor);

        if (surtidor.getId() != 0) {
            cout << "Surtidor creado con exito." << endl;
            menuEstaciones(db);
        } else {
            cout << "No se pudo crear el surtidor, vuelve a intentar." << endl;
            menuEstaciones(db);
        }

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

}
