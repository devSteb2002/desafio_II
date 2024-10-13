#include "controladores/gestionRed.h"
#include "clases/surtidor.h"
#include "validaciones/entradas.h"
// #include "clases/red.h"
#include "clases/estacion.h"
#include "clases/tanque.h"
#include "clases/region.h"
#include "clases/venta.h"

void agregarEstacionDeServicio(QSqlDatabase& db);
void eliminarEstacionDeServicio(QSqlDatabase& db);
void menu(QSqlDatabase& db);

void gestionRed(){ //inicio del controlador

    //conexion persistente en este controlador
    conexion bdObject; // instanciar la clase conexion
    QSqlDatabase& db = bdObject.getBd(); // obtener el objeto db

    menu(db);
}


void menu(QSqlDatabase& db){

    cout << "----------------------" << endl;
    cout << "| Opciones de la red |" << endl;
    cout << "----------------------" << endl;
    cout << "1. Agregar estaciones de servicio." << endl;
    cout << "2. Eliminar una estacion de servicio." << endl;
    cout << "3. Calcular el monto total de ventas en cada E/S del pais." << endl;
    cout << "4. Fijar los precios del combustible." << endl;

    short opcion;

    while (true){
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        if (!validarCin()) continue;
        if (!validarPositivo(opcion)) continue;
        if (!validarRango(1, 4, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        agregarEstacionDeServicio(db);
        break;
    case 2:
        eliminarEstacionDeServicio(db);
        break;
    default:
        break;
    }
}

void agregarEstacionDeServicio(QSqlDatabase& db){ //agregar estaciones de servicio

    cout << "Ingrese lo datos a continucacion." << endl;

    string nombre;
    float longitud, latitud;
    unsigned short region;
    bool prin = true, validoNombre = false, validaRegion = false, validaLatitud = false;


    unsigned short *ids = nullptr;
    string *nombres = nullptr;
    unsigned short tamaño;

    Region reg(db);
    reg.obtenerRegiones(ids, nombres, tamaño);


    while (true){

        if (prin) cin.ignore(numeric_limits<streamsize>::max(), '\n');
        prin = false;

        if (!validoNombre){ // validar nombre de la estacion
            cout << "Ingrese el nombre de la estacion: ";
            getline(cin, nombre);

            if (!validarVarchar(60, nombre)) continue;
            validoNombre = true;
        }
        else if (!validaRegion){ //validar region de la estacion

            cout << "Seleccione una region." << endl;
            for (unsigned short i = 0; i < tamaño; i++) cout << ids[i] <<  " " << nombres[i] << endl;
            cout << "Digite el codigo: ";
            cin >> region;

            if (!validarCin()) continue;
            if (!validarNumeroEnArreglo(ids, tamaño, region)) continue;

            validaRegion = true;
        }
        else if (!validaLatitud) { // validar latitud
            cout << "Ingrese la latitud: ";
            cin >> latitud;

            if (!validarCin()) continue;

            validaLatitud = true;

        }else{ //validar longitud
            cout << "Ingrese la longitud: ";
            cin >> longitud;

            if (!validarCin()) continue;

            break;

        }
    }

    delete[] ids;
    delete[] nombres;

    estacion est(db);

    est.setNombre(nombre);
    est.setLatitud(latitud);
    est.setLongitud(longitud);

    unsigned int idCreado = est.crearEstacion(region);
    est.setId(idCreado);

    try {
        if (est.getId() != 0){

            Tanque tanq_(&est, &db);
            tanq_.crearTanque();

            cout << "Estacion creada con exito." << endl;

            menu(db);
            return;

        }else{
            cout << "No se puedo crear la estacion, vuelva a intentar." << endl;
        }

    } catch (const exception &e) {
        cout << "Error" << e.what() << endl;
    }

}


void eliminarEstacionDeServicio(QSqlDatabase& db){ // eliminar una estacion de servicio

    unsigned int tamaño;
    unsigned int id;

    estacion est_(db); //instancia estacion
    unsigned int *ids = est_.obtenerEstaciones(tamaño); // tamño por referencia se obtiene un array con los id

    while (true){ //pedir codigo
        cout << "Ingrese un codigo de estacion: ";
        cin >> id;

        if (!validarCin()) continue;
        if (!validarPositivo(id)) continue;
        if (!validarNumeroEnArreglo(ids, tamaño ,id)) continue; // validar que el codigo exista

        break;
    }

    est_.setId(id);
    Surtidor surt(db, est_); //instancia surtidor
    int cantidad = surt.obtenerCantidadActivos(); // obtener surtidores activos

    if (cantidad == -1){
        cout << "Error al traer los surtidores." << endl;
        est_.~estacion();
        surt.~Surtidor();
        delete[] ids;
        menu(db);
        return;
    }


    if (cantidad > 0){ // cuando no se puede eliminar por tener surtidores activos
        cout << "No se puede eliminar esta estacion, porque tiene surtidores activos." << endl;
        est_.~estacion();
        surt.~Surtidor();
        delete[] ids;
        menu(db);
        return;

    }else{ // se puede eliminar la estacion
        //nota: progresivamente se tiene que eliminar surtidor y tanque y despues estacion

        // Tanque tanq(&est_, &db);
        // Venta vent(surt, db);
        // if (tanq.eliminarTanque()){
        //      surt.eliminarSurtidor(vent);
        // }

        // est_.eliminarEstacion();

        // tanq.~Tanque();
    }


}
