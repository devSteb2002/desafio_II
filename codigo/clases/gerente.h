#ifndef GERENTE_H
#define GERENTE_H

#include "conexion/conexion.h"

class Gerente
{
public:
    Gerente();

private:
    unsigned int idGerente;
    string nombre;
    string apellido;
    unsigned int cedula;
};

#endif // GERENTE_H
