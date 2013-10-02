#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include <QString>
#include "settingsstore.h"

#include <QFile>
#include <QTextStream>

#include <QTime>
#include <QDate>

namespace Ui {
class Log;
}

class Log : public QWidget
{
    Q_OBJECT
    
public:
    explicit Log(Settingsstore *store = 0, QWidget *parent = 0);
    ~Log();
    
private:

    Ui::Log *ui;
    Settingsstore* storage;
    bool log_task_starts;
    bool log_task_stops;
    bool save_log_on_disc;

    QFile* file;
    QTextStream* stream;

public slots:
    void clear_log();
    void back();
    void add_log(QString text);
    void apply_bool(QString option, bool on_off);
};

#endif // LOG_H
