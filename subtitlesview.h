#ifndef SUBTITLESVIEW_H
#define SUBTITLESVIEW_H

#include <QWidget>

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

signals:
    void setStatusBar(QString str);

private:
    void init();

private:
    Ui::SubtitlesView *ui;
    QString filename;
};

#endif // SUBTITLESVIEW_H
