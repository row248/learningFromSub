#include "translater.h"

#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

Translater::Translater()
{
}

void Translater::translate(QString word)
{
    // From eng to rus
    url = "http://www.translate.google.com/translate_a/t?client=x&text=" + word + "&hl=en&sl=en&tl=ru";
    nam = new QNetworkAccessManager();
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply(QNetworkReply*)));

    nam->get(QNetworkRequest(url));
}

void Translater::reply(QNetworkReply *reply)
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
