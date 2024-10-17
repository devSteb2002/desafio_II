#include "validaciones/entradas.h"
#include "clases/estacion.h"
#include "clases/surtidor.h"
#include "clases/categoria.h"
#include "clases/cliente.h"
#include "clases/tanque.h"
#include "clases/combustible.h"
#include "clases/venta.h"
#include "QDate"
#include "QTime"

void menuSimluacion(QSqlDatabase& db);

void simulacionVenta(){

    srand(static_cast<unsigned int>(time(0)));

    conexion bdObject; // instanciar la clase conexion
    QSqlDatabase& db = bdObject.getBd(); // obtener el objeto db

    menuSimluacion(db);

   //bdObject.~conexion();

}

void menuSimluacion(QSqlDatabase& db){

    cout << "-----------------------------------------------" << endl;
    cout << "|              SIMULACION DE VENTAS            |" << endl;
    cout << "-----------------------------------------------" << endl;

    unsigned int tamaño;

    estacion estacion(db);
    Tanque tanque(&estacion, &db);
    Surtidor surtidor(db, estacion);
    Categoria categoria(db);
    Cliente cliente(db);
    Combustible combustible(db);
    Venta venta(surtidor, db);

    unsigned int *idsEstaciones = estacion.obtenerEstaciones(tamaño);

    if (idsEstaciones != nullptr && tamaño != 0){

        unsigned int id;
        while (true){ //pedir codigo
            cout << "Ingrese un codigo de estacion: ";
            cin >> id;

            if (!validarCin()) continue;
            if (!validarPositivo(id)) continue;
            if (!validarNumeroEnArreglo(idsEstaciones, tamaño ,id)) continue; // validar que el codigo exista

            break;
        }

        delete[] idsEstaciones;
        estacion.setId(id);
        surtidor.obnterSurtidorAleatorio();


        if (surtidor.getId() == 0){
            cout << endl;
            cout << "| Esta estacion aun no cuenta con surtidores. |" << endl;
            cout << endl;
            return;
        }

        //obtener cadegoria aletaoria
        string *nombres = nullptr;
        unsigned short *ids = nullptr;
        unsigned short tamaño;

        categoria.obtenerCategorias(nombres, ids, tamaño);

        if (nombres == nullptr && ids == nullptr){
            cout << "No existen categorias para la venta" << endl;
            return;
        }

        const unsigned short randomCategoiras = (rand() % tamaño) + 1;
        const string categoriaSeleccionada = nombres[randomCategoiras - 1];
        const unsigned short idCategoriaSeleccionada = ids[randomCategoiras - 1];
        float randomlitros = (rand() % 20) + 3;

        delete[] nombres;
        delete[] ids;

        combustible.obtenerValorLitro(estacion.getId(), idCategoriaSeleccionada); // obtener valor litro setiado setvalorlitro estacion, region
        float const valorLitro = combustible.getValorLitro(); // valor litro

        float const capacidadCategoriaEnTanque = tanque.obtenerCapacidadCategoria(idCategoriaSeleccionada); // capacidad tanque

        //verificacion de capacidad en el tanque
        if (capacidadCategoriaEnTanque == 0.0){
            cout << endl;
            cout << "| La categoria " << categoriaSeleccionada << " esta agotada. |" << endl;
            cout << endl;
            return;
        }

        if (capacidadCategoriaEnTanque < randomlitros) randomlitros = capacidadCategoriaEnTanque;

        float costoGasolina = valorLitro * randomlitros; // valor de la compra

        cout << "Surtidor:  " << surtidor.getModelo() << endl;
        cout << "Pedido: " << randomlitros << "L de " << categoriaSeleccionada << endl;
        cout << "Valor total: $" << costoGasolina << endl;

        cout << "| Datos del cliente |" << endl;

        //pedir datos del cliente
        unsigned int cedula;
        float dinero;
        string nombre, apellido;
        bool validoCedula = false, validoNombre = false, validoApellido = false;
        bool validoMetodoPago = false;
        bool init = true, yaExiste = false;
        unsigned short opcionPago;

        string metodoPago[3] = { // metodos de pago
            "Efectivo",
            "TDebito",
            "TCredito"
        };

        while(true){
            if (!validoCedula){
                cout << "Introduzca su cedula: ";
                cin >> cedula;

                if (!validarCin()) continue;
                if (!validarCedula(cedula)) continue;

                cliente.setCedula(cedula);
                if (cliente.verificarExistencia()){ // el usuario ya esta registrado
                    yaExiste = true;
                    validoNombre = true;
                    validoApellido = true;

                    cout << "---------------" << endl;
                    cout << "Bienvenido de nuevo senor(a):" << cliente.getNombre() << endl;

                }

                validoCedula = true;
            }else {
                if (!validoNombre){
                    if (init) cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    init = false;

                    cout << "Ingrese su nombre: ";
                    getline(cin, nombre);

                    if (!validarVarchar(60, nombre)) continue;
                    if (!validarNombreYApellido(nombre)) continue;

                    validoNombre = true;
                    init = true;
                }else if (!validoApellido){

                    cout << "Ingrese su apellido: ";
                    getline(cin, apellido);

                    if (!validarVarchar(60, apellido)) continue;
                    if (!validarNombreYApellido(apellido)) continue;

                    validoApellido = true;
                }else if (!validoMetodoPago) {

                    for (unsigned short i = 0; i < 3; i++)
                        cout << (i + 1) << ". " << metodoPago[i] << endl;
                    cout << "Seleccione un metodo de pago: " << endl;

                    cin >> opcionPago;

                    if (!validarCin()) continue;
                    if (!validarRango(1, 3, opcionPago)) continue;

                    validoMetodoPago = true;

                }else{
                    cout << "Ingrese el monto a pagar: ";
                    cin >> dinero;

                    if (!validarCin()) continue;

                    if (dinero < 0.0){
                        cout << "No se admiten valores negativos" << endl;
                        continue;
                    }

                    if (dinero < costoGasolina){
                        cout << "Insuficiente dinero, vuelva a intentar." <<  endl;
                        continue;
                    }

                    break;
                }
            }
        }


        if (!yaExiste){ // sino exste el usuario se crea
            cliente.setCedula(cedula);
            cliente.setNombre(nombre);
            cliente.setApellido(apellido);
            cliente.crearCliente();
        }

         float totalVenta = costoGasolina;

         QDate fechaActual = QDate::currentDate();
         string fechaFormateada = fechaActual.toString("yyyy-MM-dd").toStdString();

         QTime horaActual = QTime::currentTime();
         string horaFormateada = horaActual.toString("HH:mm:ss").toStdString();

         venta.setFecha(fechaFormateada);
         venta.setHora(horaFormateada);
         venta.setMetodoPago(metodoPago[opcionPago - 1]);
         venta.setCantidadCombustible(randomlitros);
         venta.setCantidadDinero(dinero);
         venta.setTotalVenta(totalVenta);

         bool resultado = venta.añadirVenta(idCategoriaSeleccionada, cliente.getIdCliente(), surtidor.getId());
         bool segundoResultado = tanque.actualizarCapacidadCategoria(randomlitros, idCategoriaSeleccionada);

         cout << tanque.getIdTanque() << endl;
         if (resultado && segundoResultado) {
             cout << "----------------------------------" << endl;
             cout << "|     DATOS DE LA TRANSACCION    |" << endl;
             cout << "----------------------------------" << endl;
             cout << "Datos de la compra" << endl;
             cout << "-------------------" << endl;
             cout << "Fecha/Hora : " << venta.getFecha() << " " << venta.getHora() << endl;
             cout << "Cantidad gasolina: " << venta.getCantidadCombustible() << " L" << endl;
             cout << "Tipo gasolina: " << categoriaSeleccionada << endl;
             cout << "Metodo de pago: " << venta.getMetodoPago() <<  endl;
             cout << "Dinero recibido: $" << dinero << endl;
             cout << "Dinero devuelto: $" << (dinero - costoGasolina) << endl;
             cout << "Valor total: $" << venta.getTotalVenta() << endl;
             cout << "-------------------" << endl;
             cout << "Datos del usuario" << endl;
             cout << "-------------------" << endl;
             cout << "Cedula: " << cliente.getCedula() << endl;
             cout << "Nombre: " << cliente.getNombre() << endl;
             cout << "Apellido(s): " << cliente.getApellido() << endl;
             cout << "-------------------" << endl;
             cout << endl;
         }
         else cout << "Erro en la venta" << endl;
    }else{
        cout << "No existen estaciones disponibles." << endl;
    }
}


