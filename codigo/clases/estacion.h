#ifndef ESTACION_H
#define ESTACION_H

#include <iostream>
#include "conexion/conexion.h"

using namespace std;

class estacion
{
public:
    estacion(QSqlDatabase& db_);

    unsigned int crearEstacion();
    unsigned int* obtenerEstaciones(unsigned short &tamaño);


    //setter
    void setId(int id);
    void setNombre(string nombre);
    void setRegion(string region);
    void setLatitud(float latitud);
    void setLongitud(float longitud);

    //getter
    unsigned int getId() const;
    string getNombre() const;
    string getRegion() const;
    float getLatitud() const;
    float getLongitud() const;

    ~estacion();

private:
    unsigned int id;
    string nombre;
    string region;
    float latitud;
    float longitud;
    QSqlDatabase& db;
};

#endif // ESTACION_H
