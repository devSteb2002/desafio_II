#ifndef ESTACION_H
#define ESTACION_H

#include "conexion/conexion.h"

using namespace std;

class estacion
{

    friend ostream& operator<<(ostream &os, const estacion & e){ // sobrecarga de operador << para imprimir el id de la clase
        os << "Estacion id: " << e.id;
    }

public:
    estacion(QSqlDatabase& db_);

    unsigned int crearEstacion(unsigned int idRegion);
    unsigned int* obtenerEstaciones(unsigned int &tamaño, bool mostrarMensaje = true);
    unsigned int obtenerTotalEs(unsigned int idRed);
    bool eliminarEstacion();

    //setter
    void setId(int id);
    void setNombre(string nombre);
    void setLatitud(float latitud);
    void setLongitud(float longitud);

    //getter
    unsigned int getId() const;
    string getNombre() const;
    float getLatitud() const;
    float getLongitud() const;

    ~estacion();

private:
    unsigned int id;
    string nombre;
    float latitud;
    float longitud;
    QSqlDatabase& db;
};

#endif // ESTACION_H
