#include "taskmanager.h"

Taskmanager::Taskmanager()
{
}


//methods to get tasks and change their stats or kill/close em ...
QList<int> Taskmanager::sym_get_task_uid_list(){
    QList<int> task_uids;

   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    CApaWindowGroupName* wgName= CApaWindowGroupName::NewL(wsSession);
    CleanupStack::PushL(wgName);

    CArrayFixFlat<TInt>* WindowGroupIds = new(ELeave)CArrayFixFlat<TInt>(1);
    CleanupStack::PushL(WindowGroupIds);

    User::LeaveIfError(wsSession.WindowGroupList(0,WindowGroupIds));
    const TInt count = WindowGroupIds->Count();



    TApaTaskList taskList( wsSession );

    for (TInt ii=0; ii<count; ii++)
    {
        TInt wgId = WindowGroupIds->At(ii);
        wgName->ConstructFromWgIdL(wgId);

        //wgName->Caption()
        //wgName->AppUid().Name()
        //wgName->Hidden()
        //wgName->IsSystem()
        //wgName->IsBusy()
        //wgName->AppUid()

        if(task_uids.indexOf(wgName->AppUid().iUid)<0){
            task_uids.append((int) wgName->AppUid().iUid);

            /*
            TApaTask task = taskList.FindApp(TUid::Uid(wgName->AppUid().iUid));
            if( task.Exists() )
            {
                RThread th;
                if (th.Open(task.ThreadId())==KErrNone)
                {
                    TName name = th.Name();
                    qDebug("[" + QString::number(ii).toUtf8() + "] "
                           + "[" + QString((QChar*) name.Ptr(),name.Length()).toUtf8() + "] "
                           + "[" + QString::number(wgName->AppUid().iUid).toUtf8() + "] ");
                }
                else{
                    qDebug(" !error during task.ThreadId()");
                }
            }
            else{
                qDebug(" !error task does not exsist");
            }
            */
        }
    }
    CleanupStack::PopAndDestroy(2);//WindowGroupIds, wgName
   #endif

    return task_uids;
}

QString Taskmanager::sym_get_task_name(int task_uid){
    QString task_name;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {


        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            TName name = th.Name();
            task_name = QString((QChar*) name.Ptr(),name.Length()).toUtf8();
            th.Close();
        }

        //qDebug(" - name task");
    }
   #endif
    return task_name;
}

bool Taskmanager::sym_get_task_is_system(int task_uid){
    bool is = false;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    CApaWindowGroupName* wgName= CApaWindowGroupName::NewL(wsSession);
    CleanupStack::PushL(wgName);

    CArrayFixFlat<TInt>* WindowGroupIds = new(ELeave)CArrayFixFlat<TInt>(1);
    CleanupStack::PushL(WindowGroupIds);

    User::LeaveIfError(wsSession.WindowGroupList(0,WindowGroupIds));
    const TInt count = WindowGroupIds->Count();

    for (TInt ii=0; ii<count; ii++)
    {
        TInt wgId = WindowGroupIds->At(ii);
        wgName->ConstructFromWgIdL(wgId);

        if(((int) wgName->AppUid().iUid) == task_uid)
        {
            if(wgName->IsSystem()){
                is = true;
            }
        }
    }
    CleanupStack::PopAndDestroy(2);//WindowGroupIds, wgName
   #endif
    return is;
}

bool Taskmanager::sym_get_task_is_hidden(int task_uid){
    bool is = false;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    CApaWindowGroupName* wgName= CApaWindowGroupName::NewL(wsSession);
    CleanupStack::PushL(wgName);

    CArrayFixFlat<TInt>* WindowGroupIds = new(ELeave)CArrayFixFlat<TInt>(1);
    CleanupStack::PushL(WindowGroupIds);

    User::LeaveIfError(wsSession.WindowGroupList(0,WindowGroupIds));
    const TInt count = WindowGroupIds->Count();

    for (TInt ii=0; ii<count; ii++)
    {
        TInt wgId = WindowGroupIds->At(ii);
        wgName->ConstructFromWgIdL(wgId);

        if(((int) wgName->AppUid().iUid) == task_uid)
        {
            if(wgName->Hidden()){
                is = true;
            }
        }
    }
    CleanupStack::PopAndDestroy(2);//WindowGroupIds, wgName
   #endif
    return is;
}

bool Taskmanager::sym_get_task_is_busy(int task_uid){
    bool is = false;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    CApaWindowGroupName* wgName= CApaWindowGroupName::NewL(wsSession);
    CleanupStack::PushL(wgName);

    CArrayFixFlat<TInt>* WindowGroupIds = new(ELeave)CArrayFixFlat<TInt>(1);
    CleanupStack::PushL(WindowGroupIds);

    User::LeaveIfError(wsSession.WindowGroupList(0,WindowGroupIds));
    const TInt count = WindowGroupIds->Count();

    for (TInt ii=0; ii<count; ii++)
    {
        TInt wgId = WindowGroupIds->At(ii);
        wgName->ConstructFromWgIdL(wgId);

        if(((int) wgName->AppUid().iUid) == task_uid)
        {
            if(wgName->IsBusy()){
                is = true;
            }
        }
    }
    CleanupStack::PopAndDestroy(2);//WindowGroupIds, wgName
   #endif
    return is;
}

void Taskmanager::sym_foreground_task(int task_uid){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {

        //task.EndTask();
        //task.KillTask();
        //task.SendSystemEvent(EApaSystemEventShutdown);
        task.BringToForeground();
        //task.SendToBackground();

        //RThread th;
        //if (th.Open(task.ThreadId())==KErrNone)
        //{
            //th.Kill(0);
            //th.Close();
        //}
        qDebug(" - foreground task");
    }
   #endif
}

void Taskmanager::sym_background_task(int task_uid){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {
        //task.EndTask();
        //task.KillTask();
        //task.SendSystemEvent(EApaSystemEventShutdown);
        //task.BringToForeground();
        task.SendToBackground();

        //RThread th;
        //if (th.Open(task.ThreadId())==KErrNone)
        //{
        //	th.Kill(0);
        //	th.Close();
        //}

        qDebug(" - background task");
    }
   #endif
}

void Taskmanager::sym_kill_task(int task_uid){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {
        //task.EndTask();
        task.KillTask();
        //task.SendSystemEvent(EApaSystemEventShutdown);
        //task.BringToForeground();
        //task.SendToBackground();

        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Kill(0);
            th.Close();
        }

        qDebug(" - Killed task");
    }
   #endif
}

void Taskmanager::sym_close_task(int task_uid){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {
        task.EndTask();
        //task.KillTask();
        //task.SendSystemEvent(EApaSystemEventShutdown);
        //task.BringToForeground();
        //task.SendToBackground();



        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
        //	th.Kill(0);
            th.Close();
        }

        qDebug(" - Stopped task");
    }
   #endif
}


int Taskmanager::sym_get_cputime(int task_uid){
    int cpu_time = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TTimeIntervalMicroSeconds aCpuTime;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.GetCpuTime(aCpuTime);
            cpu_time = (int) aCpuTime.Int64();
            th.Close();
        }


        //qDebug(" - CPU time");
    }
   #endif
    return cpu_time;
}


int Taskmanager::sym_get_process_handles(int task_uid){
    int handles = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TInt processhandles;
    TInt threadhandles;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.HandleCount(processhandles,threadhandles);
            handles = (int) processhandles;
            th.Close();
        }

        qDebug(" - process handle count");
    }
   #endif
    return handles;
}

int Taskmanager::sym_get_thread_handles(int task_uid){
    int handles = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TInt processhandles;
    TInt threadhandles;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.HandleCount(processhandles,threadhandles);
            handles = (int) threadhandles;
            th.Close();
        }

        qDebug(" - thread handle count");
    }
   #endif
    return handles;
}





//      STACK infos


int Taskmanager::sym_get_thread_stack_expandlimit(int task_uid){
    int expandlimit = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TThreadStackInfo stackinfo;


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.StackInfo(stackinfo);
            TLinAddr iexpandlimit = stackinfo.iExpandLimit;
            expandlimit = (int) iexpandlimit;
            th.Close();
        }

        qDebug(" - thread stack iexpandlimit");
    }
   #endif
    return expandlimit;
}


int Taskmanager::sym_get_thread_stack_base(int task_uid){
    int base = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TThreadStackInfo stackinfo;


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.StackInfo(stackinfo);
            TLinAddr ibase = stackinfo.iBase;
            base = (int) ibase;
            th.Close();
        }

        qDebug(" - thread stack ibase");
    }
   #endif
    return base;
}


int Taskmanager::sym_get_thread_stack_limit(int task_uid){
    int limit = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    TThreadStackInfo stackinfo;


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.StackInfo(stackinfo);
            TLinAddr ilimit = stackinfo.iLimit;
            limit = (int) ilimit;
            th.Close();
        }

        qDebug(" - thread stack ilimit");
    }
   #endif
    return limit;
}


//    Process meminfo


int Taskmanager::sym_get_process_meminfo_codebase(int task_uid){
    int codebase = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            codebase = (int) meminfo.iCodeBase;
            th.Close();
        }

        qDebug(" - process meminfo icodebase");
    }
   #endif
    return codebase;
}

int Taskmanager::sym_get_process_meminfo_codesize(int task_uid){
    int codesize = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            codesize = (int) meminfo.iCodeSize;
            th.Close();
        }

        qDebug(" - process meminfo icodesize");
    }
   #endif
    return codesize;
}


int Taskmanager::sym_get_process_meminfo_constdatabase(int task_uid){
    int constdatabase = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            constdatabase = (int) meminfo.iConstDataBase;
            th.Close();
        }

        qDebug(" - process meminfo iconstdatabase");
    }
   #endif
    return constdatabase;
}


int Taskmanager::sym_get_process_meminfo_constdatasize(int task_uid){
    int constdatasize = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            constdatasize = (int) meminfo.iConstDataSize;
            th.Close();
        }

        qDebug(" - process meminfo iconstdatasize");
    }
   #endif
    return constdatasize;
}

int Taskmanager::sym_get_process_meminfo_initializeddatabase(int task_uid){
    int initializeddatabase = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            initializeddatabase = (int) meminfo.iInitialisedDataBase;
            th.Close();
        }

        qDebug(" - process meminfo iinitializeddatabase");
    }
   #endif
    return initializeddatabase;
}

int Taskmanager::sym_get_process_meminfo_initializeddatasize(int task_uid){
    int initializeddatasize = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            initializeddatasize = (int) meminfo.iInitialisedDataSize;
            th.Close();
        }

        qDebug(" - process meminfo iinitializeddatasize");
    }
   #endif
    return initializeddatasize;
}

int Taskmanager::sym_get_process_meminfo_uninitializeddatabase(int task_uid){
    int uninitializeddatabase = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            uninitializeddatabase = (int) meminfo.iUninitialisedDataBase;
            th.Close();
        }

        qDebug(" - process meminfo iuninitializeddatasize");
    }
   #endif
    return uninitializeddatabase;
}

int Taskmanager::sym_get_process_meminfo_uninitializeddatasize(int task_uid){
    int uninitializeddatasize = 0;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    RProcess aprocess;
    TModuleMemoryInfo meminfo;

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.Process(aprocess);
            aprocess.GetMemoryInfo(meminfo);
            uninitializeddatasize = (int) meminfo.iUninitialisedDataSize;
            th.Close();
        }

        qDebug(" - process meminfo iuninitializeddatasize");
    }
   #endif
    return uninitializeddatasize;
}


int Taskmanager::sym_get_thread_priority(int task_uid){
    int priority = -1;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            priority = (int) th.Priority();
            th.Close();

        }

        qDebug(" - thread priority");
    }
   #endif
    return priority;
}

void Taskmanager::sym_set_thread_priority(int task_uid, int priority){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.SetPriority((TThreadPriority) priority);
            th.Close();
        }
        qDebug(" - thread priority set");
    }
   #endif
}

int Taskmanager::sym_get_process_priority(int task_uid){
    int priority = -100;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));


    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            priority = (int) th.ProcessPriority();
            th.Close();
        }
        qDebug(" - process priority");
    }
   #endif
    return priority;
}

void Taskmanager::sym_set_process_priority(int task_uid, int priority){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));



    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            th.SetProcessPriority((TProcessPriority) priority);
            th.Close();
        }
        qDebug(" - process priority set");
    }
   #endif
}

void Taskmanager::sym_send_system_event(int task_uid, int event){
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));


    if( task.Exists() )
    {
        task.SendSystemEvent((TApaSystemEvent) event);

        qDebug(" - system event sent");
    }
   #endif
}

QList<int> Taskmanager::sym_get_task_caps(int task_uid){
    QList<int> task_caps;
   #ifdef Q_OS_SYMBIAN
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp(TUid::Uid((TInt) task_uid));

    if( task.Exists() )
    {
        RThread th;
        if (th.Open(task.ThreadId())==KErrNone)
        {
            TBool has_cap;
            for(int i = 0; i < 20; i++){
            has_cap = th.HasCapability((TCapability)i);
            if(has_cap){
                task_caps.append(i);
            }

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

            -1    ECapability_None;
            -2    ECapability_Denied;
            */
            }
            th.Close();
        }

        //qDebug(" - name task");
    }
   #endif
    return task_caps;
}
