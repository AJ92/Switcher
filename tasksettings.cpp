#include "tasksettings.h"
#include "ui_tasksettings.h"

Tasksettings::Tasksettings(Settingsstore* store, Taskaccessmanager* taskaccessmanager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tasksettings)
{
    ui->setupUi(this);
    my_task_uid = -1;//default value to recognize if task uid has been set already...
    storage = store;
    taskaccess = taskaccessmanager;
    taskpriority = new Taskpriority();
    taskpriority->hide();
    taskevent = new Taskevent(taskaccess);
    taskevent->hide();
    QObject::connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(hide_me()));
    QObject::connect(ui->pushButtonRefresh, SIGNAL(clicked()), this, SLOT(refresh_info()));
    QObject::connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(close_task()));
    QObject::connect(ui->pushButtonKill, SIGNAL(clicked()), this, SLOT(kill_task()));
    QObject::connect(ui->pushButtonPriority, SIGNAL(clicked()), this, SLOT(set_task_priority()));
    QObject::connect(ui->pushButtonEvent, SIGNAL(clicked()), this, SLOT(send_system_event()));

    refresh_interval = 1000;
    timer_refresh = new QTimer();
    timer_refresh->setInterval(refresh_interval);
    QObject::connect(timer_refresh, SIGNAL(timeout()), this, SLOT(refresh_stats()));
}

void Tasksettings::set_task_priority(){
    taskpriority->set_task(my_task_uid);
    taskpriority->showFullScreen();
}

void Tasksettings::send_system_event(){
    taskevent->set_task(my_task_uid);
    taskevent->showFullScreen();
}

void Tasksettings::hide_me(){
    timer_refresh->stop();
    hide();
}

void Tasksettings::set_task(int task_uid){
    my_task_uid = task_uid;
    ui->progressBarTasktime->setValue(0);
    ui->progressBarCputime->setValue(0);
    timer_refresh->start();
    refresh_info();
}

void Tasksettings::refresh_stats(){
    get_cpu_usage();
}

void Tasksettings::refresh_info(){
    ui->textEditInfo->clear();
    ui->textEditInfo->setHtml(taskaccess->get_task_info(my_task_uid));
    QTextCursor cursor(ui->textEditInfo->textCursor());
    cursor.movePosition(QTextCursor::Start);
    ui->textEditInfo->setTextCursor(cursor);
}


void Tasksettings::get_cpu_usage(){
    ui->progressBarTasktime->setValue(taskaccess->get_task_time_of_task_precent(my_task_uid));
    ui->progressBarCputime->setValue(taskaccess->get_cpu_time_of_task_percent(my_task_uid));
}

void Tasksettings::close_task(){
    taskaccess->close_task(my_task_uid);
    timer_refresh->stop();
    hide();
}

void Tasksettings::kill_task(){
    taskaccess->kill_task(my_task_uid);
    timer_refresh->stop();
    hide();
}

Tasksettings::~Tasksettings()
{
    delete ui;
}
