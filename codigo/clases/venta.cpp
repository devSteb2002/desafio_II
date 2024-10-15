#include "venta.h"
#include <iomanip>

Venta::Venta(class Surtidor &surtidor_ , QSqlDatabase& db_): surtidor(surtidor_), db(db_){

}


bool Venta::añadirVenta(unsigned short idCategoria, unsigned int idCliente, unsigned int idSurtidor){

    QSqlQuery query(db);

    query.prepare("INSERT INTO tbl_venta "
    " (fecha, hora, metodo_pago, cantidad_combustible, cantidad_dinero, total_venta, id_categoria, id_cliente, id_surtidor) "
    " VALUES(?,?,?,?,?,?,?,?,?)");

    query.bindValue(0, QString::fromStdString(getFecha()));
    query.bindValue(1, QString::fromStdString(getHora()));
    query.bindValue(2, QString::fromStdString(getMetodoPago()));
    query.bindValue(3, getCantidadCombustible());
    query.bindValue(4, getCantidadDinero());
    query.bindValue(5, getTotalVenta());
    query.bindValue(6, idCategoria);
    query.bindValue(7, idCliente);
    query.bindValue(8, idSurtidor);

    if (query.exec()){
        return true;
    }else return false;
}

bool Venta::eliminarVentas(unsigned int *&ids, unsigned int tamaño){ //eliminar todas las ventas
    QSqlQuery query_(db);

    //eliminar todas las ventas asociadas a un id
    QString query = "DELETE FROM tbl_venta WHERE id_surtidor IN ("; //genrar la consulta inicial

    for (unsigned int i = 0; i < tamaño; i++){
        query += (i > 0) ? ",?" : "?"; // generar la cantidad de parametros necesarios
    }

    query += ")";
    query_.prepare(query);

    for (unsigned int i = 0; i < tamaño; i++){
        query_.bindValue(i, ids[i]); // llenar los paramteros necesarios
    }

    if (query_.exec()){
        return true;
    }else{
        cout << query_.lastError().text().toStdString() << endl;
        return false;
    }

    return false;
}


void Venta::calcularVentasPorES(unsigned int idRed, Categoria& categoria){ // Calcular el monto total de las ventas por E/S del pais por categoria

    unsigned int tamaño = categoria.obtenerCantidadDeCategorias(); //obtener numero de categorias

    if (tamaño > 0){

        QSqlQuery query(db);
        query.prepare("SELECT  est.id_estacion, est.nombre, cat.nombre, SUM(vent.total_venta) FROM tbl_venta AS vent "
         "INNER JOIN tbl_surtidor AS sur ON sur.id_surtidor = vent.id_surtidor "
         "INNER JOIN tbl_estacion AS est ON est.id_estacion = sur.id_estacion "
         "INNER JOIN tbl_categoria AS cat ON vent.id_categoria = cat.id_categoria "
        "WHERE sur.activo = ? and est.id_red = ? GROUP BY vent.id_categoria, est.id_estacion ORDER BY est.id_estacion");

        query.bindValue(0, 1);
        query.bindValue(1, idRed);

        if (query.exec()){

            unsigned int idAnterior = 0;
            string nEstacion = "";
            bool existe = false;

            cout << "-----------------------------------------------------" << endl;
            cout << "|          MONTO POR ESTACION Y CATEGORIA            |" << endl;
            cout << "-----------------------------------------------------" << endl;

            while (query.next()){
                unsigned const int idEstacion = query.value(0).toUInt();
                string const nombreEstacion = query.value(1).toString().toStdString();
                string const nombreCategoria = query.value(2).toString().toStdString();
                unsigned const int monto = query.value(3).toUInt();

                nEstacion = nombreEstacion;

                if (idEstacion == idAnterior){
                    nEstacion = "";
                    cout << left << "" << nEstacion;
                }
                else {
                    cout << endl;
                    cout << left <<  setw(15)  << nEstacion << " | ";
                }

                cout << setw(5) << nombreCategoria <<  ":  " ;
                cout << setw(5) <<  monto << " | ";

                idAnterior = idEstacion;
                existe = true;
            }

            if (!existe)cout << "    No se encuetran registros de ventas     " << endl;

            cout << endl;

        }else cout << query.lastError().text().toStdString() << endl;

    }else{
        cout << "No se encutran categorias existentes." << endl;
    }

}

void Venta::litrosDeCombustibleVendidosPorCategoria() {

    QSqlQuery query(db);

    query.prepare(
        "SELECT c.nombre, SUM(v.cantidad_combustible) as total_litros "
        "FROM tbl_venta v "
        "JOIN tbl_categoria c ON v.id_categoria = c.id_categoria "
        "GROUP BY c.nombre"
        );

    if (query.exec()) {
        cout << "---------------------------------" << endl;
        cout << "| Litros vendidos por categoria |" << endl;
        cout << "---------------------------------" << endl;
        while (query.next()) {
            string categoria = query.value(0).toString().toStdString();
            double litrosVendidos = query.value(1).toDouble();

            cout << "Categoria: " << categoria << endl;
            cout << "Litros vendidos: " << litrosVendidos << " L" << endl;
            cout << "-----------------------------------" << endl;
        }
    } else {
        cout << "Error al consultar litros vendidos." << endl;
    }

}


//Setter
void Venta::setIdVenta(unsigned int idVenta){
    this->idVenta = idVenta;
}

void Venta::setFecha(string fecha){
    this->fecha = fecha;
}

void Venta::setHora(string hora){
    this->hora = hora;
}

void Venta::setMetodoPago(string metodoPago){
    this->metodoPago = metodoPago;
}

void Venta::setCantidadCombustible(float cantidadCombustible){
    this->cantidadCombustible = cantidadCombustible;
}
void Venta::setCantidadDinero(float cantidadDinero){
    this->cantidadDinero = cantidadDinero;
}

void Venta::setTotalVenta(float totalVenta){
    this->totalVenta = totalVenta;
}

//Getter
unsigned int Venta::getIdVenta() const{
    return this->idVenta;
}

string Venta::getFecha() const{
    return this->fecha;
}

string Venta::getHora() const{
    return this->hora;
}

string Venta::getMetodoPago() const{
    return this->metodoPago;
}

float Venta::getCantidadCombustible() const{
    return this->cantidadCombustible;
}

float Venta::getCantidadDinero() const{
    return this->cantidadDinero;
}

float Venta::getTotalVenta() const{
    return this->totalVenta;
}


Venta::~Venta(){

}
