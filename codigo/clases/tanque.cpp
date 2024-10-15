#include "tanque.h"
#include "estacion.h"

Tanque::Tanque(): estacion(nullptr), db(nullptr){}

Tanque::Tanque(class estacion* estacion_, QSqlDatabase* db_) : estacion(estacion_), db(db_) {

    QSqlQuery query(*db);

    query.prepare("SELECT id_tanque, capacidad_tanque FROM tbl_tanque WHERE id_estacion = ?");
    query.bindValue(0, estacion->getId());

    if (query.exec()){
        if (query.next()){
            unsigned int id = query.value(0).toUInt();
            float capacidadTanque = query.value(1).toFloat();

            setIdTanque(id);
            setCapacidadTanque(capacidadTanque);
        }
    }
}

void Tanque::crearTanque(){

    QSqlQuery query(*db);
    query.prepare("INSERT INTO tbl_tanque (id_estacion) VALUES(?)");
    query.bindValue(0, estacion->getId());

    if (!query.exec()){
        cout << "Error al guardar un tanque" << endl;
        return;
    }
}

bool Tanque::eliminarTanque(){

    QSqlQuery query(*db);
    query.prepare("DELETE FROM tbl_tanque WHERE id_estacion = ?");
    query.bindValue(0, estacion->getId());

    if (query.exec()) return true;
    else cout << query.lastError().text().toStdString() << endl;

    return false;
}

void Tanque::asignarCapacidadAleatoriaDelTanque() {

    QSqlQuery query(*db);

    // Obtener el ID del tanque para la estación actual
    query.prepare("SELECT id_tanque FROM tbl_tanque WHERE id_estacion = ?");
    query.bindValue(0, estacion->getId());

    if (!query.exec() || !query.next()) {  // Agregar validación de next() para obtener un valor
        cout << "Error al obtener el tanque." << endl;
        return;
    }

    unsigned int idTanque = query.value(0).toUInt();

    // Verificar si ya se asignó la capacidad del tanque
    query.prepare("SELECT COUNT(*) FROM tbl_tanque_categoria WHERE id_tanque = ?");
    query.bindValue(0, idTanque);

    if (!query.exec() || !query.next()) {
        cout << "Error al verificar la capacidad del tanque." << endl;
        return;
    }

    int existe = query.value(0).toInt();

    if (existe > 0) {
        cout << "Ya se asignó la capacidad del tanque de suministro." << endl;
        return;
    }

    // Obtener las categorías de combustible desde la base de datos
    query.prepare("SELECT id_categoria FROM tbl_categoria");
    if (!query.exec()) {
        cout << "Error al obtener las categorías de combustible." << endl;
        return;
    }

    // Asignar capacidades aleatorias a cada categoría
    unsigned int capacidadTotalTanque = 0;
    while (query.next()) {
        unsigned int idCategoria = query.value(0).toUInt();
        unsigned short capacidad = 100 + rand() % 101;  // Capacidad aleatoria entre 100 y 200
        capacidadTotalTanque += capacidad;

        // Insertar en la tabla tbl_tanque_categoria
        QSqlQuery insertQuery(*db);
        insertQuery.prepare("INSERT INTO tbl_tanque_categoria (id_tanque, id_categoria, capacidad_categoria) VALUES (?, ?, ?)");
        insertQuery.bindValue(0, idTanque);
        insertQuery.bindValue(1, idCategoria);
        insertQuery.bindValue(2, capacidad);

        if (!insertQuery.exec()) {
            cerr << "Error al insertar en tbl_tanque_categoria: " << insertQuery.lastError().text().toStdString() << endl;
            return;
        }
    }

    // Actualizar la capacidad total del tanque en la tabla tbl_tanque
    QSqlQuery updateQuery(*db);
    updateQuery.prepare("UPDATE tbl_tanque SET capacidad_tanque = ? WHERE id_estacion = ?");
    updateQuery.bindValue(0, capacidadTotalTanque);
    updateQuery.bindValue(1, estacion->getId());

    if (!updateQuery.exec()) {
        cout << "Error al actualizar la capacidad total del tanque." << endl;
        return;
    }

    cout << "-----------------------------------------------" << endl;
    cout << "| Capacidad del tanque asignada correctamente |" << endl;
    cout << "-----------------------------------------------" << endl;
}

void Tanque::verificarFugas() {
    QSqlQuery query(*db);

    // Obtener la capacidad original del tanque por categoria
    query.prepare(
        "SELECT tc.id_categoria, tc.capacidad_categoria, c.nombre, t.capacidad_tanque "
        "FROM tbl_tanque_categoria tc "
        "JOIN tbl_tanque t ON tc.id_tanque = t.id_tanque "
        "JOIN tbl_categoria c ON tc.id_categoria = c.id_categoria "
        "WHERE t.id_estacion = ?"
        );
    query.bindValue(0, estacion->getId());

    if (!query.exec()) {
        cout << "Error al obtener la capacidad original de los tanques." << endl;
        return;
    }

    while (query.next()) {
        unsigned int idCategoria = query.value(0).toUInt();
        float capacidadCategoria = query.value(1).toFloat();
        string nombreCategoria = query.value(2).toString().toStdString();

        QSqlQuery ventasQuery(*db);
        ventasQuery.prepare(
            "SELECT SUM(v.cantidad_combustible) "
            "FROM tbl_venta v "
            "JOIN tbl_surtidor s ON v.id_surtidor = s.id_surtidor "
            "WHERE v.id_categoria = ? AND s.id_surtidor = ?"
            );
        ventasQuery.bindValue(0, idCategoria);
        ventasQuery.bindValue(1, estacion->getId());

        if (!ventasQuery.exec() || !ventasQuery.next()) {
            cout << "Error al obtener la cantidad vendida para la categoría." << endl;
            continue;
        }

        float cantidadVendida = ventasQuery.value(0).toFloat();

        // Obtener la cantidad de combustible almacenada en el tanque
        QSqlQuery almacenQuery(*db);

        almacenQuery.prepare(
            "SELECT capacidad_tanque "
            "FROM tbl_tanque "
            "WHERE id_estacion = ?"
            );
        almacenQuery.bindValue(0, estacion->getId());

        if (!almacenQuery.exec() || !almacenQuery.next()) {
            cout << "Error al obtener la cantidad almacenada para la categoría." << endl;
            continue;
        }

        float cantidadAlmacenada = almacenQuery.value(0).toFloat();

        // Verificar si lo vendido más lo almacenado es menor al 95% de la capacidad original
        float totalActual = cantidadVendida + cantidadAlmacenada;
        float capacidadMinima = capacidadCategoria * 0.95;

        if (totalActual < capacidadMinima) {
            cout << "--------------------------" << endl;
            cout << "| Posible fuga detectada en la categoria: " << nombreCategoria << " |" << endl;
            cout << "--------------------------" << endl;
        } else {
            cout << "---------------------------" << endl;
            cout << "| No hay fugas detectadas en la categoria: " << nombreCategoria << " |" << endl;
            cout << "---------------------------" << endl;
        }
    }

}

//setter
void Tanque::setIdTanque(unsigned int idTanque){
    this->idTanque = idTanque;
}
void Tanque::setCapacidadTanque(float capacidadTanque){
    this->capacidadTanque = capacidadTanque;
}

//getter

unsigned int  Tanque::getIdTanque() const{
    return this->idTanque;
}

float Tanque::getCapacidadTanque() const{
    return this->capacidadTanque;
}


Tanque::~Tanque(){

    // if (estacion){
    //     delete estacion;
    //     estacion = nullptr;
    // }

    // if (db){
    //     delete db;
    //     db = nullptr;
    // }

}
