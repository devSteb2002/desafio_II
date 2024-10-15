#include "nave.h"

Nave::Nave(QSqlDatabase& db_) : db(db_) {}


void Nave::crearNaves(unsigned int idEstacion, unsigned short numeroNaves){

    QSqlQuery query(db);

    query.prepare("INSERT INTO tbl_nave(id_estacion) VALUES(?)");

    for (unsigned short i = 0; i < numeroNaves; i++){

        query.bindValue(0, idEstacion);

        if (!query.exec()){
            cout << "Error al crear una nave." << endl;
        }
    }
}

void Nave::obtenerNaves(unsigned int idEstacion) {

    QSqlQuery query(db);

    query.prepare("SELECT id_nave FROM tbl_nave WHERE id_estacion = ?");
    query.bindValue(0, idEstacion);

    if (!query.exec()) {
        cout << "Error al obtener las naves." << endl;
        return;
    }

    while(query.next()) {
        unsigned int idNave = query.value(0).toUInt();

        cout << "Nave: " << idNave << endl;
    }

}


void Nave::setIdNave(unsigned short idNave){
    this->idNave = idNave;
}
unsigned short Nave::getIdNave() const{
    return this->idNave;
}

Nave::~Nave(){

}
