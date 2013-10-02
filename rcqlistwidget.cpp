#include "rcqlistwidget.h"
#include <QMouseEvent>
#include <QMenu>

RCQListWidget::RCQListWidget(QWidget *parent) : QListWidget(parent)
{
    action_open = new QAction("open",this);
    action_settings = new QAction("settings",this);
    action_close = new QAction("close",this);
    action_kill = new QAction("kill",this);


    installEventFilter(this);
}

bool RCQListWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::ContextMenu)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);
        QMenu *menu = new QMenu(this);

        menu->addAction(action_open);
        menu->addAction(action_settings);
        menu->addAction(action_close);
        menu->addAction(action_kill);
        menu->exec(mouseEvent->globalPos());

        return false;
    }
    else
        return QListWidget::eventFilter(obj, event);
}
