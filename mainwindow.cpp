#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subtitlesview.h"

#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QListIterator>
#include <QLabel>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("learningFromSub");
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toSubView()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file with subtitles"), "/home", tr("*.srt"));
    if (!filename.isEmpty()) {
        addRecentFileAction(filename);
        subview->setFileName(filename);
        this->setCentralWidget(subview);
    }
}

// NOTE: remove copy-paste
void MainWindow::toSubView(QAction *act)
{
    QString filename = act->data().toString();
    addRecentFileAction(filename);
    subview->setFileName(filename);
    this->setCentralWidget(subview);
    is_subview = true;
}

void MainWindow::init()
{
    // Fill @recentFileActions container
    loadRecentFileActions();
    updateResetMenu();
    is_subview = false;

    subview = new SubtitlesView();

    connect(ui->actionOpen , SIGNAL(triggered()), this, SLOT(toSubView()));
    connect(ui->menuResent, SIGNAL(triggered(QAction*)), this, SLOT(toSubView(QAction*)));
    connect(subview, SIGNAL(setStatusBar(QString)), this, SLOT(updateStatusBar(QString)));
}

void MainWindow::updateResetMenu()
{
    QListIterator<QAction *> it(recentFileActions);
    // reverse
    it.toBack();
    // Maximum 5 items
    for (int i=0; it.hasPrevious() && i < 5; i++) {
        ui->menuResent->addAction(it.previous());
    }
}

void MainWindow::updateStatusBar(QString str)
{
    ui->statusBar->showMessage(str);
    QLabel *label = new QLabel("12/500");
    ui->statusBar->addWidget(label,2);
    qDebug() << str;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (is_subview) {
        switch (event->key()) {
        case Qt::Key_Left:
            subview->previousWord();
            break;
        case Qt::Key_Right:
            subview->nextWord();
            break;
        default:
            qDebug() << event->key();
        }
    }
}

void MainWindow::addRecentFileAction(QString filename)
{
    QAction *act = new QAction(this);
    act->setData(filename);
    act->setText( QFileInfo(filename).fileName() );
    recentFileActions << act;

    // Redraw menu && resave filenames
    updateResetMenu();
    saveRecentFileNames();
}

void MainWindow::loadRecentFileActions()
{
    int size = settings->beginReadArray("recent_files");
    QString data; // hold data from array indexes
    for (int i=0; i<size; ++i) {
        recentFileActions << new QAction(this);
        settings->setArrayIndex(i);
        data = settings->value("filename").toString();

        // add to QAction container
        recentFileActions[i]->setData(data);
        recentFileActions[i]->setText( QFileInfo(data).fileName() );
    }
    settings->endArray();
}

void MainWindow::saveRecentFileNames()
{
    settings->beginWriteArray("recent_files");
    for (int i=0; i<recentFileActions.count(); i++) {
        settings->setArrayIndex(i);
        settings->setValue( "filename", recentFileActions[i]->data().toString() );
    }
    settings->endArray();
}
