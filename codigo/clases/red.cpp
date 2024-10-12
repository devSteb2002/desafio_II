#include "red.h"

red::red(): idRed(0), pais("Colombia") {
    conexion conn; // instancia conexion
    QSqlDatabase& db = conn.getBd(); // conexion a la base de datos

    QSqlQuery query(db);

    query.prepare("SELECT id_red FROM tbl_red LIMIT 1");
    query.exec();

    if (!query.exec()){
        cout << "Error en la consulta" << endl;
        return;
    }

    if (query.next()){
        unsigned int idRed = query.value(0).toInt();
        setIdRed(idRed);
    }

    conn.~conexion(); // cerrar conexion
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
