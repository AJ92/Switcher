#include "taskaccessmanager.h"
#include <QTextEdit>

Taskaccessmanager::Taskaccessmanager(QObject *parent) :
    QObject(parent)

{
    taskman = new Taskmanager();
    my_uid = taskman->sym_get_task_uid_list().first();

    //first update should be called in the recieving object !!!

    first_update = false;

    /*
      *
      * 1000 default
      * 50 set for test
      *
      */

    refresh_interval = 1000;
    refreshs = 0;

    QTimer *timer_check_key = new QTimer();
    timer_check_key->setInterval(refresh_interval);
    QObject::connect(timer_check_key, SIGNAL(timeout()), this, SLOT(check_list()));
    timer_check_key->start();

}


void Taskaccessmanager::foreground_task(int task_uid){
    taskman->sym_foreground_task(task_uid);
    task_list_updated(update_list());
}

void Taskaccessmanager::close_task(int task_uid){
    taskman->sym_close_task(task_uid);
    task_list_updated(update_list());
}

void Taskaccessmanager::kill_task(int task_uid){
    taskman->sym_kill_task(task_uid);
    task_list_updated(update_list());
}

void Taskaccessmanager::send_system_event(int task_uid, int event){
    taskman->sym_send_system_event(task_uid, event);
}


QList<QString> Taskaccessmanager::update_list(){
    list_task_uid.clear();
    list_task_uid = taskman->sym_get_task_uid_list();
    list_task_name.clear();
    QList<QString> widget_string_list;
    for(int i = 0; i < list_task_uid.length(); i++){
        QString task_name = "";
        task_name = taskman->sym_get_task_name(list_task_uid.at(i));

        //CHECK ...
        /*
        if(task_name == ""){
            qDebug("failed to retrieve the task_name !!!!");
            exit(42);
        }
        */

        /*
        QString task_properties = "";
        QString task_uid = "[" + QString::number(list_task_uid.at(i)) + "]";
        if(taskman->sym_get_task_is_system(list_task_uid.at(i))){
            task_properties.append("[System]");
        }
        if(taskman->sym_get_task_is_busy(list_task_uid.at(i))){
            task_properties.append("+[Busy]");
        }
        if(taskman->sym_get_task_is_hidden(list_task_uid.at(i))){
            task_properties.append("+[Hidden]");
        }
        */
        list_task_name.append(task_name);
        widget_string_list.append(task_name);
    }
    qDebug("refresh: " + QString::number(refreshs).toUtf8() +
           "    count: " + QString::number(list_task_uid.length()).toUtf8());
    refreshs+=1;
    return widget_string_list;
}

QString Taskaccessmanager::get_task_info(int task_uid){

    qDebug(QString::number(task_uid).toUtf8());

    QString info;
    //info.append();
    info.append("<p>");



        info.append("<b>General info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Name: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString(get_task_name_uid(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("uUID: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(task_uid).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("System: ");
                    info.append("<font color='#00FF00'>");
                        if(taskman->sym_get_task_is_system(task_uid)){
                            info.append("yes");
                        }
                        else{
                            info.append("no");
                        }
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Hidden: ");
                    info.append("<font color='#00FF00'>");
                        if(taskman->sym_get_task_is_hidden(task_uid)){
                            info.append("yes");
                        }
                        else{
                            info.append("no");
                        }
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Busy: ");
                    info.append("<font color='#00FF00'>");
                        if(taskman->sym_get_task_is_busy(task_uid)){
                            info.append("yes");
                        }
                        else{
                            info.append("no");
                        }
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");



        info.append("<b>Advanced info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Capabilities: ");
                    info.append("<font color='#00FF00'>");
                        info.append(get_task_capabilities(task_uid).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Cpu time: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_cputime(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Thread priority: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_thread_priority(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Process priority: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_priority(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");



        info.append("<b>Handle info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Handles (threads): ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_thread_handles(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Handles (process): ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_handles(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");



        info.append("<b>Thread stack info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Stack base: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_thread_stack_base(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Stack expand limit: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_thread_stack_expandlimit(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Stack limit: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_thread_stack_limit(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");



        info.append("<b>Process memory info:</b>");
            info.append("<ul>");

                info.append("<li>");
                    info.append("Code base: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_codebase(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Code size: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_codesize(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Const data base: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_constdatabase(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("Const data size: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_constdatasize(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("initialized data base: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_initializeddatabase(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("initialized data size: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_initializeddatasize(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("uninitialized data base: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_uninitializeddatabase(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

                info.append("<li>");
                    info.append("uninitialized data size: ");
                    info.append("<font color='#00FF00'>");
                        info.append(QString::number(taskman->sym_get_process_meminfo_uninitializeddatasize(task_uid)).toUtf8());
                    info.append("</font>");
                info.append("</li>");

            info.append("</ul>");

    info.append("</p>");



    return info;
}

void Taskaccessmanager::check_list(){
    QList<int> temp_list_task_uid = taskman->sym_get_task_uid_list();

    if(temp_list_task_uid != list_task_uid){
        //task_list has changed, update list and call signal
        task_list_updated(update_list());
    }
    if(first_update == false){
        task_list_updated(update_list());
        first_update = true;
    }
    //task_list_updated(update_list());
    calc_cpu_usage();
}

int Taskaccessmanager::get_task_uid(int list_index){
    if((list_index > list_task_uid.length()-1) || (list_index < 0)){
        qDebug("-1");
        return -1;
    }
    return list_task_uid.at(list_index);
}

QString Taskaccessmanager::get_task_name_index(int list_index){
    if((list_index > list_task_name.length()-1) || (list_index < 0)){
        qDebug("-1");
        return "-1";
    }
    return list_task_name.at(list_index);
}

QString Taskaccessmanager::get_task_name_uid(int task_uid){
    int index = list_task_uid.indexOf(task_uid);
    if(index < 0){
        qDebug("-1");
        return "-1";
    }
    return list_task_name.at(index);
}

int Taskaccessmanager::get_my_task_uid(){
    return my_uid;
}


int Taskaccessmanager::get_cpu_time_of_task_percent(int task_uid){
    int index = list_task_uid.indexOf(task_uid);
    if(index < 0){
        qDebug("-1");
        return -1;
    }
    return list_cpu_time_of_task_percent.value(index);
}

int Taskaccessmanager::get_task_time_of_task_precent(int task_uid){
    int index = list_task_uid.indexOf(task_uid);
    if(index < 0){
        qDebug("-1");
        return -1;
    }
    return list_task_time_of_task_precent.at(index);
}

int Taskaccessmanager::get_cpu_time(){
    return cpu_time;
}

int Taskaccessmanager::get_cpu_time_percent(){
    return cpu_time_percent;
}

void Taskaccessmanager::calc_cpu_usage(){

    /*  data to update:
     *
     *   QList<int> list_cpu_time_of_task_percent;
     *   QList<int> list_task_time_of_task_precent;
     *   int cpu_time;
     *   int cpu_time_percent;
     */


    //update cpu times consumed by single tasks and store them into the list
    QList<int> temp_list_cpu_time_old = list_task_cpu_time;
    list_task_cpu_time.clear();
    list_cpu_time_of_task_percent.clear();
    list_task_time_of_task_precent.clear();
    QList<int> temp_list_tasks = taskman->sym_get_task_uid_list();
    int cpu_time_all = 0;

    for(int i = 0; i < temp_list_tasks.length(); i++){
        int temp_cpu_time = taskman->sym_get_cputime(temp_list_tasks.at(i));
        cpu_time_all += temp_cpu_time - temp_list_cpu_time_old.value(i);
        list_task_cpu_time.append(temp_cpu_time);
    }

    //update lists
    for(int i = 0; i < temp_list_tasks.length(); i++){
        int diff_cpu_time = 0;

        if(index >= 0){
            diff_cpu_time = list_task_cpu_time.value(i) - temp_list_cpu_time_old.value(i);
        }

        if(cpu_time_all <= 0){
            //qDebug("You have a magic device, task do not need any time or needs negative time to work !!!");
        }

        if(diff_cpu_time <= 0){
            //qDebug("Task was created last meassure step, stopped or does not consume any time...");
            diff_cpu_time = 1;
        }


        list_task_time_of_task_precent.append((int) ((((float) diff_cpu_time) / ((float) cpu_time_all))*100.0));
        list_cpu_time_of_task_percent.append((int) ((((float) diff_cpu_time) / ((float) (refresh_interval*1000.0)))*100.0));
    }

    //update cpu_time consumed by all tasks  (with or without hidden system tasks !!!)  :
    cpu_time = cpu_time_all;
    cpu_time_percent = (int) ((((float) cpu_time_all) / ((float) (refresh_interval*1000.0)))*100.0);
}

void task_list_updated(QList<QString>){
    //its a signal -.-
}

QString Taskaccessmanager::get_task_capabilities(int task_uid){
    QList<int> capslist = taskman->sym_get_task_caps(task_uid);
    QString caps;
    for(int i = 0; i < capslist.length(); i++){
        /*
        0    ECapabilityTCB;
        1    ECapabilityCommDD;
        2    ECapabilityPowerMgmt;
        3    ECapabilityMultimediaDD;
        4    ECapabilityReadDeviceData;
        5    ECapabilityWriteDeviceData;
        6    ECapabilityDRM;
        7    ECapabilityTrustedUI;
        8    ECapabilityProtServ;
        9    ECapabilityDiskAdmin;
        10   ECapabilityNetworkControl;
        11   ECapabilityAllFiles;
        12   ECapabilitySwEvent;
        13   ECapabilityNetworkServices;
        14   ECapabilityLocalServices;
        15   ECapabilityReadUserData;
        16   ECapabilityWriteUserData;
        17   ECapabilityLocation;
        18   ECapabilitySurroundingsDD;
        19   ECapabilityUserEnvironment;
        */
        if(capslist.at(i) == 0){
            caps.append("TCB ");
        }
        else if(capslist.at(i) == 1){
            caps.append("CommDD ");
        }
        else if(capslist.at(i) == 2){
            caps.append("PowerMgmt ");
        }
        else if(capslist.at(i) == 3){
            caps.append("MultimediaDD ");
        }
        else if(capslist.at(i) == 4){
            caps.append("ReadDeviceData ");
        }
        else if(capslist.at(i) == 5){
            caps.append("WriteDeviceData ");
        }
        else if(capslist.at(i) == 6){
            caps.append("DRM ");
        }
        else if(capslist.at(i) == 7){
            caps.append("TrustedUI ");
        }
        else if(capslist.at(i) == 8){
            caps.append("ProtServ ");
        }
        else if(capslist.at(i) == 9){
            caps.append("DiskAdmin ");
        }
        else if(capslist.at(i) == 10){
            caps.append("NetworkControl ");
        }
        else if(capslist.at(i) == 11){
            caps.append("AllFiles ");
        }
        else if(capslist.at(i) == 12){
            caps.append("SwEvent ");
        }
        else if(capslist.at(i) == 13){
            caps.append("NetworkServices ");
        }
        else if(capslist.at(i) == 14){
            caps.append("LocalServices ");
        }
        else if(capslist.at(i) == 15){
            caps.append("ReadUserData ");
        }
        else if(capslist.at(i) == 16){
            caps.append("WriteUserData ");
        }
        else if(capslist.at(i) == 17){
            caps.append("Location ");
        }
        else if(capslist.at(i) == 18){
            caps.append("SurroundingsDD ");
        }
        else if(capslist.at(i) == 19){
            caps.append("UserEnvironment ");
        }
    }
    return caps;
}
