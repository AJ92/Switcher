#include "glwindow.h"


#ifdef Q_OS_SYMBIAN
#include <mw/coecntrl.h>
#include <centralrepository.h>
#endif

GLwindow::GLwindow(QWidget *parent)
    : QWidget(parent)
    , framecount(1)
    , fps(1.0)
    , window_w(1)
    , window_h(1)
    , timestep(1.0)
    , renderstep(10)
    , gldebug(0)
    , init(0)
{
    qDebug("\nGLwindow::initEGL()...");
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_StyledBackground, false);
    setAttribute(Qt::WA_PaintUnclipped);
    clear_color_r = 1.0f;
    clear_color_g = 1.0f;
    clear_color_b = 1.0f;
    clear_color_d = 0.0f;
}

GLwindow::~GLwindow()
{
    
}

float GLwindow::get_fps(){
    return fps;
}

float GLwindow::get_timestep(){
    return timestep;
}

int GLwindow::get_renderstep(){
    return renderstep;
}

int GLwindow::get_window_width(){
    return window_w;
}

int GLwindow::get_window_height(){
    return window_h;
}

float GLwindow::get_window_aspect(){
    return window_aspect;
}

bool GLwindow::initialized(){
    if(init == 1){
        return true;
    }else{
        return false;
    }
}

void GLwindow::set_clear_color(float r, float g, float b, float d){
    clear_color_r = r;
    clear_color_g = g;
    clear_color_b = b;
    clear_color_d = d;
}

void GLwindow::resizeEvent(QResizeEvent *event){

    if (event->size().width() == event->oldSize().width()
            && event->size().height() == event->oldSize().height()) {
        // The size has not changed.
        return;
    }

    int w = event->size().width();
    int h = event->size().height();
    if(init == 1){
        glViewport(0,0, w, h);
    }
    window_w = w;
    window_h = h;

    window_aspect = float(w)/float(h);

    // Call the base class' resizeEvent().
    QWidget::resizeEvent(event);
    qDebug("Screen init:");
    qDebug("   width : " + QString::number(window_w).toUtf8());
    qDebug("   height: " + QString::number(window_h).toUtf8());
    qDebug("   aspect: " + QString::number(window_aspect).toUtf8());
}


void GLwindow::initEGL(){
    setAttribute(Qt::WA_NoSystemBackground);

    // EGL variables
    eglDisplay	= 0;
    eglConfig	= 0;
    eglSurface	= 0;
    eglContext	= 0;

#ifdef Q_OS_WIN32
    HWND hwnd = this->winId();
    HDC dc = GetWindowDC(hwnd);
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)dc);
#endif

#ifdef Q_OS_SYMBIAN
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif

    EGLint majorVersion;
    EGLint minorVersion;

    if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion)) {
        qDebug("GLwindow: Error: initEGL() failed!!!");
        cleanupAndExit(eglDisplay);
    }

    qDebug("GLwindow: initEGL() finished");

    EGLint pi32ConfigAttribs[13];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;

    pi32ConfigAttribs[4] = EGL_DEPTH_SIZE;
    pi32ConfigAttribs[5] = 8;
    pi32ConfigAttribs[6] = EGL_NONE;

    EGLint pi32ContextAttribs[3];
    pi32ContextAttribs[0] = EGL_CONTEXT_CLIENT_VERSION;
    pi32ContextAttribs[1] = 2;
    pi32ContextAttribs[2] = EGL_NONE;

    EGLint configs;

    if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &configs)
            || (configs != 1))  {
        qDebug("GLwindow: Error: eglChooseConfig() failed!");
        cleanupAndExit(eglDisplay);
    }

    qDebug("GLwindow: eglChooseConfig() finished");

#ifdef Q_OS_SYMBIAN
    EGLNativeWindowType pwd = (void*)(this->winId()->DrawableWindow());
#else
    EGLNativeWindowType pwd = (EGLNativeWindowType)this->winId();
#endif

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, pwd, NULL);
    qDebug("GLwindow: eglCreateWindowSurface() finished");

    if (!testEGLError("eglCreateWindowSurface")) {
        cleanupAndExit(eglDisplay);
    }

    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL,
                                  pi32ContextAttribs);
    qDebug("GLwindow: eglCreateContext() finished");

    if (!testEGLError("eglCreateContext")) {
        cleanupAndExit(eglDisplay);
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    qDebug("GLwindow: eglMakeCurrent() finished");

    if (!testEGLError("eglMakeCurrent")) {
        cleanupAndExit(eglDisplay);
    }
    qDebug("GLwindow: EGL initialization successfull...");
    init = 1;
}

bool GLwindow::testEGLError(const char *pszLocation)
{
    Q_UNUSED(pszLocation); // To prevent compiler warnings.

    EGLint err = eglGetError();

    if (err != EGL_SUCCESS) {
        //qDebug("%s failed (%d)" <<  pszLocation << err);
        qDebug("testEGLError...");
        return false;
    }

    return true;
}

void GLwindow::cleanupAndExit(EGLDisplay eglDisplay){
    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(eglDisplay);
    exit(0);
}

void GLwindow::swapbuffer(){

    if(init != 1){
        return;
    }

    fps = framecount/(time.elapsed() / 1000.0);
    timestep = 60.0/fps;
    if (!(framecount % 100)){
        framecount = 0;
        time.start();
    }
    framecount ++;


    renderstep = (int)(1000.0f/fps);

    if (!eglSwapBuffers(eglDisplay, eglSurface)) {
        // eglSwapBuffers() failed!
        GLint errVal = eglGetError();

        /*
        EXPLANATION:

        http://library.forum.nokia.com/index.jsp?topic=/Nokia_Symbian3_Developers_Library/GUID-894AB487-C127-532D-852B-37CB0DEA1440.html

        On the Symbian platform, EGL handles the window resize in the next
        call to eglSwapBuffers(), which resizes the surface to match the new
        window size. If the preserve buffer option is in use, this function
        also copies across all the pixels from the old surface that overlap
        the new surface, although the exact details depend on the
        implementation.

        If the surface resize fails, eglSwapBuffers() returns EGL_FALSE and an
        EGL_BAD_ALLOC error is raised. This may mean that the implementation
        does not support the resizing of a surface or that there is not enough
        memory available (on a platform with GPU, this would be GPU rather
        than system memory).

        Applications must always monitor whether eglSwapBuffers() fails after
        a window resize. When it does fail, the application should do the
        following:
          - Call eglDestroySurface() to destroy the current EGL window surface.
          - Call eglCreateWindowSurface() to recreate the EGL window surface.

        This may cause a noticeable flicker and so should be done only when
        necessary.
        */

        qDebug("swapbuffer: eglSwapbuffers() failed with error");

        if (errVal == EGL_BAD_ALLOC || errVal == EGL_BAD_SURFACE) {
            if (errVal == EGL_BAD_ALLOC)
                qDebug("Error was bad alloc, taking care of it.");

            eglDestroySurface(eglDisplay, eglSurface);
#ifdef Q_OS_SYMBIAN
            EGLNativeWindowType pwd = (void*)(this->winId()->DrawableWindow());
#else
            EGLNativeWindowType pwd = (EGLNativeWindowType)this->winId();
#endif
            eglSurface = eglCreateWindowSurface(eglDisplay,eglConfig,pwd, NULL);
        }
        else {
            cleanupAndExit(eglDisplay);
        }
    }


    glClearColor( clear_color_r, clear_color_g, clear_color_b, clear_color_d );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

