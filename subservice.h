#ifndef SUBSERVICE_H
#define SUBSERVICE_H

#include <QString>
#include <QStringList>
#include <QMap>

struct WordInfo {
    QString word;
    int index;
    int count;
};

class SubService : public QObject
{
Q_OBJECT

public:
    void setFileName(QString _filename);
    WordInfo next();
    WordInfo prev();
    WordInfo current();

private:
    // Return QMap without dublicate words with ther appearance count
    QMap<QString, int> eatFile();

public slots:
    // Functions to fill @words
    void mostRare();
    void mostOften();
    void random();

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