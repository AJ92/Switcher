#ifndef TASKSETTINGS_H
#define TASKSETTINGS_H

#include <QWidget>
#include "taskaccessmanager.h"
#include <QTimer>
#include "taskpriority.h"
#include "taskevent.h"
#include "settingsstore.h"

namespace Ui {
class Tasksettings;
}

class Tasksettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tasksettings(Settingsstore* store, Taskaccessmanager *taskaccessmanager = 0, QWidget *parent = 0);
    ~Tasksettings();
    void set_task(int task_uid);
    
private:
    Ui::Tasksettings *ui;
    Settingsstore* storage;
    Taskaccessmanager *taskaccess;
    QTimer* timer_refresh;

    int my_task_uid;

    int refresh_interval;

    Taskpriority *taskpriority;
    Taskevent *taskevent;

public slots:
    void hide_me();
    void refresh_info();
    void refresh_stats();
    void get_cpu_usage();
    void close_task();
    void kill_task();
    void set_task_priority();
    void send_system_event();
};

#endif // TASKSETTINGS_H
