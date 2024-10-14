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

float Tanque::obtenerCapacidadCategoria(unsigned short idCategoria){
    QSqlQuery query(*db);

    query.prepare("SELECT tc.capacidad_categoria, tc.id_tanque FROM tbl_tanque AS tan "
                  "INNER JOIN tbl_tanque_categoria AS tc ON "
                   "tan.id_tanque = tc.id_tanque WHERE "
                  "tan.id_estacion = ? and tc.id_categoria = ?");
    query.bindValue(0, estacion->getId());
    query.bindValue(1, idCategoria);

    if (query.exec() && query.next()){
        setIdTanque(query.value(1).toUInt());
        return query.value(0).toFloat();
    }else{
        setIdTanque(0);
        cout << "Error al consultar la capacidad de la categoria" << endl;
        return 0.0;
    }
}

bool Tanque::actualizarCapacidadCategoria(float litros, unsigned short idCategoria){ // actualizar la capacidad de las categorias

    QSqlQuery query(*db);

    query.prepare("SELECT id_categoria_tanque, capacidad_categoria "
                  "FROM tbl_tanque_categoria WHERE id_tanque = ? and id_categoria = ?");
    query.bindValue(0, getIdTanque());
    query.bindValue(1, idCategoria);

    if (query.exec() && query.next()){
        unsigned int idCategoriaTanque = query.value(0).toUInt();
        float capacidadEnTanque = query.value(1).toFloat();
        float nuevaCapacidad = capacidadEnTanque - litros;

        query.prepare("UPDATE tbl_tanque_categoria SET capacidad_categoria = ? "
                      "WHERE id_categoria_tanque = ?");
        query.bindValue(0, nuevaCapacidad);
        query.bindValue(1, idCategoriaTanque);

        if (query.exec()) return true;
        else return false;
    }else return false;
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
