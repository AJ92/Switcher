#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include "settingsstore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    taskaccessmanager = new Taskaccessmanager();
    Settingsstore* settings_store = new Settingsstore();

    tasksettings = new Tasksettings(settings_store,taskaccessmanager);
    tasksettings->hide();

    systeminfo = new Systeminfo(settings_store,taskaccessmanager);
    systeminfo->hide();

    settings_win = new Settings(settings_store);
    settings_win->hide();

    QObject::connect(taskaccessmanager, SIGNAL(task_list_updated(QList<QString>)), this, SLOT(update_list(QList<QString>)));

    QObject::connect(ui->listWidgetTasks->action_open, SIGNAL(triggered()), this, SLOT(foreground_task()));
    QObject::connect(ui->listWidgetTasks->action_settings, SIGNAL(triggered()), this, SLOT(task_settings()));
    QObject::connect(ui->listWidgetTasks->action_close, SIGNAL(triggered()), this, SLOT(close_task()));
    QObject::connect(ui->listWidgetTasks->action_kill, SIGNAL(triggered()), this, SLOT(kill_task()));

    QObject::connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(exit_app()));
    QObject::connect(ui->pushButtonTaskinfo, SIGNAL(clicked()), this, SLOT(system_info()));
    QObject::connect(ui->pushButtonSettings, SIGNAL(clicked()), this, SLOT(settings()));

    timer_refresh_list = new QTimer();
    timer_refresh_list->setInterval(1000);
    QObject::connect(timer_refresh_list, SIGNAL(timeout()), this, SLOT(refresh_stats()));
    timer_refresh_list->start();
}

//MY FUNCS:


void MainWindow::keyPressEvent(QKeyEvent *event){
   #ifdef Q_OS_SYMBIAN
    qDebug("key pressed: " + QString::number(event->nativeVirtualKey()).toUtf8());
   #endif
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
   #ifdef Q_OS_SYMBIAN
    qDebug("key released: " + QString::number(event->nativeVirtualKey()).toUtf8());
   #endif
}

void MainWindow::exit_app(){
    exit(0);
}

void MainWindow::task_settings(){
    if(ui->listWidgetTasks->currentIndex().isValid()){
        tasksettings->showFullScreen();
        tasksettings->set_task(taskaccessmanager->get_task_uid(ui->listWidgetTasks->currentIndex().row()));
    }
}

void MainWindow::system_info(){
    systeminfo->showFullScreen();
    systeminfo->set_on();
}

void MainWindow::settings(){
    settings_win->open();
}

void MainWindow::foreground_task(){
    if(ui->listWidgetTasks->currentIndex().isValid()){
        taskaccessmanager->foreground_task(taskaccessmanager->get_task_uid(ui->listWidgetTasks->currentIndex().row()));
    }
}

void MainWindow::close_task(){
    if(ui->listWidgetTasks->currentIndex().isValid()){
        taskaccessmanager->close_task(taskaccessmanager->get_task_uid(ui->listWidgetTasks->currentIndex().row()));
    }
}

void MainWindow::kill_task(){
    if(ui->listWidgetTasks->currentIndex().isValid()){
       taskaccessmanager->close_task(taskaccessmanager->get_task_uid(ui->listWidgetTasks->currentIndex().row()));
    }
}


void MainWindow::update_list(QList<QString> widget_string_list){

    ui->listWidgetTasks->clear();
    for(int i = 0; i < widget_string_list.length(); i++){
        ui->listWidgetTasks->insertItem(i,widget_string_list.at(i));
        //GET LAST STARTED TASK
        if(list_task_name.contains(widget_string_list.at(i)) == false){
            systeminfo->set_last_started_task(widget_string_list.at(i));
        }
    }
    refresh_stats();
    //GET LAST CLOSED TASK
    for(int i = 0; i < list_task_name.length(); i++){
        if(widget_string_list.contains(list_task_name.at(i)) == false){
            systeminfo->set_last_closed_task(list_task_name.at(i));
        }
    }
    if(list_task_name.length() != widget_string_list.length()){
        systeminfo->set_task_runnin(widget_string_list.length());
    }
    list_task_name = widget_string_list;
}

void MainWindow::refresh_stats(){
    for(int i = 0; i < ui->listWidgetTasks->count(); i++){
        int cpu_usage = taskaccessmanager->get_cpu_time_of_task_percent(taskaccessmanager->get_task_uid(i));
        if(cpu_usage > 100 || cpu_usage < 0){
            cpu_usage = 0;
        }
        ui->listWidgetTasks->item(i)->setText(QString("%1").arg(cpu_usage, 3, 10, QChar('0')).toUtf8() +
                                              "% - " + taskaccessmanager->get_task_name_index(i));
    }
}


//END MY FUNCS!

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}
