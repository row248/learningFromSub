#ifndef SUBTITLESVIEW_H
#define SUBTITLESVIEW_H

#include <QWidget>
#include <QLabel>

#include "headers/sqlprovider.h"

class SubService;
class Translater;

namespace Ui {
class SubtitlesView;
}

class SubtitlesView : public QWidget
{
    Q_OBJECT

    enum Options {
        MostRare = 0,
        MostOften,
        Random,
        MatchDb,
        MatchDbRandom
    };

public:
    explicit SubtitlesView(QWidget *parent = 0);
    ~SubtitlesView();
    void setFileName(QString str);

public slots:
    void playSound();
    void favorite();
    void optionsChanged(int i);

    // If pressed key
    void showTranslate(QString str);

    // If clicked button
    void showTranslate();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void init();
    void initUi();

public slots:
    void update();

public:
    QString word;

private:
    Ui::SubtitlesView *ui;
    QString filename;
    SubService *subserv;
    QString index;
    Translater *translater;
    SqlProvider db;
};

#endif // SUBTITLESVIEW_H
