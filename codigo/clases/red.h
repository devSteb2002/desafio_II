#ifndef RED_H
#define RED_H

#include "conexion/conexion.h"

using namespace std;

class red
{
    public:
        red(QSqlDatabase& bd_);

        unsigned int getIdRed() const;
        string getPais() const;
        void setIdRed(unsigned int id);
        ~red();

    private:
        unsigned int idRed;
        string pais;
        QSqlDatabase& bd;

};

#endif // RED_H
