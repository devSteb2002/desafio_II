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


void Categoria::obtenerCategorias(string *&nombres, unsigned short *&ids, unsigned short &tamaño){

    QSqlQuery query(db);
    query.prepare("SELECT id_categoria, nombre FROM tbl_categoria");

    if (query.exec()){

        tamaño = obtenerCantidadDeCategorias(); // obtener la cantidad de categorias existentes

        if (tamaño == 0){
            cout << "No hay categorias en la base de datos." << endl;
            return;
        }

        string *arrayNombres = new string[tamaño];
        unsigned short *arrayIds = new unsigned short[tamaño];
        unsigned short i = 0;

        while (query.next()){
            unsigned int idCategoria = query.value(0).toUInt();
            string nombre = query.value(1).toString().toStdString();

            arrayNombres[i] = nombre;
            arrayIds[i] = idCategoria;

            i++;
        }

        nombres = arrayNombres; // retornar los arreglos por referencia
        ids = arrayIds;


    }else{
        nombres = nullptr;
        ids = nullptr;
        tamaño = 0;

        cout << "Error al consultar categorias" << endl;
    }
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
