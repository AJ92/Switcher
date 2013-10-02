#ifndef TASKACCESSMANAGER_H
#define TASKACCESSMANAGER_H

#include <QObject>
#include "taskmanager.h"
#include <QString>
#include <QTimer>
#include <QList>

class Taskaccessmanager : public QObject
{
    Q_OBJECT
public:
    explicit Taskaccessmanager(QObject *parent = 0);
private:

    //for managing tasks:
    QTimer *timer_refresh_list;

    //for tasks:
    Taskmanager *taskman;
    int my_uid;
    QList<int> list_task_uid;
    QList<QString> list_task_name;

    //stores old cpu time
    QList<int> list_task_cpu_time;

    QList<int> list_cpu_time_of_task_percent;
    QList<int> list_task_time_of_task_precent;
    int cpu_time;
    int cpu_time_percent;

    //used for cpu time calculations !!!
    int refresh_interval;


    bool first_update;

    void calc_cpu_usage();

    int refreshs;

public:
    int get_task_uid(int list_index);
    QString get_task_name_index(int list_index);
    QString get_task_name_uid(int task_uid);
    int get_my_task_uid();
    void send_system_event(int task_uid, int event);

    int get_cpu_time_of_task_percent(int task_uid);
    int get_task_time_of_task_precent(int task_uid);
    int get_cpu_time();
    int get_cpu_time_percent();

    QString get_task_info(int task_uid);
    QString get_task_capabilities(int task_uid);

signals:
    void task_list_updated(QList<QString>);


public slots:
    void check_list();
    QList<QString> update_list();
    void foreground_task(int task_uid);
    void close_task(int task_uid);
    void kill_task(int task_uid);
};

#endif // TASKACCESSMANAGER_H
