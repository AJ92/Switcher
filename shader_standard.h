#ifndef SHADER_STANDARD_H
#define SHADER_STANDARD_H

#include "Shaders/shader.h"
#include <QMatrix4x4>

class Shader_standard : public Shader
{
public:
    explicit Shader_standard(QString name, QString vertex_shader_path, QString fragment_shader_path);


    void set_stats();
    void set_mat();
    void set_data();
    void draw();
    void set_back();

private:
    GLuint mvp_mat_loc;
    GLuint norm_mat_loc;
    GLuint samp2d_loc;

    QMatrix4x4 m_p;
    QMatrix4x4 m_mvp;
    GLfloat mvp_mat[16];
    QMatrix4x4 m_norm;
    GLfloat norm_mat[16];
};

#endif // SHADER_STANDARD_H
