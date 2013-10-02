#include "halmanager.h"
#include <hal_data.h>

Halmanager::Halmanager()
{
}


/*
  * RAM FUNCS...
 */
int Halmanager::get_ram(){
    TInt ram;
    HAL::Get(HAL::EMemoryRAM, ram);
    return (int) ram;
}

int Halmanager::get_ramfree(){
    TInt ramfree;
    HAL::Get(HAL::EMemoryRAMFree, ramfree);
    return (int) ramfree;
}

int Halmanager::get_rampercent(){
    int ram = get_ram();
    int ramfree = get_ramfree();
    return (int)(((float)ramfree/(float)ram)*100);
}


/*
  * CPU FUNCS...
 */
int Halmanager::get_cpunum(){
    TInt cpunum;
    HAL::Get(HAL::ENumCpus, cpunum);
    return (int) cpunum;
}

QString Halmanager::get_cputype(){
    TInt cputype;
    HAL::Get(HAL::ECPU, cputype);
    if(cputype == HAL::ECPU_ARM){
        return "ARM";
    }
    else if(cputype == HAL::ECPU_MCORE){
        return "MCORE";
    }
    else if(cputype == HAL::ECPU_X86){
        return "X86";
    }
    else{
        return "unknown";
    }
}

QString Halmanager::get_cpuabitype(){
    TInt cpuabitype;
    HAL::Get(HAL::ECPUABI, cpuabitype);
    if(cpuabitype == HAL::ECPUABI_ARM4){
        return "ARM4";
    }
    else if(cpuabitype == HAL::ECPUABI_ARM5T){
        return "AMR5T";
    }
    else if(cpuabitype == HAL::ECPUABI_ARMI){
        return "ARMI";
    }
    else if(cpuabitype == HAL::ECPUABI_MCORE){
        return "MCORE";
    }
    else if(cpuabitype == HAL::ECPUABI_MSVC){
        return "MSVC";
    }
    else if(cpuabitype == HAL::ECPUABI_THUMB){
        return "THUMB";
    }
    else if(cpuabitype == HAL::ECPUABI_X86){
        return "X86";
    }
    else{
        return "unknown";
    }
}

int Halmanager::get_cpuarch(){
    TInt cpuarch;
    HAL::Get(HAL::ECPUArch, cpuarch);
    return (int) cpuarch;
}

int Halmanager::get_cpuinterruptbase(){
    TInt cpuinterruptbase;
    HAL::Get(HAL::ECpuProfilingDefaultInterruptBase, cpuinterruptbase);
    return (int) cpuinterruptbase;
}

int Halmanager::get_cpuspeed(){
    TInt cpuspeed;
    HAL::Get(HAL::ECPUSpeed, cpuspeed);
    return (int) cpuspeed;
}
