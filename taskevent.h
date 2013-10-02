#ifndef TASKEVENT_H
#define TASKEVENT_H

#include <QWidget>
#include <QList>
#include "taskaccessmanager.h"

namespace Ui {
class Taskevent;
}

class Taskevent : public QWidget
{
    Q_OBJECT
    
public:
    explicit Taskevent(Taskaccessmanager *taskaccessmanager = 0, QWidget *parent = 0);
    ~Taskevent();
    int my_task_uid;
    bool task_set;
    
private:
    Ui::Taskevent *ui;
    Taskaccessmanager *taskaccess;
    QList<int> events;

public slots:
    void hide_me();
    void set_task(int task_uid);
    void send_event();
};

#endif // TASKEVENT_H
