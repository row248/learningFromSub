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
        updateStatusBar(filename);
    }
}

// NOTE: remove copy-paste
void MainWindow::toSubView(QAction *act)
{
    QString filename = act->data().toString();
    addRecentFileAction(filename);
    subview->setFileName(filename);
    this->setCentralWidget(subview);
    updateStatusBar(filename);
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
    QListIterator<QAction *> it(recentFileActions);
    // reverse
    it.toBack();
    // Maximum 5 items
    for (int i=0; it.hasPrevious() && i < 5; i++) {
        ui->menuResent->addAction(it.previous());
    }
}

void MainWindow::updateStatusBar(QString filename)
{
    ui->statusBar->showMessage(QFileInfo(filename).fileName());
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

// TODO: Delete not using filenames from settings
void MainWindow::loadRecentFileActions()
{
    int size = settings->beginReadArray("recent_files");
    QString data; // hold data from array indexes
    for (int i=0; i<size; ++i) {
        settings->setArrayIndex(i);
        data = settings->value("filename").toString();

        // Check if file exists
        QFile file(data);
        if (!file.exists())
            continue;

        // add QActions to list
        recentFileActions << new QAction(this);
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
