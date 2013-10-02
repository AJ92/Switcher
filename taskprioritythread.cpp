#include "taskprioritythread.h"
#include "ui_taskprioritythread.h"

Taskprioritythread::Taskprioritythread(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Taskprioritythread)
{
    ui->setupUi(this);
    taskman = new Taskmanager();
    priorities.append(-30);
    priorities.append(-20);
    priorities.append(-10);
    priorities.append(0);
    priorities.append(10);
    priorities.append(20);
    priorities.append(30);
    priorities.append(100);
    priorities.append(150);
    priorities.append(200);
    priorities.append(350);
    priorities.append(400);
    priorities.append(450);
    priorities.append(500);
    priorities.append(810);
    priorities.append(820);
    priorities.append(830);
    priorities.append(840);
    priorities.append(850);
    priorities.append(860);
    priorities.append(870);
    priorities.append(880);

    task_set = false;

    QObject::connect(ui->pushButtonThread, SIGNAL(clicked()), this, SLOT(set_thread_priority()));
    QObject::connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(hide_me()));

}

void Taskprioritythread::hide_me(){
    task_set = false;
    hide();
}

void Taskprioritythread::set_task(int task_uid){
    my_task_uid = task_uid;
    task_set = true;
}

void Taskprioritythread::set_thread_priority(){
    if(ui->listWidget->currentIndex().isValid() && task_set){
        taskman->sym_set_thread_priority(my_task_uid, priorities.at(ui->listWidget->currentIndex().row()));
        hide();
        task_set = false;
    }
}

Taskprioritythread::~Taskprioritythread()
{
    delete ui;
}
