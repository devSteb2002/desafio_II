#include "surtidor.h"

Surtidor::Surtidor(QSqlDatabase& db_, class estacion& estacion_): db(db_), estacion(estacion_) {}


int Surtidor::obtenerCantidadActivos(){

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(id_surtidor) FROM tbl_surtidor WHERE id_estacion = ? and activo = ?");
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

unsigned int Surtidor::agregarSurtidor() {

    QSqlQuery query(db);

    query.prepare("INSERT INTO tbl_surtidor (modelo, activo, id_estacion) VALUES (?, ?, ?)");
    query.bindValue(0, QString::fromStdString(getModelo()));
    query.bindValue(1, getActivo());
    query.bindValue(2, estacion.getId());

    if (!query.exec()) {
        cerr << "Error al insertar en la bd: " << query.lastError().text().toStdString() << endl;
        return 0;
    }

    unsigned int idSurtidor = query.lastInsertId().toInt();
    return idSurtidor;
}


bool Surtidor::eliminarSurtidor(Venta &venta){ //eliminar surtidor
    QSqlQuery query(db);
    query.prepare("DELETE FROM tbl_surtidor WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (query.exec()) return true;

    return false;

}

//setter
void Surtidor::setId(unsigned int id){
    this->id = id;
}
void Surtidor::setModelo(string modelo){
    this->modelo = modelo;
}
void Surtidor::setActivo(bool activo){
    this->activo = activo;
}


//getters

unsigned int Surtidor::getId() const{
    return this->id;
}
string Surtidor::getModelo() const{
    return this->modelo;
}
bool Surtidor::getActivo() const{
    return this->activo;
}

Surtidor::~Surtidor(){

}
