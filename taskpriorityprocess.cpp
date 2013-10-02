#include "taskpriorityprocess.h"
#include "ui_taskpriorityprocess.h"

Taskpriorityprocess::Taskpriorityprocess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Taskpriorityprocess)
{
    ui->setupUi(this);
    taskman = new Taskmanager();
    priorities.append(150);
    priorities.append(250);
    priorities.append(350);
    priorities.append(450);
    priorities.append(650);
    priorities.append(750);
    priorities.append(850);
    priorities.append(950);

    task_set = false;

    QObject::connect(ui->pushButtonProcess, SIGNAL(clicked()), this, SLOT(set_process_priority()));
    QObject::connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(hide_me()));

}

void Taskpriorityprocess::hide_me(){
    task_set = false;
    hide();
}

void Taskpriorityprocess::set_task(int task_uid){
    my_task_uid = task_uid;
    task_set = true;
}

void Taskpriorityprocess::set_process_priority(){
    if(ui->listWidget->currentIndex().isValid() && task_set){
        taskman->sym_set_process_priority(my_task_uid, priorities.at(ui->listWidget->currentIndex().row()));
        hide();
        task_set = false;
    }
}

Taskpriorityprocess::~Taskpriorityprocess()
{
    delete ui;
}
