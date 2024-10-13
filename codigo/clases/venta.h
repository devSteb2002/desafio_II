#ifndef VENTA_H
#define VENTA_H

#include "conexion/conexion.h"

class Venta
{
public:
    Venta(class Surtidor &surtidor_ , QSqlDatabase& db_);

    bool eliminarVenta();

    void setIdVenta();
    void setFecha();
    void setHora();
    void setMetodoPago();
    void setCantidadCombustible();
    void setCantidadDinero();


    unsigned int getIdVenta() const;
    string getFecha() const;
    string getHora() const;
    string getMetodoPago() const;
    float getCantidadCombustible() const;
    float getCantidadDinero() const;


    ~Venta();
private:
    unsigned int idVenta;
    string fecha;
    string hora;
    string metodoPago;
    float cantidadCombustible;
    float cantidadDinero;
    Surtidor& surtidor;
    QSqlDatabase& db;

};

#endif // VENTA_H
