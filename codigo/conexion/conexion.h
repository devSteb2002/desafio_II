#ifndef CONEXION_H
#define CONEXION_H

#include <iostream>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

using namespace std;

class conexion
{
public:
    conexion();
    QSqlDatabase &getBd(); // referencia al objeto qsqldatabase para trabajar con sqlite
    ~conexion();

private:
    QString const NombreBd = "termax.db";
    QSqlDatabase db;
};
#endif // CONEXION_H
