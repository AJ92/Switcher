#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCheckBox>
#include "info.h"
#include "settingsstore.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(Settingsstore *store = 0, QWidget *parent = 0);
    ~Settings();
    
public slots:
    void open();
    void back();
    void info();

    //systeminfo
    void graph_update_time_change(int value);
    void advanced_info_change(bool value);
    void cpu_usage_change(bool value);
    void ram_usage_change(bool value);

    //log
    void log_task_starts_change(bool value);
    void log_task_stops_change(bool value);
    void save_log_change(bool value);


private:

    void fix_checkbox_lext(QCheckBox* checkbox);

    Info *info_win;
    Ui::Settings *ui;
    Settingsstore* storage;
};

#endif // SETTINGS_H
