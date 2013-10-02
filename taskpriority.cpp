#include "taskpriority.h"
#include "ui_taskpriority.h"

Taskpriority::Taskpriority(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Taskpriority)
{
    ui->setupUi(this);
    taskpriorityprocess = new Taskpriorityprocess();
    taskprioritythread = new Taskprioritythread();
    QObject::connect(ui->pushButtonThread, SIGNAL(clicked()), this, SLOT(set_thread()));
    QObject::connect(ui->pushButtonProcess, SIGNAL(clicked()), this, SLOT(set_process()));
}

void Taskpriority::hide_me(){
    task_set = false;
    hide();
}

void Taskpriority::set_task(int task_uid){
    my_task_uid = task_uid;
    task_set = true;
}

void Taskpriority::set_thread(){
    taskprioritythread->showFullScreen();
    taskprioritythread->set_task(my_task_uid);
    hide_me();
}

void Taskpriority::set_process(){
    taskpriorityprocess->showFullScreen();
    taskpriorityprocess->set_task(my_task_uid);
    hide_me();
}

Taskpriority::~Taskpriority()
{
    delete ui;
}
