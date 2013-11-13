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
        previousWord();
        break;
    case Qt::Key_Right:
        nextWord();
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

void SubtitlesView::matchDb()
{
    bool any = subserv->matchDb(db);

    if (any) {
        ui->icon->setEnabled(true);
    } else {
        ui->icon->setEnabled(false);
    }
}

void SubtitlesView::matchDbRandom()
{
    bool any = subserv->matchDbRandom(db);

    if (any) {
        ui->icon->setEnabled(true);
    } else {
        ui->icon->setEnabled(false);
    }
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

void SubtitlesView::update()
{
    WordInfo word = subserv->current();
    updateUi(word);
}

void SubtitlesView::enableIcon()
{
    if (!ui->icon->isEnabled())
        ui->icon->setEnabled(true);
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

    // Icon may be disabled if @matchDb will nothig found
    connect(ui->btn_mostRare, SIGNAL(clicked()), this, SLOT(enableIcon()));
    connect(ui->btn_mostOften, SIGNAL(clicked()), this, SLOT(enableIcon()));
    connect(ui->btn_random, SIGNAL(clicked()), this, SLOT(enableIcon()));

    // For purpose of giving db instance
    connect(ui->btn_matchDb, SIGNAL(clicked()), this, SLOT(matchDb()));
    connect(ui->btn_mathDbRandom, SIGNAL(clicked()), this, SLOT(matchDbRandom()));

    // Update UI after click to buttons
    connect(ui->btn_mostRare, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_mostOften, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_random, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_matchDb, SIGNAL(clicked()), this, SLOT(update()));
    connect(ui->btn_mathDbRandom, SIGNAL(clicked()), this, SLOT(update()));

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

    ui->comboBox->addItem("Most Rare");
    ui->comboBox->addItem("Most Often");
    ui->comboBox->addItem("Random");
    ui->comboBox->addItem("Match db");
    ui->comboBox->addItem("Most db (random)");
}

void SubtitlesView::updateUi(WordInfo &info)
{
    word = info.word;

    // Add +1 for human read
    index = QString::number(info.index + 1) + "/" + QString::number(info.count);

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
