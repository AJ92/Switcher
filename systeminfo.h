#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QWidget>
#include <QString>
#include "taskaccessmanager.h"
#include <QTime>
#include <QTimer>
#include <QList>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include "drivemanager.h"
#include "halmanager.h"
#include "settingsstore.h"
#include "log.h"

namespace Ui {
class Systeminfo;
}

class Systeminfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit Systeminfo(Settingsstore* store, Taskaccessmanager *taskaccessmanager = 0, QWidget *parent = 0);
    ~Systeminfo();
    void set_on();

public slots:
    void update_system_info();
    void draw_diagramm();
    void build_diagramm();
    void back();

    void show_log();

    void apply_int(QString option, int num);
    void apply_bool(QString option, bool on_off);

    void set_task_runnin(int count);
    void set_last_started_task(QString name);
    void set_last_closed_task(QString name);

    
private:
    Ui::Systeminfo *ui;
    Taskaccessmanager* taskaccess;
    Settingsstore* storage;
    Drivemanager* driveman;
    Halmanager* halman;
    Log* log;
    QTimer* draw_timer;
    int draw_interval;
    QGraphicsScene* scene;

    //CPU USAGE LIST
    QList<QGraphicsLineItem*> line_list;
    QList<int> line_value_list;

    //RAM USAGE LIST
    QList<QGraphicsLineItem*> line_list2;
    QList<int> line_value_list2;
    int line_pos;

    bool draw;

    bool draw_cpu;
    bool draw_ram;

    //some more infos
    int tasks_running;
    QString last_started_task;
    QString last_closed_task;

};

#endif // SYSTEMINFO_H
