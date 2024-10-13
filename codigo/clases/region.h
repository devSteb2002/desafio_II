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
    void setValorCombustible(float valorCombustible);

    unsigned short getIdRegion();
    string getNombre();
    float getValorCombustible();

    ~Region();


private:
    unsigned short idRegion;
    string nombre;
    float valorCombustible;
    QSqlDatabase& db;
};

#endif // REGION_H
