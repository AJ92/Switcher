#ifndef ENGINEREMOTEANALYZER_H
#define ENGINEREMOTEANALYZER_H
#include <QWidget>
#include <QString>
#include <QTimer>

#include "sender.h"
#include "glwindow.h"
#include "graphicsengine.h"

class Engineremoteanalyzer : public QWidget
{
    Q_OBJECT
public:
    Engineremoteanalyzer(GLwindow* glw,Graphicsengine* gen, QString host,int port);
    ~Engineremoteanalyzer();
    void send_error_msg(QString msg);

private:
    QTimer* timer;
    Sender* sender;
    GLwindow* glwindow;
    Graphicsengine* gengine;

private slots:
    void send_fps_var();
    void send_ts_var();
    void send_mspf_var();
    void send_vaspf_var();
    void send_tcpf_var();
    void send_vcpf_var();
    void send_rs_var();

};

#endif // ENGINEREMOTEANALYZER_H
