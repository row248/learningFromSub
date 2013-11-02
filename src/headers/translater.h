#ifndef TRANSLATER_H
#define TRANSLATER_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtMultimedia/QMediaPlayer>

class Translater : public QObject
{
    Q_OBJECT
public:
    Translater();
    void translate(QString word);
    void sound(QString word);

signals:
    void gotTranslate(QString str);

public slots:
    void translate_reply(QNetworkReply* reply);

private:
    QMediaPlayer *mplayer;
};

#endif // TRANSLATER_H
