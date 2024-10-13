#include "red.h"

red::red(QSqlDatabase& bd_): idRed(0),  pais("Colombia"), bd(bd_) {

    QSqlQuery query(bd);

    query.prepare("SELECT id_red FROM tbl_red LIMIT 1");
    query.exec();

    if (!query.exec()){
        cout << "Error en la consulta" << endl;
        return;
    }

    if (query.next()){
        unsigned int idRed = query.value(0).toUInt();
        setIdRed(idRed);
    }
}

//setter
void red::setIdRed(unsigned int id){
    this->idRed = id;
}


//getter
unsigned int red::getIdRed() const{
    return this->idRed;
}

string red::getPais() const{
    return this->pais;
}


red::~red(){

}
