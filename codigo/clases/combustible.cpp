#include "combustible.h"

Combustible::Combustible(QSqlDatabase& db_) : db(db_) {}


void Combustible::guardarCombustible(unsigned short idRegion, unsigned short idCategoria){ // guardar el valor por litro en cada region por categoria

    QSqlQuery query(db);

    query.prepare("SELECT id_combustible FROM tbl_combustible "
                  "WHERE id_categoria = ? and id_region = ?");
    query.bindValue(0, idCategoria);
    query.bindValue(1, idRegion);

    if (query.exec() && query.next()){ // existen datos
        setIdCombustible(query.value(0).toUInt());

        query.prepare("UPDATE tbl_combustible SET valor_litro = ? WHERE id_combustible = ?");
        query.bindValue(0, getValorLitro());
        query.bindValue(1, getIdCombustible());

        if (query.exec()) return;
        else{
            cout << "No se pudo actualizar los datos." << endl;
            return;
        }

    }else{ // no existen datos
        query.prepare("INSERT INTO tbl_combustible"
                      "(valor_litro, id_categoria, id_region)"
                      "VALUES (?, ?, ?)");

        query.bindValue(0, getValorLitro());
        query.bindValue(1, idCategoria);
        query.bindValue(2, idRegion);

        if (query.exec()) return;
        else{
            cout << "Erro al guardar los datos." << endl;
            return;
        }
    }
}

void Combustible::setIdCombustible(unsigned short idCombustible){
    this->idCombustible = idCombustible;
}

void Combustible::setValorLitro(float valorLitro){
    this->valorLitro = valorLitro;
}

unsigned short Combustible::getIdCombustible() const{
    return idCombustible;
}

float Combustible::getValorLitro() const{
    return valorLitro;
}

Combustible::~Combustible(){

}
