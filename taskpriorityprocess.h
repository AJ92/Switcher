#ifndef TASKPRIORITYPROCESS_H
#define TASKPRIORITYPROCESS_H

#include <QWidget>
#include <QList>
#include "taskmanager.h"

namespace Ui {
class Taskpriorityprocess;
}

class Taskpriorityprocess : public QWidget
{
    Q_OBJECT
    
public:
    explicit Taskpriorityprocess(QWidget *parent = 0);
    ~Taskpriorityprocess();
    void set_task(int task_uid);

private:
    Ui::Taskpriorityprocess *ui;
    Taskmanager *taskman;
    QList<int> priorities;
    int my_task_uid;
    bool task_set;

public slots:
    void set_process_priority();
    void hide_me();
};

#endif // TASKPRIORITYPROCESS_H
