#include "log.h"
#include "ui_log.h"

Log::Log(Settingsstore* store, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
    storage = store;

    log_task_starts = storage->get_bool("task_starts");
    log_task_stops = storage->get_bool("task_stops");
    save_log_on_disc = storage->get_bool("save_log");

    ui->textEdit_log->append(" - LOG start:");
    if(log_task_starts){
        ui->textEdit_log->append(" - logging task starts");
    }
    if(log_task_stops){
        ui->textEdit_log->append(" - logging task stops");
    }
    if(save_log_on_disc){
        ui->textEdit_log->append(" - storing log on disc");
        file = new QFile("C:/(" + QDate::currentDate().toString("dd.MM.yyyy") + ") (" + QTime::currentTime().toString("hh.mm.ss.zzz") + ").txt");
        if (!file->open (QIODevice::ReadWrite))
        {
            qDebug("Couldn't open log file for write...");
        }
        else{
            file->close();
        }
    }


    QObject::connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(back()));
    QObject::connect(ui->pushButton_clear,SIGNAL(clicked()),this,SLOT(clear_log()));


    QObject::connect(storage,SIGNAL(bool_set(QString,bool)),this,SLOT(apply_bool(QString,bool)));
}

Log::~Log()
{
    delete ui;
}


void Log::apply_bool(QString option, bool on_off){
    if(option.compare("task_starts") == 0){
        log_task_starts = on_off;
        if(log_task_starts){
            ui->textEdit_log->append(" - logging task starts enabled");
        }
        else{
            ui->textEdit_log->append(" - logging task starts disabled");
        }
    }
    if(option.compare("task_stops") == 0){
        log_task_stops = on_off;
        if(log_task_stops){
            ui->textEdit_log->append(" - logging task stops enabled");
        }
        else{
            ui->textEdit_log->append(" - logging task stops disabled");
        }
    }
    if(option.compare("save_log") == 0){
        save_log_on_disc = on_off;
        if(save_log_on_disc){
            ui->textEdit_log->append(" - storing log on disc enabled");
            file = new QFile("C:/(" + QDate::currentDate().toString("dd.MM.yyyy") + ") (" + QTime::currentTime().toString("hh.mm.ss.zzz") + ").txt");
            if (!file->open (QIODevice::ReadWrite))
            {
                qDebug("Couldn't open log file for write...");
            }
            else{
                file->close();
            }
        }
        else{
            ui->textEdit_log->append(" - storing log on disc disabled");
        }
    }
}

void Log::clear_log(){
    ui->textEdit_log->clear();
}

void Log::add_log(QString text){
    if(text.contains("task started") && !log_task_starts){
        return;
    }
    else if(text.contains("task stopped") && !log_task_stops){
        return;
    }
    ui->textEdit_log->append(text);
    if(save_log_on_disc){
        if (!file->open (QIODevice::ReadWrite))
        {
            qDebug("Couldn't open log file for write...");
        }
        else{
            file->seek(file->size());
            file->write(text.toUtf8()+"\n");
            file->close();
        }
    }

}

void Log::back(){
    this->hide();
}
