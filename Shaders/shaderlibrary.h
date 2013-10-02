#ifndef SHADERLIBRARY_H
#define SHADERLIBRARY_H

//#include <QtOpenGL>
#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <Shaders/shader.h>
#include <QString>
#include <QList>
#include <QMap>

class Shaderlibrary
{
public:
    Shaderlibrary();
    ~Shaderlibrary();
    void set_shader_of_material(QString material, Shader* shader);
    Shader* get_shader_of_material(QString material);

private:
    void printLog(GLuint obj);
    QMap<QString,Shader*> shaders;
    // should add a standart shader and a standart material to render
    // in case of missing textures or stuff ...

};

#endif // SHADERLIBRARY_H
