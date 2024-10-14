#ifndef COMBUSTIBLE_H
#define COMBUSTIBLE_H

#include "conexion/conexion.h"

class Combustible
{
public:
    Combustible(QSqlDatabase& db_);

    void guardarCombustible(unsigned short idRegion, unsigned short idCategoria);
    void obtenerValorLitro(unsigned int idEstacion, unsigned short idCategoria);

    void setIdCombustible(unsigned short idCombustible);
    void setValorLitro(float valorLitro);

    unsigned short getIdCombustible() const;
    float getValorLitro() const;

    ~Combustible();
private:
    unsigned int idCombustible;
    float valorLitro;
    QSqlDatabase& db;

};

#endif // COMBUSTIBLE_H
