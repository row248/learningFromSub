#ifndef TRANSLATER_H
#define TRANSLATER_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>

class Translater : public QObject
{
    Q_OBJECT
public:
    Translater();
    void translate(QString word);

signals:
    void gotTranslate(QString str);

public slots:
    void reply(QNetworkReply* reply);

private:
    QNetworkAccessManager* nam;
    QUrl url;
};

#endif // TRANSLATER_H
