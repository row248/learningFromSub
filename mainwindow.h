#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QSettings>

class SubtitlesView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toSubView();
    // From resent files
    void toSubView(QAction *act);
    void updateResetMenu();
    void updateStatusBar(QString str);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void init();
    void loadRecentFileActions();
    void addRecentFileAction(QString filename);
    void saveRecentFileNames();

private:
    Ui::MainWindow *ui;
    SubtitlesView *subview;
    QStringList recent_files;
    QList<QAction *> recentFileActions;
    QSettings *settings;
    bool is_subview;
};

#endif // MAINWINDOW_H
