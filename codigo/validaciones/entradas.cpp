#include "entradas.h"
#include <limits>
#include <regex>

bool validarCin(){ // validar entrada de datos
    if (cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tipo de dato invalido, vuelva a intentar." << endl;
        return false;
    }

    return true;
}

bool validarRango(unsigned short rango1, unsigned short rango2,  unsigned short numero){ // validar un rango de datos

    if (numero < rango1 || numero > rango2){
        cout << "Las opciones permitas son desde " << rango1 << " hasta " << rango2 << ", vuelva a intentar." << endl;
        return false;
    }

    return true;
}

bool validarPositivo(short numero){

    if (numero < 0){
        cout << "No se admiten valores negativos, vuelva a intentar" << endl;
        return false;
    }

    return true;
}

bool validarVarchar(unsigned short longitud, string text){ // sanetizar y validar varchar

    if (text.length() > longitud){
        cout << "La capacidad maxima de caracteres es de " << longitud << ", vuelva a intentar." << endl;
        return false;
    }

    size_t comillaSimple = text.find("'");
    size_t comillasDobles = text.find("\"");

    if (comillaSimple != string::npos){
        cout << "No se permiten comillas simples, vuelve a intentar." << endl;
        return false;
    }

    if (comillasDobles != string::npos){
        cout << "No se permiten comillas dobles, vuelve a intentar." << endl;
        return false;
    }

    return true;
}

bool validarCedula(unsigned int cedula){

    size_t digitos = to_string(cedula).length();

    if (digitos < 8 || digitos > 10){
        cout << "La cedula debe contener de 8 a 10 numeros." << endl;
        return false;
    }

    return true;
}

bool validarNombreYApellido(string nombre){

    regex regexNombre("^[A-Za-zÀ-ÖØ-öø-ÿ'\\s-]+$");

    if (!regex_match(nombre, regexNombre)){
        cout << "El nombre o apellido no es valido" << endl;
        return false;
    }

    return true;
}

