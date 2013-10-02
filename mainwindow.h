#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QTimer>
#include <QList>
#include <QKeyEvent>
#include "taskmanager.h"
#include "tasksettings.h"
#include "systeminfo.h"
#include "settings.h"
#include "ckeycapturer.h"

#include "taskaccessmanager.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    //for managing tasks:
    QTimer *timer_refresh_list;


    //for tasks:
    Taskaccessmanager *taskaccessmanager;
    Tasksettings *tasksettings;
    Systeminfo *systeminfo;
    Settings *settings_win;
    int my_uid;
    QList<int> list_task_uid;
    QList<QString> list_task_name;

 public slots:
    void update_list(QList<QString> widget_string_list);
    void refresh_stats();
    void task_settings();
    void system_info();
    void settings();
    void foreground_task();
    void close_task();
    void kill_task();
    void exit_app();
};

#endif // MAINWINDOW_H
