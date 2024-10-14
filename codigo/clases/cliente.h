#ifndef CLIENTE_H
#define CLIENTE_H

#include "conexion/conexion.h"

class Cliente
{

private:
    unsigned int idCliente;
    unsigned int cedula;
    QSqlDatabase& db;

public:
    Cliente(QSqlDatabase& db_);

    void setIdCliente(unsigned int idCliente);
    void setCedula(unsigned int cedula);

    unsigned int getIdCliente() const;
    unsigned int getCedula() const;

    ~Cliente();
};

#endif // CLIENTE_H
