#include "headers/subtitlesview.h"
#include "ui_subtitlesview.h"
#include "headers/subservice.h"
#include "headers/translater.h"
#include "headers/sqlprovider.h"

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
    initUi();
}

SubtitlesView::~SubtitlesView()
{
    delete ui;
}

void SubtitlesView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        subserv->prev();
        break;
    case Qt::Key_Right:
        subserv->next();
        break;
    case Qt::Key_T:
        translater->translate(word);
        break;
    case Qt::Key_S:
        translater->sound(word);
        break;
    case Qt::Key_Equal:
        favorite();
        break;
    }

    //
    update();
}

void SubtitlesView::setFileName(QString str)
{
    filename = str;
    subserv->setFileName(filename);
    ui->statusBar->setText(QFileInfo(filename).fileName());
    update();
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

void SubtitlesView::favorite()
{
    if (ui->icon->toolTip().startsWith("add", Qt::CaseInsensitive)) {
        db.addWord(word);
    } else {
        db.deleteWord(word);
    }

    // Change favorite button
    update();
}

void SubtitlesView::optionsChanged(int i)
{
    bool enableFavoriteIcon = true;

    switch (ui->options->itemData(i).toInt()) {
    case Options::MostRare:
        subserv->mostRare();
        break;
    case Options::MostOften:
        subserv->mostOften();
        break;
    case Options::Random:
        subserv->random();
        break;
    case Options::MatchDb:
        enableFavoriteIcon = subserv->matchDb(db);
        break;
    case Options::MatchDbRandom:
        enableFavoriteIcon = subserv->matchDbRandom(db);
        break;
    }

    // Change all stuff
    update();

    ui->icon->setEnabled(enableFavoriteIcon);
}

void SubtitlesView::init()
{
    subserv = new SubService();
    translater = new Translater();

    connect(ui->btnNext, SIGNAL(clicked()), subserv, SLOT(next()));
    connect(ui->btnPrev, SIGNAL(clicked()), subserv, SLOT(prev()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(update()));

    // Translate events
    connect(translater, SIGNAL(gotTranslate(QString)), this, SLOT(showTranslate(QString)));
    connect(ui->btn_translate, SIGNAL(clicked()), this, SLOT(showTranslate()));
    connect(ui->btn_sound, SIGNAL(clicked()), this, SLOT(playSound()));

    // Add delete word to db
    connect(ui->icon, SIGNAL(clicked()), this, SLOT(favorite()));
}

void SubtitlesView::initUi()
{
    QFile style("/home/row/Documents/projects/LearningFromSub/src/resources/favorite_icon.qss");
    if (style.open(QIODevice::ReadOnly)) {
        ui->icon->setStyleSheet( QString(style.readAll()) );
    }
    ui->icon->setFixedSize(40,40);

    ui->options->addItem("Most Rare", Options::MostRare);
    ui->options->addItem("Most Often", Options::MostOften);
    ui->options->addItem("Random", Options::Random);
    ui->options->addItem("Match db", Options::MatchDb);
    ui->options->addItem("Most db (random)", Options::MatchDbRandom);

    connect(ui->options, SIGNAL(activated(int)), this, SLOT(optionsChanged(int)));
}

void SubtitlesView::update()
{
    // Fill struct
    WordInfo wordInfo = subserv->current();
    word = wordInfo.word;

    db.updateWordStats(word);

    // Add +1 for human read
    index = QString::number(wordInfo.index + 1) + "/" + QString::number(wordInfo.count);

    ui->word->setText(word);
    ui->index->setText(index);

    // Check if user have word in db
    bool is_favorite = subserv->is_favorite(db);

    if (is_favorite) {
        ui->icon->setIcon(QIcon(":/Images/close.png"));
        ui->icon->setToolTip("Delete word from data base");
    } else if (word == NOTHING_FOUND) {
        ui->icon->setToolTip("Choose another option");
    } else {
        ui->icon->setIcon(QIcon(":/Images/add.gif"));
        ui->icon->setToolTip("Add word to data base");
    }
}
