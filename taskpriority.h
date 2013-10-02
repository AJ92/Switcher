#ifndef TASKPRIORITY_H
#define TASKPRIORITY_H

#include <QWidget>
#include "taskpriorityprocess.h"
#include "taskprioritythread.h"

namespace Ui {
class Taskpriority;
}

class Taskpriority : public QWidget
{
    Q_OBJECT
    
public:
    explicit Taskpriority(QWidget *parent = 0);
    ~Taskpriority();
    void set_task(int task_uid);
    
private:
    Ui::Taskpriority *ui;
    int my_task_uid;
    bool task_set;
    Taskpriorityprocess *taskpriorityprocess;
    Taskprioritythread *taskprioritythread;

public slots:
    void hide_me();
    void set_process();
    void set_thread();

};

#endif // TASKPRIORITY_H
