#include "categoria.h"

Categoria::Categoria(QSqlDatabase& db_) : db(db_) {}


unsigned short Categoria::obtenerCantidadDeCategorias(){

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(id_categoria) FROM tbl_categoria");

    if (query.exec() && query.next()){
       unsigned short tamaño = query.value(0).toUInt();

        return tamaño;
    }


    return 0;
}

void Categoria::setIdCategoria(unsigned int idCategoria){
    this->idCategoria = idCategoria;
}

void Categoria::setNombre(string nombre){
    this->nombre = nombre;
}


unsigned int Categoria::getIdCategoria() const{
    return this->idCategoria;
}

string Categoria::getNombre() const{
    return this->nombre;
}


Categoria::~Categoria(){

}
