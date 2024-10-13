#include "estacion.h"

estacion::estacion(QSqlDatabase& db_) : db(db_) {}


unsigned int estacion::crearEstacion(unsigned int idRegion){ // crear una estacion de servicio

    QSqlQuery query(db);
    query.prepare("INSERT INTO tbl_estacion (nombre, latitud, longitud, id_red, id_region) VALUES (?, ?, ?, ?, ?)");
    //query.bindValue(":id_estacion", 2);
    query.bindValue(0, QString::fromStdString(getNombre()));
    query.bindValue(1, getLatitud());
    query.bindValue(2, getLongitud());
    query.bindValue(3, 1);
    query.bindValue(4, idRegion);

    if (!query.exec()){
        cerr << "Error al insertar en la bd" << query.lastError().text().toStdString() << endl;
        return 0;
    }

    unsigned int idInsertado = query.lastInsertId().toInt(); //retornar el id creado
    return idInsertado;
}

unsigned int* estacion::obtenerEstaciones(unsigned int &tamaño){ //obtener las estaciones de servicio de la red

    QSqlQuery query(db);
    unsigned short cantidad = 0;

    query.prepare("SELECT COUNT(id_estacion) FROM tbl_estacion WHERE id_red = ?");
    query.bindValue(0, 1);

    if (!query.exec()){
        cout << "Error al consultar datos" << endl;
        return nullptr;
    }

    if (query.next()) cantidad = query.value(0).toInt();

    unsigned int* ids = new unsigned int[cantidad];
    unsigned int iterador = 0;

    query.prepare("SELECT id_estacion, nombre FROM tbl_estacion WHERE id_red = ?");
    query.bindValue(0, 1);

    if (!query.exec()){
        cout << "No se pudo consultar las estaciones." << endl;
        delete[] ids;
        return nullptr;
    }

    cout << "-----------------------------------------------" << endl;
    cout << "|              Lista de Estaciones            |" << endl;
    cout << "-----------------------------------------------" << endl;


    while(query.next()){
        unsigned int const idEstacion = query.value(0).toInt();
        string nombre = query.value(1).toString().toStdString();
        ids[iterador] = idEstacion;

        cout << idEstacion << " " << nombre << endl;
        iterador++;
    }

    tamaño = iterador;
    return ids;
}


void estacion::eliminarEstacion(){ //eliminar una estacion (eliminar primero surtidores y tanques)



}


//setter
void estacion::setId(int id){
    this->id = id;
}

void estacion::setNombre(string nombre){
    this->nombre = nombre;
}

void estacion::setLatitud(float latitud){
    this->latitud = latitud;
}

void estacion::setLongitud(float longitud){
    this->longitud = longitud;
}

//getter
unsigned int  estacion::getId() const{
    return this->id;
}

string estacion::getNombre() const{
    return this->nombre;
}


float estacion::getLatitud() const{
    return this->latitud;
}

float estacion::getLongitud() const{
    return this->longitud;
}


estacion::~estacion(){

}
