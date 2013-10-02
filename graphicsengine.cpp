#include "graphicsengine.h"

Graphicsengine::Graphicsengine(Shaderlibrary* shaderlibrary,Resources* resources)
{
    qDebug("Graphicsengine starting...");
    sl = shaderlibrary;
    res = resources;
    cam = new Camera();
    mspf = 0;
    vaspf = 0;
    tcpf = 0;
    vcpf = 0;
}

Graphicsengine::Graphicsengine(){
    qDebug("Graphicsengine starting... Do not forget to set shaderlibrary and resources!");
    cam = new Camera();
}

Graphicsengine::~Graphicsengine()
{

}

void Graphicsengine::set_camera(Camera *camera){
    cam = camera;
}

void Graphicsengine::set_window(GLwindow *window){
    win = window;
}

void Graphicsengine::set_shaderlibrary(Shaderlibrary *shaderlibrary){
    sl = shaderlibrary;
}

void Graphicsengine::set_resources(Resources *resources){
    res = resources;
}

int Graphicsengine::get_mspf(){
    return mspf;
}

int Graphicsengine::get_vaspf(){
    return vaspf;
}

int Graphicsengine::get_tcpf(){
    return tcpf;
}

int Graphicsengine::get_vcpf(){
    return vcpf;
}

void Graphicsengine::render_frame(){
    win->swapbuffer();

    int t_mspf = 0;
    int t_vaspf = 0;
    int t_tcpf = 0;
    int t_vcpf = 0;

    QString last_mesh = "";

    QList<Material*> materials_to_set = res->get_material_instances();
    for(int i = 0; i < materials_to_set.length(); i++){

        Shader* current_shader = sl->get_shader_of_material(materials_to_set.at(i)->get_material_name());


        current_shader->set_stats();

        current_shader->set(win);
        current_shader->set(cam);
        current_shader->set(materials_to_set.at(i));
        current_shader->set_mat();

        t_mspf += 1;

        QList<Mesh*> meshs_to_render = res->get_mesh_instances().at(i);
        for(int j = 0; j < meshs_to_render.length(); j++){

            current_shader->set(res->get_model_instances().at(i).at(j));
            current_shader->set(meshs_to_render.at(j));



            //vertex buffer objects
            if(last_mesh != meshs_to_render.at(j)->get_mesh_name()){
                current_shader->set_data();
                t_vaspf += 1;
            }
            last_mesh = meshs_to_render.at(j)->get_mesh_name();



            // Render the triangles
            current_shader->draw();

            t_tcpf += meshs_to_render.at(j)->get_triangle_count();
            t_vcpf += meshs_to_render.at(j)->get_triangle_count()*3*3*3;
            //glFlush();

        }
        current_shader->set_back();

    }

    mspf = t_mspf;
    vaspf = t_vaspf;
    tcpf = t_tcpf;
    vcpf = t_vcpf;
}

/*
void Graphicsengine::render_frame(){
    win->swapbuffer();


    m_p.setToIdentity();
    m_p.perspective(cam->FOV,win->get_window_aspect(),cam->Z_NEAR,cam->Z_FAR);

    int t_mspf = 0;
    int t_vaspf = 0;
    int t_tcpf = 0;
    int t_vcpf = 0;

    QString last_mesh = "";

    QList<Material*> materials_to_set = res->get_material_instances();
    for(int i = 0; i < materials_to_set.length(); i++){

        GLuint current_programm = sl->get_shader_of_material(materials_to_set.at(i)->get_material_name())->get_program();
        glUseProgram(current_programm);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        GLuint mvp_mat_loc = glGetUniformLocation(current_programm, "mvp_matrix");
        GLuint norm_mat_loc = glGetUniformLocation(current_programm, "norm_matrix");
        GLuint samp2d_loc = glGetUniformLocation(current_programm, "sampler1");

        //shader / material specific gles settings:

        glBindTexture(GL_TEXTURE_2D, materials_to_set.at(i)->get_mtl_diffuse_tex_map());

        t_mspf += 1;

        QList<Mesh*> meshs_to_render = res->get_mesh_instances().at(i);
        for(int j = 0; j < meshs_to_render.length(); j++){


            Model* tempmdl = res->get_model_instances().at(i).at(j);

            //matrix model_view_projection
            m_mvp = cam->M_camera_view.inverted() * tempmdl->m_modelview;
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

            //vertex arrays

            //if(last_mesh != meshs_to_render.at(j)->get_mesh_name()){
            //    glUniform1i(samp2d_loc, 0);

            //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, meshs_to_render.at(j)->get_vertices());
            //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, meshs_to_render.at(j)->get_texcoords());

            //    t_vaspf += 1;
            //}


            //vertex buffer objects
            if(last_mesh != meshs_to_render.at(j)->get_mesh_name()){
                glUniform1i(samp2d_loc, 0);

                glBindBuffer(GL_ARRAY_BUFFER, meshs_to_render.at(j)->get_vertex_vbo());
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, meshs_to_render.at(j)->get_texcoord_vbo());
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, meshs_to_render.at(j)->get_normal_vbo());
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);



                t_vaspf += 1;
            }



            last_mesh = meshs_to_render.at(j)->get_mesh_name();

            // Render the triangles
            glDrawArrays(GL_TRIANGLES, 0, meshs_to_render.at(j)->get_triangle_count()*3);

            t_tcpf += meshs_to_render.at(j)->get_triangle_count();
            t_vcpf += meshs_to_render.at(j)->get_triangle_count()*3*3*3;
            //glFlush();
        }
    }
    mspf = t_mspf;
    vaspf = t_vaspf;
    tcpf = t_tcpf;
    vcpf = t_vcpf;
}
*/
