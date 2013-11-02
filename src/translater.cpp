#include "headers/translater.h"

#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFile>

#include <QtMultimedia/QMediaPlayer>

Translater::Translater()
{
    mplayer = new QMediaPlayer();
}

void Translater::translate(QString word)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager();

    // From eng to rus
    QUrl url_toTranslate = "http://www.translate.google.com/translate_a/t?client=x&text=" + word + "&hl=en&sl=en&tl=ru";
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(translate_reply(QNetworkReply*)));

    nam->get(QNetworkRequest(url_toTranslate));
}

void Translater::sound(QString word)
{
    mplayer->setMedia(QNetworkRequest(QUrl("http://www.translate.google.com/translate_tts?ie=UTF-8&q=" + word + "&tl=en")));
    mplayer->setVolume(100);
    mplayer->play();
}

void Translater::translate_reply(QNetworkReply *reply)
{
    QJsonDocument json = QJsonDocument::fromJson( reply->readAll() );
    QString translate;

    if (!json.isNull()) {
        QJsonObject jobj = json.object();

        translate.append( "<font size=5 color=#000000>" );
        translate.append( jobj["sentences"].toArray()[0].toObject()["trans"].toString() );
        translate.append( "</font><br><br>" );

        // Arrays of noun, adjectives and etc.
        QJsonArray jarrByTimes = jobj["dict"].toArray();
        foreach (const QJsonValue &jval, jarrByTimes) {
            translate.append("<font size=4 color=#2e2e2e>");
            translate.append( jval.toObject()["pos"].toString() + "<br>" );
            translate.append("</font>");

            QJsonArray jvalOfTranslates = jval.toObject()["terms"].toArray();

            translate.append("<font color=#545454>");
            foreach (const QJsonValue &str, jvalOfTranslates) {
                translate.append( str.toString() + ", ");
            }
            // delete lasy comma
            translate.replace(translate.count() - 2, 2, "");
            translate.append("</font>");

            translate.append( "<br><br>" );
        }

    } else {
        translate.append("Nothing found :(");
    }

    emit gotTranslate(translate);
}

