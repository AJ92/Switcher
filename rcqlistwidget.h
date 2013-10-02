#ifndef RCQLISTWIDGET_H
#define RCQLISTWIDGET_H

#include <QListWidget>

class RCQListWidget : public QListWidget
{
Q_OBJECT
public:
    explicit RCQListWidget(QWidget *parent = 0);
    QAction* action_open;
    QAction* action_settings;
    QAction* action_close;
    QAction* action_kill;

private:
    bool eventFilter(QObject *, QEvent *);

};

#endif // RCQLISTWIDGET_H
