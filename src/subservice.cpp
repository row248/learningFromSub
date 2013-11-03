#include "headers/subservice.h"
#include "headers/sqlprovider.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QSet>
#include <QMap>
#include <QTime>

void SubService::setFileName(QString _filename)
{
    filename = _filename;
    index = 0;

    // Default behaivor
    mostRare();
}

WordInfo SubService::next()
{
    index++;
    return fillWordInfo();
}


WordInfo SubService::prev()
{
    index--;
    return fillWordInfo();
}

WordInfo SubService::current()
{
    return fillWordInfo();
}

bool SubService::is_favorite(SqlProvider &db)
{
    return db.has_word( fillWordInfo().word );
}

QMap<QString, int> SubService::eatFile()
{
    // reset index
    index = 0;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0, "Error", file.errorString());

    QTextStream in(&file);
    QString fileData = in.readAll();
    QRegExp rx("(\\w{4,})");
    QStringList result;
    int pos = 1;

    while ((pos = rx.indexIn(fileData, pos)) != -1) {
        result << rx.cap(1).toLower();
        pos += rx.matchedLength();
    }

    QMap<QString, int> appearance;

    foreach(QString word, result) {
        if (appearance.contains(word)) {
            appearance[word]++;
        } else {
            appearance.insert(word, 1);
        }
    }

    count = appearance.count();

    return appearance;
}

void SubService::mostRare()
{
    QMap<QString, int> appearance = eatFile();

    words = appearance.keys();
    qSort(words.begin(), words.end(), [&appearance](QString a, QString b) {
        return appearance[a] < appearance[b];
    });
}

void SubService::mostOften()
{
    QMap<QString, int> appearance = eatFile();

    words = appearance.keys();
    qSort(words.begin(), words.end(), [&appearance](QString a, QString b) {
        return appearance[a] > appearance[b];
    });
}

void SubService::random()
{
    QMap<QString, int> appearance = eatFile();

    words = appearance.keys();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for (int i=words.count()-1; i>0; --i) {
        int random1 = qrand() % words.count();
        int random2 = qrand() % words.count();
        qSwap(words[random1], words[random2]);
    }
}

void SubService::matchDb(SqlProvider &db)
{
    QMap<QString, int> appearance = eatFile();

    QStringList wordsFromFile = appearance.keys();
    QStringList db_words = db.findAllWords();

    // Delete old values
    words.clear();

    for (int i=0; i < db_words.count(); ++i) {
        if (wordsFromFile.contains(db_words[i])) {
            words.append( db_words[i] );
        }
    }

    // Because count setted in @eatFile method
    count = words.count();
}

WordInfo SubService::fillWordInfo()
{
    if (index < 0) {
        index = 0;
    } else if (index >= count) {
        index = count - 1;
    }

    WordInfo word;
    word.word = words.at(index);
    word.index = index;
    word.count = count;

    return word;
}
