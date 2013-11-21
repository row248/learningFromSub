#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QtSql>
#include <QStringList>
#include <QDateTime>

class SqlProvider
{
public:
    SqlProvider();
    bool has_word(QString word);
    bool addWord(QString word);
    bool deleteWord(QString word);
    void updateWordStats(QString word);
    QStringList findAllWords();

private:
    void prepare();
    uint timestamp();
    uint minutesFromLastSeen(QString word);
    uint getShowCount(QString word);
    bool execSql(QSqlQuery &query);

private:
    QSqlDatabase db;
    QString word_table = "create table words ("
            "id integer primary key,"
            "word varchar(255),"
            "added_time integer not null,"
            "last_seen_time integer not null,"
            "show_count integer not null"
            ");";
};

#endif // SQLPROVIDER_H
