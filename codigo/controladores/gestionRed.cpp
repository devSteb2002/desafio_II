#include "controladores/gestionRed.h"
#include "clases/surtidor.h"
#include "validaciones/entradas.h"
#include "clases/red.h"
#include "clases/estacion.h"
#include "clases/tanque.h"
#include "clases/nave.h"
#include "clases/region.h"
#include "clases/venta.h"
#include "clases/categoria.h"
#include "clases/combustible.h"

void agregarEstacionDeServicio(QSqlDatabase& db);
void eliminarEstacionDeServicio(QSqlDatabase& db);
void calcularMontoDeVentasPorES(QSqlDatabase& db);
void fijarPreciosCombustibles(QSqlDatabase& db);
void menu(QSqlDatabase& db);

void gestionRed(){ //inicio del controlador

    //conexion persistente en este controlador
    conexion bdObject; // instanciar la clase conexion
    QSqlDatabase& db = bdObject.getBd(); // obtener el objeto db

    menu(db);

    bdObject.~conexion();
}


void menu(QSqlDatabase& db){

    cout << "---------------------------------------------------------" << endl;
    cout << "|                 OPCIONES DE LA RED                    |" << endl;
    cout << "---------------------------------------------------------" << endl;
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
        if (!validarRango(1, 5, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        agregarEstacionDeServicio(db);
        break;
    case 2:
        eliminarEstacionDeServicio(db);
        break;
    case 3:
        calcularMontoDeVentasPorES(db);
        break;
    case 4:
        fijarPreciosCombustibles(db);
        break;
    default:
        break;
    }



}

void agregarEstacionDeServicio(QSqlDatabase& db){ //agregar estaciones de servicio

    cout << "Ingrese lo datos a continuacion." << endl;

    string nombre;
    float longitud, latitud;
    unsigned short region, naves;
    bool prin = true, validoNombre = false, validaRegion = false, validaLatitud = false, validarLongitud = false;


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

        }else if (!validarLongitud){ //validar longitud
            cout << "Ingrese la longitud: ";
            cin >> longitud;

            if (!validarCin()) continue;

            validarLongitud = true;
        }else{
            //validar namves de 1 a 4
            cout << "Ingrese el numero de naves de esta estacion." << endl;
            cin >> naves;

            if (!validarCin()) continue;
            if (!validarRango(1, 4, naves)) continue;

            break;
        }
    }


    delete[] ids;
    delete[] nombres;

    estacion est(db);
    Tanque tanq_(&est, &db);
    Nave nave(db);

    est.setNombre(nombre);
    est.setLatitud(latitud);
    est.setLongitud(longitud);

    try {

        unsigned int idCreado = est.crearEstacion(region);
        est.setId(idCreado);

        if (est.getId() != 0){

            nave.crearNaves(idCreado, naves); // crear naves segun la estacion
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


    if (tamaño > 0){
        while (true){ //pedir codigo
            cout << "Ingrese un codigo de estacion: ";
            cin >> id;

            if (!validarCin()) continue;
            if (!validarPositivo(id)) continue;
            if (!validarNumeroEnArreglo(ids, tamaño ,id)) continue; // validar que el codigo exista

            break;
        }
    }else{
        if (ids != nullptr) delete[] ids;
        cout << "No existen estaciones disponibles." << endl;
        menu(db);
        return;
    }



    delete[] ids;

    est_.setId(id);
    Surtidor surt(db, est_); //instancia surtidor
    Venta vent(surt, db);

    bool eliminar = false;

    if (surt.verificarExistencia()){ // si existen surtidores
        // se eliminan sus ventas y los propios suritodes
        int cantidad = surt.obtenerCantidadActivos(); // obtener surtidores activos

        if (cantidad == -1){
            cout << "Error al traer los surtidores." << endl;
        }else if (cantidad > 0){ // cuando no se puede eliminar por tener surtidores activos
            cout << "No se puede eliminar esta estacion, porque tiene surtidores activos." << endl;
        }else{ // eliminar surtidores y ventas

            bool surtidoresEliminados = surt.eliminarSurtidores(vent);

            if (!surtidoresEliminados){ // se eliminaron las ventas y los surtidores correctamente
                cout << "Error al eliminar los surtidores." << endl;
            }else {
                eliminar = true;
            }
        }
    }else eliminar = true;


    if (eliminar){
        //eliminar tanque y estacion
        Tanque tanq(&est_, &db);

        if (tanq.eliminarTanque()){ // eliminar tanque
            if (est_.eliminarEstacion()){
                cout << "La estacion " << est_.getId() << " fue eliminada con exito" << endl;
            }else{
                cout << "La estacion " << est_.getId() << " no se pudo eliminar " << endl;
            }
        }
    }

    menu(db);
}


void calcularMontoDeVentasPorES(QSqlDatabase& db){ // calcular el monto de ventas

    unsigned int tamaño;

    red red(db);

    estacion est(db);
    Surtidor surt(db, est);
    Venta vent(surt, db);

    unsigned int idRed = red.getIdRed();

    unsigned int *idsEstaciones = est.obtenerEstaciones(tamaño, false);

    if (tamaño == 0){ // no hay estaciones
        cout << "No hay estaciones para calcular la venta." << endl;

        if (idsEstaciones != nullptr) delete[] idsEstaciones;

        menu(db);
        return;

    }else{

        Categoria categoria(db);
        vent.calcularVentasPorES(idRed, categoria);

        menu(db);
        return;

    }
}

void fijarPreciosCombustibles(QSqlDatabase& db){ // fijar precios de combustible segun categoria y segun region

    cout << "-----------------------------------------------" << endl;
    cout << "|   FIJAR PRECIOS DE COMBUSTIBLE POR REGION    |" << endl;
    cout << "-----------------------------------------------" << endl;

    string *nombreRegiones = nullptr;
    string *nombresCategorias = nullptr;
    unsigned short *idsRegiones = nullptr;
    unsigned short *idsCategorias = nullptr;
    unsigned short tamañoRegion;
    unsigned short tamañoCategoria;

    float precioLitro;

    Region region(db);
    Categoria categoria(db);
    Combustible combustible(db);

    region.obtenerRegiones(idsRegiones, nombreRegiones, tamañoRegion);
    categoria.obtenerCategorias(nombresCategorias, idsCategorias, tamañoCategoria);

    for (unsigned int i = 0; i < tamañoRegion; i++){ // iniciar con las regiones
        cout << "Region: " << nombreRegiones[i] << endl;
        cout << "----------" << endl;

        for (unsigned int j = 0; j < tamañoCategoria; j++){
            cout << "Tipo de combustible: " << nombresCategorias[j] << endl;

            while (true){ // validar el ingreso el datos
                cout << "Ingrese el precio por litro: ";
                cin >> precioLitro;

                if (!validarCin()) continue;
                if (precioLitro < 0.0){
                    cout << "El precio debe ser positivo, vuelva a intentar." << endl;
                    continue;
                }

                break;
            }

            combustible.setValorLitro(precioLitro);
            combustible.guardarCombustible(idsRegiones[i], idsCategorias[j]);
        }
    }

    cout << "Precios fijados correctamente." << endl;

    if (idsCategorias != nullptr) delete[] idsCategorias;
    if (idsRegiones != nullptr) delete[] idsRegiones;
    if (nombreRegiones != nullptr) delete[] nombreRegiones;
    if (nombresCategorias != nullptr) delete[] nombresCategorias;

    menu(db);
    return;

}
