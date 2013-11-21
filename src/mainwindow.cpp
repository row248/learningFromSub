#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/subtitlesview.h"

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
    this->setFixedSize(size());
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
}

void MainWindow::init()
{
    // Fill @recentFileActions container
    loadRecentFileActions();
    updateResetMenu();

    subview = new SubtitlesView();

    connect(ui->actionOpen , SIGNAL(triggered()), this, SLOT(toSubView()));
    connect(ui->menuResent, SIGNAL(triggered(QAction*)), this, SLOT(toSubView(QAction*)));
}

void MainWindow::updateResetMenu()
{
    // Clear all old actions
    ui->menuResent->clear();
    for (int i=0; i < recentFileActions.count() && i < 5; ++i) {
        // Check if file exists
        if ( QFileInfo(recentFileActions.at(i)->data().toString()).exists() ) {
            ui->menuResent->addAction(recentFileActions.at(i));
        }
    }
}

void MainWindow::addRecentFileAction(QString filename)
{
    QAction *act = new QAction(this);
    act->setData(filename);
    act->setText( QFileInfo(filename).fileName() );
    recentFileActions.prepend(act);

    // Redraw menu && resave filenames
    updateResetMenu();
    saveRecentFileNames();
}

// TODO: Delete not using filenames from settings
void MainWindow::loadRecentFileActions()
{
    int size = settings->beginReadArray("recent_files");
    QString data; // hold data from array indexes

    // With limitation of count
    for (int i=0; i<size && i < 5; ++i) {
        settings->setArrayIndex(i);
        data = settings->value("filename").toString();

        // add QActions to list
        QAction *act = new QAction(this);
        act->setData(data);
        act->setText( QFileInfo(data).fileName() );

        recentFileActions << act;
    }
    settings->endArray();
}

void MainWindow::saveRecentFileNames()
{
    settings->beginWriteArray("recent_files");
    int count = recentFileActions.count();
    for (int i=0; i < count && i < 5; ++i) {
        settings->setArrayIndex(i);
        settings->setValue( "filename", recentFileActions.at(i)->data().toString() );
    }
    settings->endArray();
}
