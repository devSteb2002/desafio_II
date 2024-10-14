#ifndef NAVE_H
#define NAVE_H

#include "conexion/conexion.h"

class Nave
{
public:
    Nave(QSqlDatabase& db_);

    void crearNaves(unsigned int idEstacion, unsigned short numeroNaves);
    bool eliminarNaves(unsigned int idEstacion);

    void setIdNave(unsigned short idNave);
    unsigned short getIdNave() const;



    ~Nave();
private:
    unsigned short idNave;
    QSqlDatabase& db;
};

#endif // NAVE_H
