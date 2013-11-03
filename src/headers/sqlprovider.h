#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QtSql>
#include <QStringList>

class SqlProvider
{
public:
    SqlProvider();
    bool has_word(QString word);
    bool addWord(QString word);
    bool deleteWord(QString word);
    QStringList findAllWords();

private:
    void prepare();

private:
    QSqlDatabase db;
};

#endif // SQLPROVIDER_H
