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
    bool eliminarTanque();
    void verificarFugas();
    void asignarCapacidadAleatoriaDelTanque();
    float obtenerCapacidadCategoria(unsigned short idCategoria);
    bool actualizarCapacidadCategoria(float litros, unsigned short idCategoria);

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
