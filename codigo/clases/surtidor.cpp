#include "surtidor.h"

Surtidor::Surtidor(QSqlDatabase& db_, class estacion& estacion_): db(db_), estacion(estacion_) {}


int Surtidor::obtenerCantidadActivos(){

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(s.id_surtidor) FROM tbl_surtidor AS s INNER JOIN tbl_tanque AS t ON s.id_tanque = t.id_tanque WHERE t.id_estacion = ? and s.activo = ?");
    query.bindValue(0, estacion.getId());
    query.bindValue(1, 1);

    if (query.exec()){
        if (query.next()){
            int cantidad = query.value(0).toInt();
            return cantidad;
        }
    }

    return -1;
}


//setter
void Surtidor::setId(unsigned int id){

}
void Surtidor::setModelo(string modelo){

}
void Surtidor::setActivo(bool activo){

}


//getters

unsigned int Surtidor::getId() const{

}
string Surtidor::getModelo() const{

}
bool Surtidor::getActivo() const{

}

Surtidor::~Surtidor(){

}
