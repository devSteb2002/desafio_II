#ifndef ENTRADAS_H
#define ENTRADAS_H

#include <iostream>

using namespace std;

bool validarCin();
bool validarRango(unsigned short rango1, unsigned short rango2, unsigned short numero);
bool validarPositivo(short numero);
bool validarVarchar(unsigned short longitud, string text);
bool validarNumeroEnArreglo(unsigned int *&array, unsigned int tamañaArray ,unsigned int numero);

#endif // ENTRADAS_H
