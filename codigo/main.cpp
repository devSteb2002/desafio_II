#include "conexion/conexion.h"
#include "validaciones/entradas.h"
#include "controladores/gestionRed.h"

void inicio();


template <typename T> T myMax(T x, T y)
{
    return (x > y) ? x : y;
}

int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);


    // bdObject.~conexion(); // cerrra conexio


    // QSqlQuery query(db); // crear un objeto query
    // query.prepare("SELECT * FROM tbl_red"); // prepara la consulta
    // query.exec(); // ejecutar la consulta

    // while (query.next()){
    //     cout << query.value(0).toInt() << endl;
    // }


    // // Insertar un empleado en la base de datos
    // // query.prepare("INSERT INTO empleados (nombre, salario) VALUES (:nombre, :salario)");
    // // query.bindValue(":nombre", "Carlos González");
    // // query.bindValue(":salario", 5000.50);



    inicio();



    return 0;
}


void inicio(){
    //Inicio

    cout << "-----------------------------" << endl;
    cout << "|          TERMAX           |" << endl;
    cout << "-----------------------------" << endl;
    cout << "Inicio" << endl;
    cout << "------" << endl;
    cout << "Opciones disponibles. " << endl;
    cout << "1. Gestion de red." << endl;
    cout << "2. Gestion de estaciones de servicion." << endl;


    short opcion;
    while (true){
        cout << "Ingrese la opcion: ";
        cin >> opcion;

        if (!validarCin()) continue;
        if (!validarPositivo(opcion)) continue;
        if (!validarRango(1, 2, opcion)) continue;

        break;
    }

    switch (opcion) {
    case 1:
        gestionRed();
        break;
    case 2:
        break;
    default:
        break;
    }


}
