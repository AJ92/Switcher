#include "engineremoteanalyzer.h"

Engineremoteanalyzer::Engineremoteanalyzer(GLwindow* glw ,Graphicsengine* gen, QString host, int port)
{

    glwindow = glw;
    gengine = gen;
    sender = new Sender(host,port);


    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(send_fps_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_ts_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_mspf_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_vaspf_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_tcpf_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_vcpf_var()));
    connect(timer,SIGNAL(timeout()),this,SLOT(send_rs_var()));
    timer->start();


}

Engineremoteanalyzer::~Engineremoteanalyzer(){

}


void Engineremoteanalyzer::send_fps_var(){
    sender->send_msg("fps " + QString::number(glwindow->get_fps()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_ts_var(){
    sender->send_msg("ts " + QString::number(glwindow->get_timestep()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_mspf_var(){
    sender->send_msg("mspf " + QString::number(gengine->get_mspf()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_vaspf_var(){
    sender->send_msg("vaspf " + QString::number(gengine->get_vaspf()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_tcpf_var(){
    sender->send_msg("tcpf " + QString::number(gengine->get_tcpf()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_vcpf_var(){
    sender->send_msg("vcpf " + QString::number(gengine->get_vcpf()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_rs_var(){
    sender->send_msg("rs " + QString::number(glwindow->get_renderstep()).toUtf8() + ";");
}

void Engineremoteanalyzer::send_error_msg(QString msg){
    sender->send_msg("error " + msg + ";");
}

