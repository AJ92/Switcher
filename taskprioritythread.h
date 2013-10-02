#ifndef TASKPRIORITYTHREAD_H
#define TASKPRIORITYTHREAD_H

#include <QWidget>
#include <QList>
#include "taskmanager.h"

namespace Ui {
class Taskprioritythread;
}

class Taskprioritythread : public QWidget
{
    Q_OBJECT
    
public:
    explicit Taskprioritythread(QWidget *parent = 0);
    ~Taskprioritythread();
    void set_task(int task_uid);
    
private:
    Ui::Taskprioritythread *ui;
    Taskmanager *taskman;
    QList<int> priorities;
    int my_task_uid;
    bool task_set;

public slots:
    void set_thread_priority();
    void hide_me();
};

#endif // TASKPRIORITYTHREAD_H
