#include "headers/sqlprovider.h"

#include <QtSql>
#include <QMessageBox>

SqlProvider::SqlProvider()
{
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

QStringList SqlProvider::findAllWords()
{
    QSqlQuery query;
    QStringList result;
    if (!query.exec("select * from words")) {
        QMessageBox::warning(0, "Error", "Can't delete word from db");
    }

    QSqlRecord rec = query.record();
    while (query.next()) {
        result << query.value(rec.indexOf("word")).toString();
    }

    return result;
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
