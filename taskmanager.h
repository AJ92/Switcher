#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QList>
#include <QChar>
#include <QString>

#ifdef Q_OS_SYMBIAN
    #include <apgwgnam.h>
    #include <eikenv.h>
    #include <apgtask.h> // Symbian header
    #include <AknTaskList.h>
#endif

class Taskmanager
{
public:
    Taskmanager();
    //for symbian only:
    QList<int>  sym_get_task_uid_list();

    bool        sym_get_task_is_system(int task_uid);
    bool        sym_get_task_is_hidden(int task_uid);
    bool        sym_get_task_is_busy(int task_uid);

    void        sym_kill_task(int task_uid);
    void        sym_close_task(int task_uid);
    QString     sym_get_task_name(int task_uid);

    void        sym_foreground_task(int task_uid);
    void        sym_background_task(int task_uid);

    int         sym_get_cputime(int task_uid);

    int         sym_get_process_handles(int task_uid);
    int         sym_get_thread_handles(int task_uid);

    int         sym_get_thread_stack_base(int task_uid);
    int         sym_get_thread_stack_expandlimit(int task_uid);
    int         sym_get_thread_stack_limit(int task_uid);

    int         sym_get_process_meminfo_codebase(int task_uid);
    int         sym_get_process_meminfo_codesize(int task_uid);
    int         sym_get_process_meminfo_constdatabase(int task_uid);
    int         sym_get_process_meminfo_constdatasize(int task_uid);
    int         sym_get_process_meminfo_initializeddatabase(int task_uid);
    int         sym_get_process_meminfo_initializeddatasize(int task_uid);
    int         sym_get_process_meminfo_uninitializeddatabase(int task_uid);
    int         sym_get_process_meminfo_uninitializeddatasize(int task_uid);

    int         sym_get_thread_priority(int task_uid);
    void        sym_set_thread_priority(int task_uid, int priority);

    int         sym_get_process_priority(int task_uid);
    void        sym_set_process_priority(int task_uid, int priority);

    void        sym_send_system_event(int task_uid, int event);

    QList<int>  sym_get_task_caps(int task_uid);
};

#endif // TASKMANAGER_H
