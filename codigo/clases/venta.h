#ifndef VENTA_H
#define VENTA_H

#include "conexion/conexion.h"
#include "categoria.h"

class Venta
{
public:
    Venta(class Surtidor &surtidor_ , QSqlDatabase& db_);

    bool añadirVenta(unsigned short idCategoria, unsigned int idCliente,  unsigned int idSurtidor);
    bool eliminarVentas(unsigned int *&ids, unsigned int tamaño);
    void calcularVentasPorES(unsigned int idRed, Categoria& categoria);
    void litrosDeCombustibleVendidosPorCategoria();

    void setIdVenta(unsigned int idVenta);
    void setFecha(string fecha);
    void setHora(string hora);
    void setMetodoPago(string metodoPago);
    void setCantidadCombustible(float cantidadCombustible);
    void setCantidadDinero(float cantidadDinero);
    void setTotalVenta(float totalVenta);


    unsigned int getIdVenta() const;
    string getFecha() const;
    string getHora() const;
    string getMetodoPago() const;
    float getCantidadCombustible() const;
    float getCantidadDinero() const;
    float getTotalVenta() const;


    ~Venta();
private:
    unsigned int idVenta;
    string fecha;
    string hora;
    string metodoPago;
    float cantidadCombustible;
    float cantidadDinero;
    float totalVenta;
    Surtidor& surtidor;
    QSqlDatabase& db;

};

#endif // VENTA_H
