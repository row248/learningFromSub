#include "subtitlesview.h"
#include "ui_subtitlesview.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>

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

void SubtitlesView::setFileName(QString str)
{
    filename = str;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0, "error", file.errorString());

    QTextStream in(&file);

    emit setStatusBar(QFileInfo(filename).fileName());
}

void SubtitlesView::nextWord()
{
    qDebug() << "NEXT!";
}

void SubtitlesView::previousWord()
{
    qDebug() << "PREVIOUS!";
}

void SubtitlesView::init()
{
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(nextWord()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(previousWord()));
}
