#include "settings.h"
#include "ui_settings.h"
#include <QLineEdit>

Settings::Settings(Settingsstore* store, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    storage = store;

    QObject::connect(ui->pushButton_info, SIGNAL(clicked()), this, SLOT(info()));
    QObject::connect(ui->pushButton_back, SIGNAL(clicked()), this, SLOT(back()));

    QObject::connect(ui->spinBox_graph_update_time, SIGNAL(valueChanged(int)), this, SLOT(graph_update_time_change(int)));
    QObject::connect(ui->checkBox_advanced_info, SIGNAL(toggled(bool)), this, SLOT(advanced_info_change(bool)));
    QObject::connect(ui->checkBox_cpu_usage, SIGNAL(toggled(bool)), this, SLOT(cpu_usage_change(bool)));
    QObject::connect(ui->checkBox_ram_usage, SIGNAL(toggled(bool)), this, SLOT(ram_usage_change(bool)));

    QObject::connect(ui->checkBox_log_task_starts, SIGNAL(toggled(bool)), this, SLOT(log_task_starts_change(bool)));
    QObject::connect(ui->checkBox_log_task_stops, SIGNAL(toggled(bool)), this, SLOT(log_task_stops_change(bool)));
    QObject::connect(ui->checkBox_save_log, SIGNAL(toggled(bool)), this, SLOT(save_log_change(bool)));

    info_win = new Info();
    info_win->hide();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::info(){
    info_win->showFullScreen();
}

void Settings::open(){
    this->showFullScreen();
    qDebug("loading settings...");

    ui->spinBox_graph_update_time->setValue(storage->get_int("graph_update_time"));

    ui->checkBox_advanced_info->setChecked(storage->get_bool("advanced_info"));
    fix_checkbox_lext(ui->checkBox_advanced_info);

    ui->checkBox_cpu_usage->setChecked(storage->get_bool("cpu_graph"));
    fix_checkbox_lext(ui->checkBox_cpu_usage);

    ui->checkBox_ram_usage->setChecked(storage->get_bool("ram_graph"));
    fix_checkbox_lext(ui->checkBox_ram_usage);

    ui->checkBox_log_task_starts->setChecked(storage->get_bool("task_starts"));
    fix_checkbox_lext(ui->checkBox_log_task_starts);

    ui->checkBox_log_task_stops->setChecked(storage->get_bool("task_stops"));
    fix_checkbox_lext(ui->checkBox_log_task_stops);

    ui->checkBox_save_log->setChecked(storage->get_bool("save_log"));
    fix_checkbox_lext(ui->checkBox_save_log);

    qDebug("settings loaded");

    storage->read_file();
}

void Settings::back(){
    storage->save();
    this->hide();
}

void Settings::fix_checkbox_lext(QCheckBox* checkbox){
    if(checkbox){
        if(checkbox->isChecked()){
            checkbox->setText("on");
        }
        else{
            checkbox->setText("off");
        }
    }
}

//systeminfo
void Settings::graph_update_time_change(int value){
    storage->set_int("graph_update_time",value);
}

void Settings::advanced_info_change(bool value){
    storage->set_bool("advanced_info",value);
    fix_checkbox_lext(ui->checkBox_advanced_info);
}

void Settings::cpu_usage_change(bool value){
    storage->set_bool("cpu_graph",value);
    fix_checkbox_lext(ui->checkBox_cpu_usage);
}

void Settings::ram_usage_change(bool value){
    storage->set_bool("ram_graph",value);
    fix_checkbox_lext(ui->checkBox_ram_usage);
}

//log
void Settings::log_task_starts_change(bool value){
    storage->set_bool("task_starts",value);
    fix_checkbox_lext(ui->checkBox_log_task_starts);
}

void Settings::log_task_stops_change(bool value){
    storage->set_bool("task_stops",value);
    fix_checkbox_lext(ui->checkBox_log_task_stops);
}

void Settings::save_log_change(bool value){
    storage->set_bool("save_log",value);
    fix_checkbox_lext(ui->checkBox_save_log);
}
