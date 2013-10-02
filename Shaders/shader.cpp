#include "shader.h"
#include <QFile>
#include <QTextStream>
#include <QByteArray>

Shader::Shader(QString name, QString vertex_shader_path, QString fragment_shader_path)
{
    qDebug("#Shader construction:");
    shader_name = name;
    path_to_vertex_shader = vertex_shader_path;
    path_to_fragment_shader = fragment_shader_path;
}


Shader::~Shader(){

}

void Shader::set_stats(){
    //has to be reimplemented
}

void Shader::set_mat(){
    //has to be reimplemented
}

void Shader::set_data(){
    //has to be reimplemented
}

void Shader::draw(){
    //has to be reimplemented
}

void Shader::set_back(){
    //has to be reimplemented
}

void Shader::set(GLwindow* win){
    window = win;
}

void Shader::set(Camera* cam){
    camera = cam;
}

void Shader::set(Model* mdl){
    model = mdl;
}

void Shader::set(Mesh* msh){
    mesh = msh;
}

void Shader::set(Material* mtl){
    material = mtl;
}


QString Shader::get_shader_name(){
    return shader_name;
}

GLuint Shader::get_program(){
    return m_program;
}

bool Shader::check_status(){
    return created;
}

bool Shader::set_program(){
    GLint compiled;

    //vertex shader
    qDebug("    #Vertex shader:");
    m_standart_v_shader = glCreateShader(GL_VERTEX_SHADER);
    if(!m_standart_v_shader){
        qDebug("vertex shader error in creation...");
        return false;
    }

    /*
    QString vsh_qstr = load_shader_source(path_to_vertex_shader);
    GLint vsh_qstr_len = vsh_qstr.length();
    const char*  vsh = vsh_qstr.toUtf8().constData();
    //qDebug(vsh,vsh_qstr_len);
    //qDebug(QString(vsh).toUtf8());

    glShaderSource(m_standart_v_shader,1,&vsh,&vsh_qstr_len);
    */

    QFile vfile(path_to_vertex_shader);
    if (!vfile.open(QFile::ReadOnly)) {
        qWarning() << "Shader: Unable to open file" << path_to_vertex_shader;
        return false;
    }

    QByteArray vcontents = vfile.readAll();
    const char * vshader = vcontents.constData();
    glShaderSource(m_standart_v_shader,1,&vshader,NULL);

    glCompileShader(m_standart_v_shader);

    glGetShaderiv(m_standart_v_shader, GL_COMPILE_STATUS, &compiled);

    qDebug("    " + QString::number(compiled).toUtf8());
    if (compiled){
        qDebug("    vshader compiled");
    }
    else{
        qDebug("    vshader compile failed!!!");
        char messages[256];
        glGetShaderInfoLog(m_standart_v_shader, sizeof(messages), 0, &messages[0]);
        qDebug(messages);
        return false;
    }

    //fragment shader
    qDebug("    #Fragment shader:");
    m_standart_f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if(!m_standart_f_shader){
        qDebug("fragment shader error in creation...");
        return false;
    }
    /*
    QString fsh_qstr = load_shader_source(path_to_fragment_shader);
    GLint fsh_qstr_len = fsh_qstr.length();
    const char*  fsh = fsh_qstr.toUtf8().constData();

    //qDebug(fsh,fsh_qstr_len);
    //qDebug(QString(fsh).toUtf8());
    glShaderSource(m_standart_f_shader,1,&fsh,&fsh_qstr_len);
    */

    QFile ffile(path_to_fragment_shader);
    if (!ffile.open(QFile::ReadOnly)) {
        qWarning() << "Shader: Unable to open file" << path_to_fragment_shader;
        return false;
    }

    QByteArray fcontents = ffile.readAll();
    const char * fshader = fcontents.constData();
    glShaderSource(m_standart_f_shader,1,&fshader,NULL);

    glCompileShader(m_standart_f_shader);
    glGetShaderiv(m_standart_f_shader, GL_COMPILE_STATUS, &compiled);

    qDebug("    " + QString::number(compiled).toUtf8());
    if (compiled){
        qDebug("   fshader compiled");
    }
    else{
        qDebug("    fshader compile failed!!!");
        char messages[256];
        glGetShaderInfoLog(m_standart_f_shader, sizeof(messages), 0, &messages[0]);
        qDebug(messages);
        return false;
    }

    m_program = glCreateProgram();
    if(!m_program){
        qDebug("m_programm error in creation...");
    }
    glAttachShader(m_program, m_standart_v_shader);
    glAttachShader(m_program, m_standart_f_shader);
    return true;
}

bool Shader::link_program(){
    GLint linked;
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    qDebug("    " + QString::number(linked).toUtf8());
    if (linked){
        qDebug("    program linked");
    }
    else{
        qDebug("    program linking failed!!!");
        char messages[256];
        glGetProgramInfoLog(m_program, sizeof(messages), 0, &messages[0]);
        qDebug(messages);
        return false;
    }
    glUseProgram(m_program);
    created = true;
    return true;
}

QString Shader::load_shader_source(QString path){

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("        #load_shader_source: error opening shader file !!!");//failed reading the file: wrong path ? file damaged ?
        return QString("no source file");
    }
    if(file.isOpen() && file.isReadable()){
        qDebug("        #load_shader_source: Shader source is open and readable...");
    }
    QString shadersource = file.readAll();
    file.close(); // when your done.
    return shadersource;

}
