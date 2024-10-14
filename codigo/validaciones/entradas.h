#ifndef ENTRADAS_H
#define ENTRADAS_H

#include <iostream>

using namespace std;

bool validarCin();
bool validarRango(unsigned short rango1, unsigned short rango2, unsigned short numero);
bool validarPositivo(short numero);
bool validarVarchar(unsigned short longitud, string text);

template <typename T>
bool validarNumeroEnArreglo(T *&array, T tamañaArray ,T numero){

    for (unsigned int i = 0; i < tamañaArray; i++){
        if (array[i] == numero) return true;
    }

    cout << "Codigo ingresado no existe, vuelva a intentar." << endl;
    cout << "----------------" << endl;

    return false;
}

#endif // ENTRADAS_H
