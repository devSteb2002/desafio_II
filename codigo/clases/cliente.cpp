#include "cliente.h"

Cliente::Cliente(QSqlDatabase& db_) : db(db_) {}


bool Cliente::verificarExistencia(){

    QSqlQuery query(db);

    query.prepare("SELECT COUNT(id_cliente) FROM tbl_cliente WHERE cedula = ?");
    query.bindValue(0, getCedula());

    if (query.exec() && query.next()){

        query.prepare("SELECT id_cliente ,nombre, apellido FROM tbl_cliente WHERE cedula = ?");
        query.bindValue(0, getCedula());

        if (query.exec() && query.next()){
            setIdCliente(query.value(0).toUInt());
            setNombre(query.value(1).toString().toStdString());
            setApellido(query.value(2).toString().toStdString());

            return true;
        }
    }

    return false;
}

bool Cliente::crearCliente(){
    QSqlQuery query(db);

    query.prepare("INSERT INTO tbl_cliente (nombre, apellido, cedula) VALUES (?,?,?)");
    query.bindValue(0, QString::fromStdString(getNombre()));
    query.bindValue(1, QString::fromStdString(getApellido()));
    query.bindValue(2, getCedula());

    if (query.exec()){
        setIdCliente(query.lastInsertId().toUInt());
        return true;
    }else{
        cout << "Error al registrar el usuario." << endl;
        return false;
    }
}

void Cliente::setIdCliente(unsigned int idCliente){
    this->idCliente = idCliente;
}

void Cliente::setCedula(unsigned int cedula){
    this->cedula = cedula;
}

void Cliente::setNombre(string nombre){
    this->nombre = nombre;
}

void Cliente::setApellido(string apellido){
    this->apellido = apellido;
}

unsigned int Cliente::getIdCliente() const{
    return this->idCliente;
}
unsigned int Cliente::getCedula() const{
    return this->cedula;
}
string Cliente::getNombre() const{
    return this->nombre;
}
string Cliente::getApellido() const{
    return this->apellido;
}

Cliente::~Cliente(){

}
