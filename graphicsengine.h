#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "camera.h"
#include "glwindow.h"
#include "Shaders/shaderlibrary.h"
#include "Shaders/shader.h"
#include "resources.h"

class Graphicsengine
{
public:
    Graphicsengine();
    Graphicsengine(Shaderlibrary* shaderlibrary,Resources* resources);
    ~Graphicsengine();
    void set_window(GLwindow*   window);
    void set_camera(Camera*     camera);
    void set_resources(Resources*   resources);
    void set_shaderlibrary(Shaderlibrary*     shaderlibrary);
    void enable_solid();
    int get_mspf();
    int get_vaspf();
    int get_tcpf();
    int get_vcpf();

public slots:
    void render_frame();
private:
    GLwindow*       win;
    Camera*         cam;
    Resources*      res;
    Shaderlibrary*  sl;

    int mspf;   //material switches per frame, also texture bind count ...
    int vaspf;  //vertex array switches per frame, multiple switches per render cycle are counted once !!!
    int tcpf;
    int vcpf;

    QMatrix4x4 m_p;
    QMatrix4x4 m_mvp;
    GLfloat mvp_mat[16];
    QMatrix4x4 m_norm;
    GLfloat norm_mat[16];
};

#endif // GRAPHICSENGINE_H
