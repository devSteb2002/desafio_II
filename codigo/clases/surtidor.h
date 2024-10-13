#ifndef SURTIDOR_H
#define SURTIDOR_H

#include <iostream>
#include "conexion/conexion.h"
#include "estacion.h"

using namespace std;

class Surtidor
{
public:
    Surtidor(QSqlDatabase& db_, estacion &estacion_);

    int obtenerCantidadActivos();
    unsigned int agregarSurtidor(unsigned int id);

    void setId(unsigned int id);
    void setModelo(string modelo);
    void setActivo(bool activo);

    unsigned int getId() const;
    string getModelo() const;
    bool getActivo() const;

    ~Surtidor();
private:
    unsigned int id;
    string modelo;
    bool activo;
    QSqlDatabase& db;
    estacion& estacion;

};

#endif // SURTIDOR_H
