#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "conexion/conexion.h"

class Categoria
{
public:
    Categoria(QSqlDatabase& db_);

    unsigned short obtenerCantidadDeCategorias();

    void setIdCategoria(unsigned int idCategoria);
    void setNombre(string nombre);


    unsigned int getIdCategoria() const;
    string getNombre() const;


    ~Categoria();
private:
    unsigned int idCategoria;
    string nombre;
    QSqlDatabase& db;

};

#endif // CATEGORIA_H
