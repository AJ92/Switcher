#ifndef MYAPP_H
#define MYAPP_H

#include <QObject>
#include "glwindow.h"
#include "graphicsengine.h"
#include "camera.h"
#include "resources.h"
#include "Model/model.h"

class Myapp: public QObject
{
    Q_OBJECT
public:
    explicit Myapp(QObject *parent = 0);

};

#endif // MYAPP_H
