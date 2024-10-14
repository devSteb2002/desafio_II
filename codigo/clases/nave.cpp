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


void Nave::setIdNave(unsigned short idNave){
    this->idNave = idNave;
}
unsigned short Nave::getIdNave() const{
    return this->idNave;
}

Nave::~Nave(){

}
