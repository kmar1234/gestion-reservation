#include "reservation.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QDate>

Reservation::Reservation() {}

Reservation::Reservation(int num, QDate dateDebut, QDate dateFin, QString type) {
    this->num = num;
    this->dateDebut = dateDebut;
    this->dateFin = dateFin;
    this->type = type;
}
bool Reservation::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO RESERVATION (NUM, DATE_DEBUT, DATE_FIN, TYPE) " "VALUES (:NUM, :DATE_DEBUT, :DATE_FIN, :TYPE)");
    query.bindValue(":NUM", num);
    query.bindValue(":DATE_DEBUT", dateDebut);
    query.bindValue(":DATE_FIN", dateFin);
    query.bindValue(":TYPE", type);
    return query.exec();
}

bool Reservation::modifier(int idr) {
    QSqlQuery query;
    query.prepare("UPDATE RESERVATION SET NUM = :NUM, DATE_DEBUT = :DATE_DEBUT, DATE_FIN = :DATE_FIN, TYPE = :TYPE " "WHERE ID = :ID");
    query.bindValue(":ID", idr);
    query.bindValue(":NUM", num);
    query.bindValue(":DATE_DEBUT", dateDebut);
    query.bindValue(":DATE_FIN", dateFin);
    query.bindValue(":TYPE", type);
    return query.exec();
}

bool Reservation::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM RESERVATION WHERE ID = :ID");
    query.bindValue(":ID", id);
    return query.exec();
}

QSqlQueryModel* Reservation::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESERVATION");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NUM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    return model;
}

QSqlQueryModel* Reservation::trier(QString condition, bool ascendant) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString order = ascendant ? "ASC" : "DESC";

    if (condition == "dateDebut") {
        model->setQuery("SELECT * FROM RESERVATION ORDER BY DATE_DEBUT " + order);
    } else if (condition == "dateFin") {
        model->setQuery("SELECT * FROM RESERVATION ORDER BY DATE_FIN " + order);
    } else if (condition == "nom") {
        model->setQuery("SELECT * FROM RESERVATION ORDER BY NUM " + order);
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NUM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));

    return model;
}

void Reservation::recherche(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM RESERVATION WHERE ID = :ID");
    query.bindValue(":ID", id);

    if (query.exec() && query.next()) {
        this->num = query.value("NUM").toInt();
        this->dateDebut = query.value("DATE_DEBUT").toDate();
        this->dateFin = query.value("DATE_FIN").toDate();
        this->type = query.value("TYPE").toString();
    }
}

QMap<QString, int> Reservation::statistiquesParType() {
    QMap<QString, int> stats;
    QSqlQuery query("SELECT type, COUNT(*) FROM RESERVATION GROUP BY type");
    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        stats[type] = count;
    }
    return stats;
}
