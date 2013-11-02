#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QtSql>

class SqlProvider
{
public:
    SqlProvider();
    bool has_word(QString word);
    bool addWord(QString word);
    bool deleteWord(QString word);

private:
    void prepare();

private:
    QSqlDatabase db;
};

#endif // SQLPROVIDER_H
