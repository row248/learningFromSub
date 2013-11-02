#include "headers/subtitlesview.h"
#include "ui_subtitlesview.h"
#include "headers/subservice.h"
#include "headers/translater.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>
#include <QKeyEvent>

SubtitlesView::SubtitlesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubtitlesView)
{
    ui->setupUi(this);
    init();
}

SubtitlesView::~SubtitlesView()
{
    delete ui;
}

void SubtitlesView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        previousWord();
        break;
    case Qt::Key_Right:
        nextWord();
        break;
    case Qt::Key_T:
        translater->translate(word);
        break;
    }
}

void SubtitlesView::setFileName(QString str)
{
    filename = str;
    subserv->setFileName(filename);
    update();
}

void SubtitlesView::nextWord()
{
    WordInfo word = subserv->next();
    updateUi(word);
}

void SubtitlesView::previousWord()
{
    WordInfo word = subserv->prev();
    updateUi(word);
}

void SubtitlesView::showTranslate(QString str)
{
    ui->translate->setHtml(str);
}

void SubtitlesView::showTranslate()
{
    translater->translate(word);
}

void SubtitlesView::playSound()
{
    translater->sound(word);
}

void SubtitlesView::update()
{
    WordInfo word = subserv->current();
    updateUi(word);
}

void SubtitlesView::init()
{
    subserv = new SubService();
    translater = new Translater();

    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(nextWord()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(previousWord()));

    connect(ui->btn_mostRare, SIGNAL(clicked()), subserv, SLOT(mostRare()));
    connect(ui->btn_mostOften, SIGNAL(clicked()), subserv, SLOT(mostOften()));
    connect(ui->btn_random, SIGNAL(clicked()), subserv, SLOT(random()));

    // Update UI after click to buttons
    connect(ui->btn_mostRare, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_mostOften, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_random, SIGNAL(clicked()), this, SLOT(update()));

    // Translate events
    connect(translater, SIGNAL(gotTranslate(QString)), this, SLOT(showTranslate(QString)));
    connect(ui->btn_translate, SIGNAL(clicked()), this, SLOT(showTranslate()));
    connect(ui->btn_sound, SIGNAL(clicked()), this, SLOT(playSound()));
}

void SubtitlesView::updateUi(WordInfo &info)
{
    word = info.word;

    // Add +1 for human read
    index = QString::number(info.index + 1) + "/" + QString::number(info.count + 1);

    ui->word->setText(word);
    ui->index->setText(index);
}
