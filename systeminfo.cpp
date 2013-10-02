#include "systeminfo.h"
#include "ui_systeminfo.h"


Systeminfo::Systeminfo(Settingsstore* store, Taskaccessmanager* taskaccessmanager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Systeminfo)
{
    ui->setupUi(this);
    storage = store;
    taskaccess = taskaccessmanager;
    scene = new QGraphicsScene();
    ui->graphicsViewRamCpu->setScene(scene);
    ui->graphicsViewRamCpu->setRenderHints(QPainter::Antialiasing);
    ui->graphicsViewRamCpu->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsViewRamCpu->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    tasks_running = 0;
    last_started_task = "-";
    last_closed_task = "-";

    driveman = new Drivemanager();
    halman = new Halmanager();
    log = new Log(store);
    log->hide();
    QObject::connect(ui->pushButtonLog,SIGNAL(clicked()),this,SLOT(show_log()));

    QObject::connect(ui->pushButtonRefresh,SIGNAL(clicked()),this,SLOT(update_system_info()));

    draw_timer = new QTimer();
    draw_timer->setInterval(storage->get_int("graph_update_time")*1000);
    QObject::connect(draw_timer,SIGNAL(timeout()),this,SLOT(build_diagramm()));

    QObject::connect(ui->pushButtonBack,SIGNAL(clicked()),this,SLOT(back()));
    draw_timer->start();

    draw_cpu = storage->get_bool("cpu_graph");
    draw_ram = storage->get_bool("ram_graph");
    draw = false;

    QObject::connect(storage,SIGNAL(bool_set(QString,bool)),this,SLOT(apply_bool(QString,bool)));
    QObject::connect(storage,SIGNAL(int_set(QString,int)),this,SLOT(apply_int(QString,int)));
}

Systeminfo::~Systeminfo()
{
    delete ui;
}

void Systeminfo::set_task_runnin(int count){
    tasks_running = count;
    log->add_log("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] tasks running: " + QString::number(count).toUtf8());
}

void Systeminfo::set_last_started_task(QString name){
    last_started_task = name;
    log->add_log("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] task started: " + name.toUtf8());
}

void Systeminfo::set_last_closed_task(QString name){
    last_closed_task = name;
    log->add_log("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] task stopped: " + name.toUtf8());
}

void Systeminfo::show_log(){
    log->showFullScreen();
}

void Systeminfo::apply_int(QString option, int num){
    if(option.compare("graph_update_time") == 0){
        draw_timer->setInterval(num*1000);
    }
}

void Systeminfo::apply_bool(QString option, bool on_off){
    if(option.compare("cpu_graph") == 0){
        draw_cpu = on_off;
        if(!on_off){
            for(int i = 0; i < line_list.length(); i++){
                scene->removeItem(line_list.at(i));
            }
            line_list.clear();
            line_value_list.clear();
        }
    }
    if(option.compare("ram_graph") == 0){
        draw_ram = on_off;
        if(!on_off){
            for(int i = 0; i < line_list2.length(); i++){
                scene->removeItem(line_list2.at(i));
            }
            line_list2.clear();
            line_value_list2.clear();
        }
    }
}

void Systeminfo::set_on(){
    draw = true;
    draw_diagramm();
    update_system_info();
}

void Systeminfo::build_diagramm(){

    int cpu_usage = taskaccess->get_cpu_time_percent();
    if(cpu_usage > 100 || cpu_usage < 0){
        cpu_usage = 0;
    }

    int ram_usage = halman->get_rampercent();
    if(ram_usage > 100 || ram_usage < 0){
        ram_usage = 0;
    }

    if(draw_cpu){
        if(line_list.length() < 125){
            //CPU
            line_list.prepend(scene->addLine(0,0,5,0,QPen(QColor(255,0,0,255))));
            line_value_list.prepend(cpu_usage);
        }
    }

    if(draw_ram){
        if(line_list2.length() < 125){
            //RAM
            line_list2.prepend(scene->addLine(0,0,5,0,QPen(QColor(0,255,0,255))));
            line_value_list2.prepend(ram_usage);
        }
    }


    if(draw_cpu){
        if(line_list.length() >= 125){
            //CPU
            QGraphicsLineItem* line = line_list.last();
            line_list.removeLast();
            line->setLine(0,0,5,0);
            line_list.prepend(line);
            line_value_list.removeLast();
            line_value_list.prepend(cpu_usage);
        }
    }

    if(draw_ram){
        if(line_list.length() >= 125){
            //RAM
            QGraphicsLineItem* line2 = line_list2.last();
            line_list2.removeLast();
            line2->setLine(0,0,5,0);
            line_list2.prepend(line2);
            line_value_list2.removeLast();
            line_value_list2.prepend(ram_usage);
        }
    }


    if(draw){
        draw_diagramm();
    }
}

void Systeminfo::draw_diagramm(){

    QSize size = ui->graphicsViewRamCpu->size();
    int size_y = size.height();
    int size_x = size.width();
    //set pos and adjust y scale:
    for(int i = 0; i < line_list.length()-1; i++){
        //CPU
        line_list.at(i)->setLine(0,line_list.at(i+1)->line().y2(),5,-(int)((((float)line_value_list.at(i))/100.0f) * ((float)size_y)));
        line_list.at(i)->setPos(size_x-i*5,size_y);
    }
    for(int i = 0; i < line_list2.length()-1; i++){
        //RAM
        line_list2.at(i)->setLine(0,line_list2.at(i+1)->line().y2(),5,-(int)((((float)line_value_list2.at(i))/100.0f) * ((float)size_y)));
        line_list2.at(i)->setPos(size_x-i*5,size_y);
    }
    ui->graphicsViewRamCpu->viewport()->update();
}

void Systeminfo::back(){
    draw = false;
    hide();
}

//this func is ugly and long so better to keep this at the bottom of the script to prevent scrolling...
void Systeminfo::update_system_info(){

    QList<QString> drive_list = driveman->get_drives();

    QString info;
    //info.append();


    info.append("<p>");

        info.append("<b>Task info:</b>");

        info.append("<ul>");
            info.append("<li>");
                info.append("count: <font color='#00FF00'>");
                    info.append(QString::number(this->tasks_running).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("last started: <font color='#00FF00'>");
                    info.append(this->last_started_task.toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("last closed: <font color='#00FF00'>");
                    info.append(this->last_closed_task.toUtf8());
                info.append("</font>");
            info.append("</li>");


        info.append("</ul>");
        info.append("<br>");


        info.append("<b>Device info:</b>");
        info.append("<br>");
        info.append("<br>");



        info.append("<b>CPU:</b>");
        info.append("<ul>");
            info.append("<li>");
                info.append("count: <font color='#00FF00'>");
                    info.append(QString::number(halman->get_cpunum()).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("type: <font color='#00FF00'>");
                    info.append(halman->get_cputype().toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("ABI type: <font color='#00FF00'>");
                    info.append(halman->get_cpuabitype().toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("architecture: <font color='#00FF00'>");
                    info.append(QString::number(halman->get_cpuarch()).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("default interrupt base: <font color='#00FF00'>");
                    info.append(QString::number(halman->get_cpuinterruptbase()).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("speed (KHZ): <font color='#00FF00'>");
                    info.append(QString::number(halman->get_cpuspeed()).toUtf8());
                info.append("</font>");
            info.append("</li>");

        info.append("</ul>");
        info.append("<br>");


        info.append("<b>RAM:</b>");

        double size_free_mb = (double) (halman->get_ramfree())/1024.0/1024.0;
        double size_total_mb = (double) (halman->get_ram())/1024.0/1024.0;

        info.append("<ul>");

            info.append("<li>");
                info.append("Free (mb): <font color='#00FF00'>");
                    info.append(QString::number(size_free_mb).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("Free (%): <font color='#00FF00'>");
                    info.append(QString::number((int)((size_free_mb/size_total_mb)*100)).toUtf8());
                info.append("</font>");
            info.append("<li>");

            info.append("<li>");
                info.append("Used (mb)<font color='#FF7700'>");
                    info.append(QString::number(size_total_mb-size_free_mb).toUtf8());
                info.append("</font>");
            info.append("</li>");

            info.append("<li>");
                info.append("Used (%): <font color='#FF7700'>");
                    info.append(QString::number((int)(((size_total_mb-size_free_mb)/size_total_mb)*100)).toUtf8());
                info.append("</font>");
            info.append("<li>");

            info.append("<li>");
                info.append("Total (mb)<font color='#FF0000'>");
                    info.append(QString::number(size_total_mb).toUtf8());
                info.append("</font>");
            info.append("</li>");

        info.append("</ul>");
        info.append("<br>");


        info.append("<br>");
        info.append("<br>");


        info.append("<b>Filesystem:</b>");
        info.append("<br>");
        info.append("<br>");



                for(int i = 0; i < drive_list.length(); i++){

                    info.append(" " + drive_list.at(i) + ": ");

                    double volume_size_free_mb = (double) (driveman->get_volume_free_size_b(drive_list.at(i)))/1024.0/1024.0;
                    double volume_size_total_mb = (double) (driveman->get_volume_size_b(drive_list.at(i)))/1024.0/1024.0;

                    info.append("<ul>");

                        info.append("<li>");
                            info.append("Free (mb): <font color='#00FF00'>");
                                info.append(QString::number(volume_size_free_mb).toUtf8());
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Free (%): <font color='#00FF00'>");
                                info.append(QString::number((int)((volume_size_free_mb/volume_size_total_mb)*100)).toUtf8());
                            info.append("</font>");
                        info.append("<li>");

                        info.append("<li>");
                            info.append("Used (mb)<font color='#FF7700'>");
                                info.append(QString::number(volume_size_total_mb-volume_size_free_mb).toUtf8());
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Used (%): <font color='#FF7700'>");
                                info.append(QString::number((int)(((volume_size_total_mb-volume_size_free_mb)/volume_size_total_mb)*100)).toUtf8());
                            info.append("</font>");
                        info.append("<li>");

                        info.append("<li>");
                            info.append("Total (mb)<font color='#FF0000'>");
                                info.append(QString::number(volume_size_total_mb).toUtf8());
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Volume cache flags: <font color='#0077FF'>");
                                info.append(driveman->map_volume_cache_flag_to_string(driveman->get_volume_cache_flags(drive_list.at(i))));
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Drive type: <font color='#0077FF'>");
                                info.append(driveman->map_type_to_string(driveman->get_drive_type(drive_list.at(i))));
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Drive attribute: <font color='#0077FF'>");
                                info.append(driveman->map_drive_attribute_to_string(driveman->get_drive_attributes(drive_list.at(i))));
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Drive connection type: <font color='#0077FF'>");
                                info.append(driveman->map_connection_type_to_string(driveman->get_drive_connection_type(drive_list.at(i))));
                            info.append("</font>");
                        info.append("</li>");

                        info.append("<li>");
                            info.append("Drive battery state: <font color='#0077FF'>");
                                info.append(driveman->map_battery_state_to_string(driveman->get_drive_battery_state(drive_list.at(i))));
                            info.append("</font>");
                        info.append("</li>");

                    info.append("</ul>");
                    info.append("<br>");
                }



        info.append("<b>More system info:</b>");
                info.append("<ul>");

                    info.append("<li>");
                    info.append("[Soon]");
                    info.append("</li>");

                info.append("</ul>");

            /*
        info.append("<b>General info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Name: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString(get_task_name_uid(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");
            */

    info.append("</p>");

    ui->textEdit->clear();
    ui->textEdit->setHtml(info);
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);
}
