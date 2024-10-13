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

    estacion estacionActual(db);
    unsigned int tamEstaciones;

    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    // Seleccionar estación por ID
    unsigned int idEstacionSeleccionada;

    while (true) {
        cout << "Ingrese el ID de la estacion a la que desea agregar un surtidor: ";
        cin >> idEstacionSeleccionada;

        if (!validarCin()) continue;
        break;
    }

    // Verificar que el ID ingresado esté en la lista de estaciones
    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacionSeleccionada) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacionSeleccionada);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "ID de estacion no encontrado." << endl;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    string modelo;
    bool activo = true;

    while (true) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ingrese el modelo del surtidor: ";
        getline(cin, modelo);

        if (!validarVarchar(100, modelo)) continue;
        break;
    }


    Surtidor surtidor(db, estacionActual);

    surtidor.setModelo(modelo);
    surtidor.setActivo(activo);

    try {

        unsigned int idSurtidor = surtidor.agregarSurtidor();
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

    delete[] estacionesDisponibles;

}
