#include "taskevent.h"
#include "ui_taskevent.h"

Taskevent::Taskevent(Taskaccessmanager* taskaccessmanager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Taskevent)
{
    ui->setupUi(this);
    taskaccess = taskaccessmanager;
    events.append(1);
    events.append(2);
    events.append(3);
    events.append(4);
    events.append(100);

    task_set = false;

    QObject::connect(ui->pushButtonSend, SIGNAL(pressed()), this, SLOT(send_event()));
    QObject::connect(ui->pushButtonBack, SIGNAL(pressed()), this, SLOT(hide_me()));
}

void Taskevent::hide_me(){
    task_set = false;
    hide();
}

void Taskevent::set_task(int task_uid){
    my_task_uid = task_uid;
    task_set = true;
}

void Taskevent::send_event(){
    if(ui->listWidget->currentIndex().isValid() && task_set){

        //update code...
        taskaccess->send_system_event(my_task_uid, events.at(ui->listWidget->currentIndex().row()));
        hide();
        task_set = false;
    }
}

Taskevent::~Taskevent()
{
    delete ui;
}
