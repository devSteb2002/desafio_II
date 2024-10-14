#include "conexion.h"

conexion::conexion() {

    if (!QSqlDatabase::contains("desafio_II")){

        db = QSqlDatabase::addDatabase("QSQLITE", "desafio_II"); // configuracion sqlite
        db.setDatabaseName(NombreBd); //definir el nombre de la bd

        if (!db.open()){
            cout << "Error al abrir la base de datos." << db.lastError().text().toStdString() << endl;
        }
    }else{
        db = QSqlDatabase::database("desafio_II");
    }
}

QSqlDatabase& conexion:: getBd(){ //devolver la referencia del objeto db
    return db;
}

conexion::~conexion(){
    if (db.isOpen()){
        db.close();
    }
}
