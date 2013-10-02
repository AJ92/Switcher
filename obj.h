#ifndef OBJ_H
#define OBJ_H

#include <QObject>

class Obj : public QObject
{
    Q_OBJECT
public:
    explicit Obj(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // OBJ_H
