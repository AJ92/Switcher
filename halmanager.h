#ifndef HALMANAGER_H
#define HALMANAGER_H

#include <hal.h>
#include <QString>

class Halmanager
{
public:
    Halmanager();

    int get_ram();
    int get_ramfree();
    int get_rampercent();

    int get_cpunum();
    QString get_cputype();
    QString get_cpuabitype();
    int get_cpuarch();
    int get_cpuinterruptbase();
    int get_cpuspeed();
};

#endif // HALMANAGER_H
