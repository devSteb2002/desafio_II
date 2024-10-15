#include "surtidor.h"
#include <iomanip>

Surtidor::Surtidor(QSqlDatabase& db_, class estacion& estacion_): db(db_), estacion(estacion_) {}


int Surtidor::obtenerCantidadActivos(){

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(id_surtidor) FROM tbl_surtidor WHERE id_estacion = ? and activo = ?");
    query.bindValue(0, estacion.getId());
    query.bindValue(1, 1);

    if (query.exec()){
        if (query.next()){
            int cantidad = query.value(0).toInt();
            return cantidad;
        }
    }

    return -1;
}

unsigned int* Surtidor::obtenerSurtidores(unsigned int &tamaño) {

    QSqlQuery query(db);
    unsigned int cantidad = 0;

    query.prepare("SELECT COUNT(id_surtidor) FROM tbl_surtidor WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (!query.exec()) {
        cout << "Error al consultar los datos" << endl;
        return nullptr;
    }

    if (query.next()) cantidad = query.value(0).toUInt();

    if (cantidad == 0) {
        cout << "No hay surtidores en esta estacion." << endl;
        return nullptr;
    }

    unsigned int* ids = new unsigned int[cantidad];
    unsigned int iterador = 0;

    query.prepare("SELECT id_surtidor, modelo, activo FROM tbl_surtidor WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (!query.exec()) {
        cout << "No se pudo consultar los surtidores de la estacion." << endl;
        delete[] ids;
        return nullptr;
    }

    cout << "-------------------------------------" << endl;
    cout << "|     Surtidores de la estacion     |" << endl;
    cout << "-------------------------------------" << endl;
    cout << left << setw(8) << "ID" << " | " << setw(15) << "Modelo" << " | " << setw(10) << "Estado" << endl;
    cout << "-------------------------------------" << endl;

    while (query.next()) {
        unsigned int const idSurtidor = query.value(0).toUInt();
        string modelo = query.value(1).toString().toStdString();
        bool activo = query.value(2).toBool();
        ids[iterador] = query.value(0).toUInt();

        cout << left << setw(8) << idSurtidor << " | "
             << setw(15) << modelo << " | "
             << setw(10) << (activo ? "Activo" : "Inactivo") << endl;
        iterador++;
    }

    cout << "--------------------------------------" << endl;

    tamaño = cantidad;

    return ids;

}

unsigned int Surtidor::agregarSurtidor() {

    QSqlQuery query(db);

    int cantidadDeSurtidores = obtenerCantidadActivos();

    if (cantidadDeSurtidores >= 12) {
        cout << "No se puede agregar mas surtidores. Se ha alcanzado el limite maximo de 12." << endl;
        return 0;
    }

    query.prepare("SELECT id_nave FROM tbl_nave WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (!query.exec()) {
        cout << "Error al obtener las naves de la estación." << endl;
        return 0;
    }

    int cantidadNaves = 0;
    while(query.next()) {
        cantidadNaves++;
    }

    if (cantidadNaves == 0) {
        cout << "No hay naves disponibles en la estacion." << endl;
        return 0;
    }

    unsigned int* naves = new unsigned int[cantidadNaves];
    query.exec();

    int index = 0;
    while (query.next()) {
        naves[index++] = query.value(0).toUInt();
    }

    // Seleccionar una nave aleatoria
    unsigned int idNaveAleatoria = naves[rand() % cantidadNaves];

    query.prepare("INSERT INTO tbl_surtidor (modelo, activo, id_estacion) VALUES (?, ?, ?)");
    query.bindValue(0, QString::fromStdString(getModelo()));
    query.bindValue(1, getActivo());
    query.bindValue(2, estacion.getId());

    if (!query.exec()) {
        cerr << "Error al insertar en la bd: " << query.lastError().text().toStdString() << endl;
        return 0;
    }



    unsigned int idSurtidor = query.lastInsertId().toUInt();

    query.prepare("INSERT INTO tbl_nave_surtidor (id_surtidor, id_nave) VALUES (?, ?)");
    query.bindValue(0, idSurtidor);
    query.bindValue(1, idNaveAleatoria);

    if (!query.exec()) {
        cout << "Error al agregar el surtidor a la nave." << endl;
        return 0;
    }

    delete[] naves;

    return idSurtidor;
}

bool Surtidor::verificarExistencia(){ // verificar si existen surtidores en esta esta estacion
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM tbl_surtidor WHERE id_estacion = ?");
    query.bindValue(0, estacion.getId());

    if (query.exec()){
        if (query.next()){
            unsigned int cantidad = query.value(0).toUInt();

            if (cantidad == 0) return false;
            else return true;
        }
    }

    return false;
}

bool Surtidor::eliminarSurtidores(Venta &venta){ //eliminar surtidor inactivos
    QSqlQuery query(db);

    //consultar cantidad de inactivos
    query.prepare("SELECT COUNT(id_surtidor) FROM tbl_surtidor WHERE activo = ? and id_estacion = ?");
    query.bindValue(0, 0);
    query.bindValue(1, estacion.getId());

    if (query.exec()){
        if (query.next()){
            unsigned int cantidad = query.value(0).toUInt();

            if (cantidad != 0) { //Existen surtidores inactivos

                unsigned int *idsSurtidor = new unsigned int[cantidad];
                unsigned int iterador = 0;

                query.prepare("SELECT id_surtidor FROM tbl_surtidor WHERE activo = ? and id_estacion = ?"); // buscar todos los surtidores
                query.bindValue(0, 0);
                query.bindValue(1, estacion.getId());

                if (query.exec()){
                    while(query.next()){
                        unsigned int id = query.value(0).toUInt();
                        idsSurtidor[iterador] = id;
                        iterador++;
                    }

                    //eliminar las ventas asociadas a los ids de surtidores inactivos
                    bool ventasEliminadas = venta.eliminarVentas(idsSurtidor, iterador);
                    delete[] idsSurtidor;

                    if (ventasEliminadas){ // se eliminaron correctamente
                        //eliminar los surtidores

                        query.prepare("DELETE FROM tbl_surtidor WHERE id_estacion = ?");
                        query.bindValue(0, estacion.getId());

                        if (query.exec()) return true;
                        else {
                            cout << query.lastError().text().toStdString() << endl;
                            return false;
                        }
                    }

                }else{
                    delete[] idsSurtidor;
                    cout << query.lastError().text().toStdString() << endl;
                    return false;
                }
            }else{
                //no hay existencia de sutidores inactivos por lo tanto no hay ventas
                return true;
            }
        }

    }

    cout << query.lastError().text().toStdString() << endl;
    return false;
}



bool Surtidor::eliminarSurtidor() { // eliminar un surtidor en especifico
    QSqlQuery query(db);
    query.prepare("DELETE FROM tbl_surtidor WHERE id_surtidor = ?");
    query.bindValue(0, getId());

    if (query.exec()) return true;

    return false;
}

void Surtidor::desactivarSurtidor() {

    QSqlQuery query(db);

    query.prepare("SELECT activo FROM tbl_surtidor WHERE id_surtidor = ?");
    query.bindValue(0, getId());

    if (query.exec()) {
        if (query.next()) {
            bool activo = query.value(0).toBool();

            if (!activo) {
                cout << "El surtidor ya se encuentra desactivado." << endl;
                return;
            } else {
                query.prepare("UPDATE tbl_surtidor SET activo = false WHERE id_surtidor = ?");
                query.bindValue(0, getId());

                if (query.exec()) {
                    cout << "Surtidor desactivado correctamente." << endl;
                } else {
                    cout << "Error al desactivar el surtidor." << endl;
                }
                return;
            }
        } else {
            cout << "No se encontró el surtidor con ese ID." << endl;
        }
    } else {
        cout << "Error al consultar el estado del surtidor." << endl;
    }
}

void Surtidor::activarSurtidor() {
    QSqlQuery query(db);

    query.prepare("SELECT activo FROM tbl_surtidor WHERE id_surtidor = ?");
    query.bindValue(0, getId());

    if (query.exec()) {
        if (query.next()) {
            bool activo = query.value(0).toBool();

            if (activo) {
                cout << "El surtidor ya se encuentra activo." << endl;
                return;
            } else {
                query.prepare("UPDATE tbl_surtidor SET activo = true WHERE id_surtidor = ?");
                query.bindValue(0, getId());

                if (query.exec()) {
                    cout << "Surtidor activado correctamente." << endl;
                } else {
                    cout << "Error al activar el surtidor." << endl;
                }
                return;
            }
        } else {
            cout << "No se encontró el surtidor con ese ID." << endl;
        }
    } else {
        cout << "Error al consultar el estado del surtidor." << endl;
    }
}

void Surtidor::historicoTransacciones() {
    QSqlQuery query(db);

    query.prepare(
        "SELECT v.fecha, v.hora, v.metodo_pago, v.cantidad_combustible, v.cantidad_dinero, c.nombre, cl.nombre, cl.apellido, cl.cedula "
        "FROM tbl_venta v "
        "JOIN tbl_surtidor s ON v.id_surtidor = s.id_surtidor "
        "JOIN tbl_categoria c ON v.id_categoria = c.id_categoria "
        "JOIN tbl_cliente cl ON v.id_cliente = cl.id_cliente "
        "WHERE s.id_surtidor = ? "
        "ORDER BY v.fecha DESC"
        );


    query.bindValue(0, getId());

    if (!query.exec()) {
        cout << "Error al consultar las transacciones" << endl;
        return;
    }

    cout << setw(12) << left << "Fecha"
         << setw(10) << left << "Hora"
         << setw(15) << left << "Metodo Pago"
         << setw(20) << right << "Combustible (L)"
         << setw(18) << right << "Monto ($)"
         << setw(15) << left << "Categoria"
         << setw(25) << left << "Cliente"
         << setw(15) << left << "Cedula"
         << endl;
    cout << string(130, '-') << endl;

    int totalTransacciones = 0;
    double totalLitrosVendidos = 0.0;
    double totalDineroRecaudado = 0.0;

    while (query.next()) {
        string fecha = query.value(0).toString().toStdString();
        string hora = query.value(1).toString().toStdString();
        string metodoPago = query.value(2).toString().toStdString();
        double cantidadCombustible = query.value(3).toDouble();
        double cantidadDinero = query.value(4).toDouble();
        string categoria = query.value(5).toString().toStdString();
        string clienteNombre = query.value(6).toString().toStdString();
        string clienteApellido = query.value(7).toString().toStdString();
        string clienteCedula = query.value(8).toString().toStdString();

        cout << setw(12) << left << fecha
             << setw(10) << left << hora
             << setw(15) << left << metodoPago
             << setw(20) << right << cantidadCombustible
             << setw(18) << right << cantidadDinero
             << setw(15) << left << categoria
             << setw(25) << left << clienteNombre + " " + clienteApellido
             << setw(15) << left << clienteCedula
             << endl;

        totalTransacciones++;
        totalLitrosVendidos += cantidadCombustible;
        totalDineroRecaudado += cantidadDinero;
    }

    cout << endl;
    cout << "Total de transacciones: " << totalTransacciones << endl;
    cout << "Total de litros vendidos: " << totalLitrosVendidos << " L" << endl;
    cout << "Total de dinero recaudado: $" << totalDineroRecaudado << endl;

}

//setter
void Surtidor::setId(unsigned int id){
    this->id = id;
}
void Surtidor::setModelo(string modelo){
    this->modelo = modelo;
}
void Surtidor::setActivo(bool activo){
    this->activo = activo;
}


//getters

unsigned int Surtidor::getId() const{
    return this->id;
}
string Surtidor::getModelo() const{
    return this->modelo;
}
bool Surtidor::getActivo() const{
    return this->activo;
}

Surtidor::~Surtidor(){

}
