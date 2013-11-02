#include "sqlprovider.h"

#include <QtSql>
#include <QMessageBox>

SqlProvider::SqlProvider()
{
//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
//    dbase.setDatabaseName("learning_from_sub.sqlite");
//    if (!dbase.open()) {
//        qDebug() << "Can't open db";
//        return;
//    }

//    QSqlQuery a_query;
//    QString str = "create table words ("
//            "id integer primary key,"
//            "word varchar(255)"
//            ");";
//    bool qResult = a_query.exec(str);
//    qDebug() << qResult;

//    if (!a_query.exec("SELECT * FROM words")) {
//        qDebug() << "Error while select";
//    }

//    QSqlRecord rec = a_query.record();

//    while (a_query.next()) {
//        qDebug() << a_query.value(rec.indexOf("word")).toString();
//    }
    prepare();
}

bool SqlProvider::has_word(QString word)
{
    QSqlQuery query;
    query.prepare("select * from words where word=?");
    query.bindValue(0, word);
    if (!query.exec()) {
        QMessageBox::warning(0, "Error", "Can't retrieve word from db");
        return false;
    }

    QSqlRecord rec = query.record();
    // Has any result?
    if (query.next()) {
        return true;
    }

    return false;
}

bool SqlProvider::addWord(QString word)
{
    QSqlQuery query;
    query.prepare("insert into words (word) values (?)");
    query.bindValue(0, word);
    if (!query.exec()) {
        QMessageBox::warning(0, "Error", "Can't add word to db");
        return false;
    }

    return true;
}

bool SqlProvider::deleteWord(QString word)
{
    QSqlQuery query;
    query.prepare("delete from words where word=?");
    query.bindValue(0, word);
    if (!query.exec()) {
        QMessageBox::warning(0, "Error", "Can't delete word from db");
        return false;
    }

    return true;
}

void SqlProvider::prepare()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("learning_from_sub.sqlite");
    if (!db.open()) {
        QMessageBox::warning(0, "Error", "Can't open database");
    }

    QSqlQuery query;

    if (!db.tables().contains("words")) {
        QString create_db = "create table words ("
                "id integer primary key,"
                "word varchar(255)"
                ");";
        if (!query.exec(create_db)) {
            QMessageBox::warning(0, "Error", "Can't create table `words`");
        }
    }
}
