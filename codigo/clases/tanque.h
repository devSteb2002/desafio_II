#ifndef TANQUE_H
#define TANQUE_H

#include <iostream>
#include "estacion.h"

using namespace std;

class Tanque
{
public:

    Tanque();
    Tanque(class estacion* estacion_, QSqlDatabase* db_);

    void crearTanque();
    short eliminarTanque();

    void setIdTanque(unsigned int idTanque);
    void setCapacidadTanque(float capacidadTanque);

    unsigned int getIdTanque() const;
    float getCapacidadTanque() const;


    ~Tanque();
private:
    estacion* estacion; // puntero para poder generar miembro a miebro vacio
    QSqlDatabase* db;
    unsigned int idTanque;
    float capacidadTanque;
};

#endif // TANQUE_H
