#include "controladores/gestionEstaciones.h"
#include "clases/surtidor.h"
#include "validaciones/entradas.h"
#include "clases/estacion.h"
#include "clases/venta.h"
#include "clases/nave.h"
#include "clases/tanque.h"
void agregarSurtidor(QSqlDatabase& db);
void eliminarSurtidor(QSqlDatabase& db);
void desactivarSurtidor(QSqlDatabase& db);
void activarSurtidor(QSqlDatabase& db);
void historicoTransacciones(QSqlDatabase& db);
void litrosVendidosPorCategoria(QSqlDatabase& db);
void asignarCapacidadAleatoriaDelTanque(QSqlDatabase& db);
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
    cout << "2. Eliminar surtidor." << endl;
    cout << "3. Desactivar surtidor." << endl;
    cout << "4. Activar surtidor." << endl;
    cout << "5. Historico de transacciones." << endl;
    cout << "6. Ver litros de combustible vendido por categorias." << endl;
    cout << "7. Asignar aleatoriamente la capacidad del tanque de suministro." << endl;
    cout << "8. Salir" << endl;

    short opcion;

    while(true) {
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        if (!validarCin()) continue;
        if (!validarPositivo(opcion)) continue;
        if (!validarRango(1, 8, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        agregarSurtidor(db);
        break;
    case 2:
        eliminarSurtidor(db);
        break;
    case 3:
        desactivarSurtidor(db);
        break;
    case 4:
        activarSurtidor(db);
        break;
    case 5:
        historicoTransacciones(db);
        break;
    case 6:
        litrosVendidosPorCategoria(db);
        break;
    case 7:
        asignarCapacidadAleatoriaDelTanque(db);
        break;
    case 8:
        return;
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

void eliminarSurtidor(QSqlDatabase &db) {
    estacion estacionActual(db);
    unsigned int tamEstaciones;

    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    unsigned int idEstacionSeleccionada;

    while (true) {
        cout << "Ingrese el ID de la estacion de la cual desea eliminar un surtidor: ";
        cin >> idEstacionSeleccionada;

        if (!validarCin()) continue;
        break;
    }

    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacionSeleccionada) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacionSeleccionada);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "ID de estacion no encontrado. Intentelo de nuevo." << endl;
        delete[] estacionesDisponibles;
        return;
    }

    Surtidor surtidor(db, estacionActual);
    unsigned int tamSurtidores;
    unsigned int* surtidoresDisponibles = surtidor.obtenerSurtidores(tamSurtidores);

    unsigned int id;
    while (true) {
        cout << "Ingrese el ID del surtidor a eliminar: ";
        cin >> id;

        if (!validarCin()) continue;
        break;
    }

    bool surtidorEncontrado = false;
    for (unsigned int i = 0; i < tamSurtidores; i++) {
        if (surtidoresDisponibles[i] == id) {
            surtidorEncontrado = true;
            surtidor.setId(id);
            break;
        }
    }

    if (!surtidorEncontrado) {
        cout << "Surtidor no encontrado. Intenta con otro surtidor." << endl;
        delete[] estacionesDisponibles;
        delete[] surtidoresDisponibles;
        menuEstaciones(db);
        return;
    }


    try {
       bool eliminado = surtidor.eliminarSurtidor();

        if (eliminado) {
           cout << "Surtidor eliminado con exito." << endl;
           menuEstaciones(db);
        } else {
            cout << "No se pudo eliminar el surtidor. Vuelve a intentarlo" << endl;
            menuEstaciones(db);
        }

    } catch(const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    delete[] estacionesDisponibles;
    delete[] surtidoresDisponibles;
}

void desactivarSurtidor(QSqlDatabase &db) {

    estacion estacionActual(db);

    unsigned int tamEstaciones;
    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    unsigned int idEstacion;
    while(true) {
        cout << "Ingrese el ID de la estacion de la cual deseas desactivar un surtidor.";
        cin >> idEstacion;

        if (!validarCin()) continue;
        break;
    }

    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacion) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacion);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "Estacion no encontrado." << endl;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    Surtidor surtidor(db, estacionActual);
    unsigned int tamSurtidores;
    unsigned int* surtidoresDisponibles = surtidor.obtenerSurtidores(tamSurtidores);

    if (surtidoresDisponibles == nullptr) {
        menuEstaciones(db);
        return;
    }

    unsigned int id;
    while(true) {
        cout << "Ingrese el id del surtidor a desactivar: ";
        cin >> id;

        if (!validarCin()) continue;
        break;
    }

    bool surtidorEncontrado = false;
    for (unsigned int i = 0; i < tamSurtidores; i++) {
        if (surtidoresDisponibles[i] == id) {
            surtidorEncontrado = true;
            surtidor.setId(id);
            break;
        }
    }

    if (!surtidorEncontrado) {
        cout << "Surtidor no encontrado." << endl;
        delete[] surtidoresDisponibles;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    surtidor.desactivarSurtidor();
    menuEstaciones(db);

    delete[] estacionesDisponibles;
    delete[] surtidoresDisponibles;
}

void activarSurtidor(QSqlDatabase &db) {
    estacion estacionActual(db);

    unsigned int tamEstaciones;
    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    unsigned int idEstacion;
    while(true) {
        cout << "Ingrese el ID de la estacion de la cual deseas activar un surtidor.";
        cin >> idEstacion;

        if (!validarCin()) continue;
        break;
    }

    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacion) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacion);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "Estacion no encontrado." << endl;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    Surtidor surtidor(db, estacionActual);
    unsigned int tamSurtidores;
    unsigned int* surtidoresDisponibles = surtidor.obtenerSurtidores(tamSurtidores);

    if (surtidoresDisponibles == nullptr) {
        menuEstaciones(db);
        return;
    }

    unsigned int id;
    while(true) {
        cout << "Ingrese el id del surtidor a activar: ";
        cin >> id;

        if (!validarCin()) continue;
        break;
    }

    bool surtidorEncontrado = false;
    for (unsigned int i = 0; i < tamSurtidores; i++) {
        if (surtidoresDisponibles[i] == id) {
            surtidorEncontrado = true;
            surtidor.setId(id);
            break;
        }
    }

    if (!surtidorEncontrado) {
        cout << "Surtidor no encontrado." << endl;
        delete[] surtidoresDisponibles;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    surtidor.activarSurtidor();
    menuEstaciones(db);

    delete[] estacionesDisponibles;
    delete[] surtidoresDisponibles;
}

void historicoTransacciones(QSqlDatabase &db) {
    estacion estacionActual(db);

    unsigned int tamEstaciones;
    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    unsigned int idEstacion;
    while(true) {
        cout << "Ingrese el ID de la estacion de la cual deseas buscar un surtidor: ";
        cin >> idEstacion;

        if (!validarCin()) continue;
        break;
    }

    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacion) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacion);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "Estacion no encontrado." << endl;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    Surtidor surtidor(db, estacionActual);
    unsigned int tamSurtidores;
    unsigned int* surtidoresDisponibles = surtidor.obtenerSurtidores(tamSurtidores);

    if (surtidoresDisponibles == nullptr) {
        menuEstaciones(db);
        return;
    }

    unsigned int id;
    while(true) {
        cout << "Ingrese el ID del surtidor a consultar el historico de transacciones: ";
        cin >> id;

        if (!validarCin()) continue;
        break;
    }

    bool surtidorEncontrado = false;
    for (unsigned int i = 0; i < tamSurtidores; i++) {
        if (surtidoresDisponibles[i] == id) {
            surtidorEncontrado = true;
            surtidor.setId(id);
            break;
        }
    }

    if (!surtidorEncontrado) {
        cout << "Surtidor no encontrado." << endl;
        delete[] surtidoresDisponibles;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    cout << endl;

    surtidor.historicoTransacciones();
    menuEstaciones(db);

    delete[] estacionesDisponibles;
    delete[] surtidoresDisponibles;
}

void litrosVendidosPorCategoria(QSqlDatabase &db) {

    estacion est(db);

    Surtidor s(db, est);
    Venta v(s, db);

    v.litrosDeCombustibleVendidosPorCategoria();
}

void asignarCapacidadAleatoriaDelTanque(QSqlDatabase &db) {
    estacion estacionActual(db);

    unsigned int tamEstaciones;
    unsigned int* estacionesDisponibles = estacionActual.obtenerEstaciones(tamEstaciones);

    if (tamEstaciones == 0) {
        cout << "No hay estaciones disponibles." << endl;
        menuEstaciones(db);
        return;
    }

    unsigned int idEstacion;
    while(true) {
        cout << "Ingrese el ID de la estacion de la cual deseas asignar la capacidad del tanque: ";
        cin >> idEstacion;

        if (!validarCin()) continue;
        break;
    }

    bool estacionEncontrada = false;
    for (unsigned int i = 0; i < tamEstaciones; i++) {
        if (estacionesDisponibles[i] == idEstacion) {
            estacionEncontrada = true;
            estacionActual.setId(idEstacion);
            break;
        }
    }

    if (!estacionEncontrada) {
        cout << "Estacion no encontrado." << endl;
        delete[] estacionesDisponibles;
        menuEstaciones(db);
        return;
    }

    Tanque tanque(&estacionActual, &db);

    cout << endl;

    tanque.asignarCapacidadAleatoriaDelTanque();

    cout << endl;

    menuEstaciones(db);

    delete[] estacionesDisponibles;

}
