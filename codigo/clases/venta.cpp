#include "venta.h"

Venta::Venta(class Surtidor &surtidor_ , QSqlDatabase& db_): surtidor(surtidor_), db(db_){

}

bool Venta::eliminarVenta(){

}


void Venta::setIdVenta(){}
void Venta::setFecha(){}
void Venta::setHora(){}
void Venta::setMetodoPago(){}
void setCantidadCombustible();
void setCantidadDinero(){}


unsigned int Venta::getIdVenta() const{}
string Venta::getFecha() const{}
string Venta::getHora() const{}
string Venta::getMetodoPago() const{}
float Venta::getCantidadCombustible() const{}
float Venta::getCantidadDinero() const{}


Venta::~Venta(){

}
