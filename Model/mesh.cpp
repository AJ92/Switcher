#include "mesh.h"

Mesh::Mesh(QString name,int triangle_count,GLfloat vertices[],GLfloat texcoords[],GLfloat normals[],
           GLuint vertex_vbo,GLuint texcoord_vbo, GLuint normal_vbo, Material *material)
{
    mesh_name = name;
    this->triangle_count = triangle_count;
    this->vertices  = vertices;
    this->texcoords = texcoords;
    this->normals   = normals;
    this->material  = material;
    this->vertex_vbo = vertex_vbo;
    this->texcoord_vbo = texcoord_vbo;
    this->normal_vbo = normal_vbo;

    /*
    qDebug("## new mesh ##");
    int face = 0;
    for(int k = 0; k < triangle_count*3; k+=3){
        qDebug(QString::number(face).toUtf8() + ":   " +
                 QString::number(this->vertices[k]).toUtf8() + " " +
                 QString::number(this->vertices[k+1]).toUtf8() + " " +
                 QString::number(this->vertices[k+2]).toUtf8());
        face += 1;
    }
    */
}

void Mesh::inc_count(){
    count+=1;
}

void Mesh::dec_count(){
    count-=1;
}

long Mesh::get_count(){
    return count;
}

int Mesh::get_triangle_count(){
    return triangle_count;
}

GLfloat* Mesh::get_vertices(){
    return vertices;
}

GLfloat* Mesh::get_texcoords(){
    return texcoords;
}

GLfloat* Mesh::get_normals(){
    return normals;
}

GLuint Mesh::get_vertex_vbo(){
    return vertex_vbo;
}

GLuint Mesh::get_texcoord_vbo(){
    return texcoord_vbo;
}

GLuint Mesh::get_normal_vbo(){
    return normal_vbo;
}

QString Mesh::get_mesh_name(){
    return mesh_name;
}

Material* Mesh::get_material(){
    return material;
}

void Mesh::set_material(Material* mtl){
    material = mtl;
}
