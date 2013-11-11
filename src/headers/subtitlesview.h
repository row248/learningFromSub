#ifndef SUBTITLESVIEW_H
#define SUBTITLESVIEW_H

#include <QWidget>
#include <QLabel>

#include "headers/sqlprovider.h"

class SubService;
struct WordInfo;
class Translater;

namespace Ui {
class SubtitlesView;
}

class SubtitlesView : public QWidget
{
    Q_OBJECT

public:
    explicit SubtitlesView(QWidget *parent = 0);
    ~SubtitlesView();
    void setFileName(QString str);

public slots:
    void nextWord();
    void previousWord();
    void playSound();
    void matchDb();
    void favorite();

    // If pressed key
    void showTranslate(QString str);

    // If clicked button
    void showTranslate();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void init();
    void updateUi(WordInfo &info);

public slots:
    void update();
    void enableIcon();

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
