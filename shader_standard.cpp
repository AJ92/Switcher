#include "shader_standard.h"

Shader_standard::Shader_standard(QString name, QString vertex_shader_path, QString fragment_shader_path):
    Shader::Shader(name, vertex_shader_path, fragment_shader_path)
{
    //returns true for successful shader compile and program creation + attachment of shaders
    set_program();


    // Bind the custom vertex attributes
    glBindAttribLocation(m_program, 0, "vertex");
    glBindAttribLocation(m_program, 1, "texCoord");
    glBindAttribLocation(m_program, 2, "normal");

    //returns true for successful linking
    link_program();

    glUniform1i(glGetUniformLocation(m_program, "sampler1"), 0);
}


void Shader_standard::set_stats(){

    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);

    glUseProgram(m_program);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    mvp_mat_loc = glGetUniformLocation(m_program, "mvp_matrix");
    norm_mat_loc = glGetUniformLocation(m_program, "norm_matrix");
    samp2d_loc = glGetUniformLocation(m_program, "sampler1");


}

void Shader_standard::set_mat(){
    glBindTexture(GL_TEXTURE_2D, material->get_mtl_diffuse_tex_map());
}

void Shader_standard::set_data(){

    glUniform1i(samp2d_loc, 0);

    //vertex arrays
    /*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, meshs_to_render.at(j)->get_vertices());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, meshs_to_render.at(j)->get_texcoords());
    */

    //VBOs
    glBindBuffer(GL_ARRAY_BUFFER, mesh->get_vertex_vbo());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->get_texcoord_vbo());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->get_normal_vbo());
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
}


void Shader_standard::draw(){

    m_p.setToIdentity();
    m_p.perspective(camera->FOV,window->get_window_aspect(),camera->Z_NEAR,camera->Z_FAR);

    //matrix model_view_projection
    m_mvp = camera->M_camera_view.inverted() * model->m_modelview;
    m_norm = m_mvp.inverted();
    m_mvp = m_p * m_mvp;
    //memset(mvp_mat, 0, sizeof(GLfloat) * 16);
    for (int f = 0; f < 4; f++) {
        for (int g = 0; g < 4; g++) {
            mvp_mat[f * 4 + g] = (GLfloat) (m_mvp.constData()[f * 4 + g]);
            norm_mat[f * 4 + g] = (GLfloat) (m_norm.constData()[f * 4 + g]);
        }
    }


    //set the mvp_matrix in the shader programm:
    glUniformMatrix4fv(mvp_mat_loc, 1, GL_FALSE, mvp_mat);

    glUniformMatrix4fv(norm_mat_loc, 1, GL_FALSE, norm_mat);


    // Render the triangles
    glDrawArrays(GL_TRIANGLES, 0, mesh->get_triangle_count()*3);
}

void Shader_standard::set_back(){
     //set back stats like culling / depth masks and so on ...
}
