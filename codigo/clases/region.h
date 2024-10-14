#ifndef REGION_H
#define REGION_H

#include "conexion/conexion.h"

class Region
{
public:
    Region(QSqlDatabase& db_);

    void obtenerRegiones(unsigned short *&ids, string *&nombres, unsigned short &tamaño);

    void setIdRegion(unsigned short idRegion);
    void setNombre(string nombre);

    unsigned short getIdRegion();
    string getNombre();

    ~Region();


private:
    unsigned short idRegion;
    string nombre;
    QSqlDatabase& db;
};

#endif // REGION_H
