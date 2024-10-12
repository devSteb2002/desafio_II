#include "conexion.h"

conexion::conexion() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // configuracion sqlite
    db.setDatabaseName(NombreBd); //definir el nombre de la bd

    if (!db.open()){
        cout << "Error al abrir la base de datos." << db.lastError().text().toStdString() << endl;
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
