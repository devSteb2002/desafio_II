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

    unsigned int idSurtidor = query.lastInsertId().toUInt();
    return idSurtidor;
}

bool Surtidor::verificarExistencia(){ // verificar si existen surtidores en esta esta estacion
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM tbl_surtidor WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (query.exec()){
        if (query.next()){
            unsigned int cantidad = query.value(0).toUInt();

            if (cantidad == 0) return false;
            else return true;
        }
    }

    return false;
}

bool Surtidor::eliminarSurtidores(Venta &venta){ //eliminar surtidor inactivos
    QSqlQuery query(db);

    //consultar cantidad de inactivos
    query.prepare("SELECT COUNT(id_surtidor) FROM tbl_surtidor WHERE activo = ? and id_estacion = ?");
    query.bindValue(0, 0);
    query.bindValue(1, estacion.getId());

    if (query.exec()){
        if (query.next()){
            unsigned int cantidad = query.value(0).toUInt();

            if (cantidad != 0) { //Existen surtidores inactivos

                unsigned int *idsSurtidor = new unsigned int[cantidad];
                unsigned int iterador = 0;

                query.prepare("SELECT id_surtidor FROM tbl_surtidor WHERE activo = ? and id_estacion = ?"); // buscar todos los surtidores
                query.bindValue(0, 0);
                query.bindValue(1, estacion.getId());

                if (query.exec()){
                    while(query.next()){
                        unsigned int id = query.value(0).toUInt();
                        idsSurtidor[iterador] = id;
                        iterador++;
                    }

                    //eliminar las ventas asociadas a los ids de surtidores inactivos
                    bool ventasEliminadas = venta.eliminarVentas(idsSurtidor, iterador);
                    delete[] idsSurtidor;

                    if (ventasEliminadas){ // se eliminaron correctamente
                        //eliminar los surtidores

                        query.prepare("DELETE FROM tbl_surtidor WHERE id_estacion = ?");
                        query.bindValue(0, estacion.getId());

                        if (query.exec()) return true;
                        else {
                            cout << query.lastError().text().toStdString() << endl;
                            return false;
                        }
                    }

                }else{
                    delete[] idsSurtidor;
                    cout << query.lastError().text().toStdString() << endl;
                    return false;
                }
            }else{
                //no hay existencia de sutidores inactivos por lo tanto no hay ventas
                return true;
            }
        }

    }

    cout << query.lastError().text().toStdString() << endl;
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
