#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QObject>
#include <QtGui/QWidget>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <QTime>
#include <QResizeEvent>

#ifdef Q_OS_SYMBIAN
class CRepository;
#endif

class GLwindow : public QWidget
{
    Q_OBJECT
    
public:
    GLwindow(QWidget *parent = 0);
    ~GLwindow();

    void initEGL();
    bool initialized();

    float get_fps();
    float get_timestep();
    int get_renderstep();
    int get_window_width();
    int get_window_height();
    float get_window_aspect();

    void set_clear_color(float r, float g, float b, float d);

public slots:
    void swapbuffer();

protected:
    // EGL variables and functions
    EGLDisplay  eglDisplay;
    EGLConfig   eglConfig;
    EGLSurface  eglSurface;
    EGLContext  eglContext;


    float clear_color_r;
    float clear_color_g;
    float clear_color_b;
    float clear_color_d;

    bool testEGLError(const char* pszLocation);
    void cleanupAndExit(EGLDisplay eglDisplay);

    //window stuff
    virtual void resizeEvent(QResizeEvent *event);
    int window_w;
    int window_h;
    float window_aspect;
    //debug on(1) / off(0)
    int     gldebug;
    //framerate timestep vars and helper vars
    QTime   time;
    int     framecount;
    float   fps;
    float   timestep;
    //important value to prevent overhead and asyncroniouse cpu/gpu coordination and stack filling
    int     renderstep;
    //1 if glwindow is initialized 0 if not and -1 if failed ...
    int     init;
};

#endif // GLWINDOW_H
