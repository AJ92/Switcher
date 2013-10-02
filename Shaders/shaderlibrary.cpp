#include "shaderlibrary.h"
#include "shader.h"

Shaderlibrary::Shaderlibrary()
{
    const GLubyte * version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    qDebug((const char *) glGetString(GL_VENDOR));
    qDebug((const char *) glGetString(GL_RENDERER));
    qDebug((const char *) glGetString(GL_VERSION));
    qDebug((const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
    qDebug((const char *) glGetString(GL_EXTENSIONS));
    if(version == 0){
        qDebug("GLSL version: 0");
        qDebug("  hardware does not support shaders ?");
    }
}

void Shaderlibrary::set_shader_of_material(QString material, Shader *shader){
    //if there is already a reference then it will be overwritten...
    shaders[material] = shader;
}

Shader* Shaderlibrary::get_shader_of_material(QString material){
    return shaders.value(material);
}
