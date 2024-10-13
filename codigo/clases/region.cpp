#include "region.h"

Region::Region(QSqlDatabase& db_): db(db_) {}


void Region::obtenerRegiones(unsigned short *&ids, string *&nombres, unsigned short &tamaño){ // consultar las regiones disponibles

    QSqlQuery query(db);

    unsigned int tamañoArreglo = 0;

    query.prepare("SELECT COUNT(id) FROM tbl_region"); // obtener la cantidad de registros

    if (query.exec()){
        if (query.next()){
            tamañoArreglo = query.value(0).toInt();
        }
    }else{
        cout << "Errror al ejecutar la consultas " << query.lastError().text().toStdString();
    }

    if (tamañoArreglo == 0){
        ids = nullptr;
        nombres = nullptr;
        return;
    }


    unsigned short *ids_ = new unsigned short[tamañoArreglo];  // definir un arreglo con esos registros
    string *nombres_ = new string[tamañoArreglo];

    query.prepare("SELECT id, nombre FROM tbl_region");

    if (query.exec()){
        unsigned short i = 0;
        while (query.next()){
            unsigned int id = query.value(0).toInt();
            string nombre = query.value(1).toString().toStdString();
            nombres_[i] = nombre;
            ids_[i] = id;
            i++;
        }
    }else{
        cout << "Errror al ejecutar la consultas " << query.lastError().text().toStdString();
    }

    delete[] ids;
    delete[] nombres;
    ids = ids_;
    nombres = nombres_;
    tamaño = tamañoArreglo;
}



void Region::setIdRegion(unsigned short idRegion){

}

void Region::setNombre(string nombre){

}
void Region::setValorCombustible(float valorCombustible){

}

unsigned short Region::getIdRegion(){

}
string Region::getNombre(){

}
float Region::getValorCombustible(){

}

Region::~Region(){

}
