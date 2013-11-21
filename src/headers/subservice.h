#ifndef SUBSERVICE_H
#define SUBSERVICE_H

#include <QString>
#include <QStringList>
#include <QMap>

const QString NOTHING_FOUND = "Nothing found!";

struct WordInfo {
    QString word;
    int index;
    int count;
};

class SqlProvider;

class SubService : public QObject
{
Q_OBJECT

public:
    void setFileName(QString _filename);
    WordInfo current();
    bool is_favorite(SqlProvider &db);

private:
    // Return QMap without dublicate words with their appearance count
    QMap<QString, int> eatFile();

public slots:
    // Functions to fill @words
    void mostRare();
    void mostOften();
    void next();
    void prev();
    void random();

    // If no match exists, i add word @NOTHING_FOUND to @words
    bool matchDb(SqlProvider &db);
    bool matchDbRandom(SqlProvider &db);

private:
    WordInfo fillWordInfo();

private:
    QString filename;
    QStringList words;

    // Store index for @words
    int index;
    int count;

};

#endif // SUBSERVICE_H
