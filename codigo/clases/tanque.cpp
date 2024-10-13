#include "tanque.h"
#include "estacion.h"

Tanque::Tanque(): estacion(nullptr), db(nullptr){}

Tanque::Tanque(class estacion* estacion_, QSqlDatabase* db_) : estacion(estacion_), db(db_) {

    QSqlQuery query(*db);

    query.prepare("SELECT id_tanque, capacidad_tanque FROM tbl_tanque WHERE id_estacion = ?");
    query.bindValue(0, estacion->getId());

    if (query.exec()){
        if (query.next()){
            unsigned int id = query.value(0).toUInt();
            float capacidadTanque = query.value(1).toFloat();

            setIdTanque(id);
            setCapacidadTanque(capacidadTanque);
        }
    }
}

void Tanque::crearTanque(){

    QSqlQuery query(*db);
    query.prepare("INSERT INTO tbl_tanque (id_estacion) VALUES(?)");
    query.bindValue(0, estacion->getId());

    if (!query.exec()){
        cout << "Error al guardar un tanque" << endl;
        return;
    }
}

bool Tanque::eliminarTanque(){

    QSqlQuery query(*db);
    query.prepare("DELETE FROM tbl_tanque WHERE id_estacion = ?");
    query.bindValue(0, estacion->getId());

    if (query.exec()) return true;
    else cout << query.lastError().text().toStdString() << endl;

    return false;
}


//setter
void Tanque::setIdTanque(unsigned int idTanque){
    this->idTanque = idTanque;
}
void Tanque::setCapacidadTanque(float capacidadTanque){
    this->capacidadTanque = capacidadTanque;
}

//getter

unsigned int  Tanque::getIdTanque() const{
    return this->idTanque;
}

float Tanque::getCapacidadTanque() const{
    return this->capacidadTanque;
}


Tanque::~Tanque(){

    // if (estacion){
    //     delete estacion;
    //     estacion = nullptr;
    // }

    // if (db){
    //     delete db;
    //     db = nullptr;
    // }

}
