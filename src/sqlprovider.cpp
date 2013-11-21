#include "headers/sqlprovider.h"

#include <QtSql>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>
#include <QDate>

SqlProvider::SqlProvider()
{
    prepare();
}

bool SqlProvider::has_word(QString word)
{
    QSqlQuery query;
    query.prepare("select * from words where word=?");
    query.bindValue(0, word);
    execSql(query);

    // Has any result?
    return query.next() ? true : false;
}

bool SqlProvider::addWord(QString word)
{
    QSqlQuery query;
    query.prepare("insert into words (word, added_time, last_seen_time, show_count) values (?,?,?,?)");
    query.bindValue(0, word);
    query.bindValue(1, timestamp());
    query.bindValue(2, timestamp());
    query.bindValue(3, 1); // First "show count"

    return execSql(query) ? true : false;
}

bool SqlProvider::deleteWord(QString word)
{
    QSqlQuery query;
    query.prepare("delete from words where word=?");
    query.bindValue(0, word);

    return execSql(query) ? true : false;
}

void SqlProvider::updateWordStats(QString word)
{
    if (has_word(word)) {
        if (minutesFromLastSeen(word) > 20) {
            QSqlQuery query;
            query.prepare("update words set last_seen_time=?, show_count=? where word=?");
            query.bindValue(0, timestamp());
            query.bindValue(1, getShowCount(word) + 1);
            query.bindValue(2, word);
            execSql(query);
        }
    }
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
        if (!query.exec(word_table)) {
            QMessageBox::warning(0, "Error", "Can't create table `words`");
        }
    }
}

uint SqlProvider::timestamp()
{
    QDateTime time = QDateTime(QDate::currentDate(), QTime::currentTime());
    QDateTime local = time.toLocalTime();
    return local.toTime_t();
}

uint SqlProvider::minutesFromLastSeen(QString word)
{
    QSqlQuery query;
    query.prepare("select * from words where word=?");
    query.bindValue(0, word);
    execSql(query);
    QSqlRecord rec = query.record();

    // First result
    query.first();
    uint time = query.value(rec.indexOf("last_seen_time")).toUInt();
    return (timestamp() - time) / 60;

}

uint SqlProvider::getShowCount(QString word)
{
    QSqlQuery query;
    query.prepare("select * from words where word=?");
    query.bindValue(0, word);
    execSql(query);
    QSqlRecord rec = query.record();

    // First result
    query.first();
    return query.value(rec.indexOf("show_count")).toUInt();

}

bool SqlProvider::execSql(QSqlQuery &query)
{
    if (!query.exec()) {
        QMessageBox::warning(0, "Error", query.lastError().text());
        return false;
    }
    return true;
}
