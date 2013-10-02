#include "settingsstore.h"

Settingsstore::Settingsstore(QObject *parent)
{
    bool write_default_settings;
    if(QFile::exists("C:/settings.txt")){
        write_default_settings = false;
        qDebug("file exists.....");
    }
    else{
        write_default_settings = true;
        qDebug("file does not exist..");
    }

    //creating new empty file...
    file = new QFile("C:/settings.txt");

    if(write_default_settings == true){
        qDebug("writing default settings...");
        settings = QString("graph_update_time 2;\nadvanced_info on;\ncpu_graph on;\nram_graph on;\ntask_starts on;\ntask_stops on;\nsave_log on;\n");

    }
    else{
        if (!file->open (QIODevice::ReadWrite))
        {
            qDebug("Couldn't open settings file...");
        }
        qDebug("reading settings...");
        stream = new QTextStream( file );
        stream->seek(0);
        settings = stream->readAll();
        file->close();
    }
}


Settingsstore::~Settingsstore(){

}


void Settingsstore::save(){
    file->remove();
    file = new QFile("C:/settings.txt");
    if (!file->open (QIODevice::ReadWrite))
    {
        qDebug("Couldn't open settings file after removing");
    }

    file->write(settings.toUtf8());
    file->flush();
    file->close();
}


//GETTER

int Settingsstore::get_int(QString option){
    QString splitter;
    int index1 = settings.indexOf(option,0);
    int index2 = settings.indexOf(";",index1);
    splitter = settings.mid(index1+option.length()+1,index2-(index1+option.length())-1);

    qDebug(splitter.toUtf8());
    qDebug("        " + QString::number(splitter.toInt()).toUtf8());

    int ret_val = splitter.toInt();
    if(ret_val < 1){
        qDebug("READING FILE VALUE:");
        qDebug("possible error found...");
        qDebug("return falue smaller than 1... set to 1 to prevent errors in timer interval");
        return 1;
    }
    return ret_val;
}

bool Settingsstore::get_bool(QString option){
    QString splitter;
    int index1 = settings.indexOf(option,0);
    int index2 = settings.indexOf(";",index1);
    splitter = settings.mid(index1+option.length()+1,index2-(index1+option.length())-1);

    if(splitter.compare("on") == 0){
        qDebug("        true");
        return true;
    }
    qDebug("returning false");
    return false;
}




//DEBUG FILE READER...

void Settingsstore::read_settings(){
    qDebug(settings.toUtf8());
}

void Settingsstore::read_file(){
    QString line;
    file = new QFile("C:/settings.txt");
    if (!file->open (QIODevice::ReadWrite))
    {
        qDebug("Couldn't open settings file...");
    }
    stream = new QTextStream( file );
    stream->seek(0);
    while( !stream->atEnd() ) {
         line = stream->readLine();
         qDebug(line.toUtf8());
    }
    file->close();
}



//SETTER

void Settingsstore::set_int(QString option, int number){
    QString splitter;
    int index1 = settings.indexOf(option,0);
    int index2 = settings.indexOf(";",index1);
    splitter = settings.mid(index1,index2-index1+1);

    QStringList settings_list = settings.split(splitter);
    QString new_settings_file;
    new_settings_file.append(settings_list.first());
    new_settings_file.append(option.toUtf8() + " " + QString::number(number).toUtf8()+";");
    new_settings_file.append(settings_list.last());

    settings = new_settings_file;
    int_set(option,number);
}

void Settingsstore::set_bool(QString option, bool on_off){
    QString splitter;
    int index1 = settings.indexOf(option,0);
    int index2 = settings.indexOf(";",index1);
    splitter = settings.mid(index1,index2-index1+1);
    QStringList settings_list = settings.split(splitter);
    QString new_settings_file;
    new_settings_file.append(settings_list.first());
    if(on_off){
        new_settings_file.append(option.toUtf8() + " on;");
    }
    else{
        new_settings_file.append(option.toUtf8() + " off;");
    }
    new_settings_file.append(settings_list.last());
    qDebug(".");
    settings = new_settings_file;
    qDebug(".");
    bool_set(option,on_off);
    qDebug(".");
}


//SIGNALS
void int_set(QString option, int number){

}

void bool_set(QString option, bool on_off){

}
