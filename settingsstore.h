#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>


class Settingsstore: public QObject
{
    Q_OBJECT
public:
    Settingsstore(QObject *parent = 0);
    ~Settingsstore();

    void save();
    void read_settings();
    void read_file();

    /*
     *  GETTER
     */

    //SYSTEM INFO
    int get_int(QString option);
    bool get_bool(QString option);



    /*
     *  SETTER
     */
    //SYSTEM INFO
    void set_int(QString option, int number);
    void set_bool(QString option, bool on_off);

signals:
    void int_set(QString option, int number);
    void bool_set(QString option, bool on_off);

private:
    QString settings;
    QFile* file;
    QTextStream* stream;
};

#endif // SETTINGSSTORE_H
