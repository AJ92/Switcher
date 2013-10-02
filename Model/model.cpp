#include "model.h"

#define get_size(a) ( sizeof ( a ) / sizeof ( *a ) )

Model::Model(QString path, Resources* resources, bool &ok)
{
    debug = 1;
    resman = resources;
    count = 0;

    x = 0;
    y = 0;
    z = 0;

    x_scale = 1;
    y_scale = 1;
    z_scale = 1;


    visible = 1;
    lit = 0;
    light_color = QVector3D(0.0,-1.0,0.0);
    light_strength = 1.0;
    alpha_transparency = 0;
    black_transparency = 0;
    transparency = 1.0;
    sort = 1;

    QStringList pathlist = path.split(".",QString::KeepEmptyParts);
    if((pathlist.last() == "obj") || (pathlist.last() == "OBJ")){
        ok = load_file_obj(path);
    }
    else if((pathlist.last() == "ase") || (pathlist.last() == "ase")){
        ok = false;//load_file_ase(path);
    }
    ok = false;
    m_rotation.setToIdentity();
    m_translation.setToIdentity();
    m_scale.setToIdentity();
    build_model_view();
}


void Model::rotate(float angle, float x_axis, float y_axis, float z_axis){
    m_rotation.rotate(angle , x_axis, y_axis, z_axis);
    build_model_view();
}

void Model::rotate_reset(){
    m_rotation.setToIdentity();
    build_model_view();
}

void Model::translate(float x_pos, float y_pos, float z_pos){
    m_translation.setToIdentity();
    m_translation.translate(x_pos,y_pos,z_pos);
    x = x_pos;
    y = y_pos;
    z = z_pos;
    build_model_view();
}

void Model::scale(float x_sca, float y_sca, float z_sca){
    m_scale = QMatrix4x4();
    m_scale.scale(x_sca,y_sca,z_sca);
    x_scale = x_sca;
    y_scale = y_sca;
    z_scale = z_sca;
    build_model_view();
}

void Model::build_model_view(){
    m_modelview = m_translation * m_rotation;
    m_modelview = m_modelview * m_scale;
}

QVector3D Model::get_view_dir(){
    //vec forward 2 6 10
    return QVector3D(m_rotation.constData()[2],m_rotation.constData()[6],m_rotation.constData()[10]);
}

QVector3D Model::get_pos(){
    return QVector3D(x,y,z);
}

QVector3D Model::get_scale(){
    return QVector3D(x_scale,y_scale,z_scale);
}

void Model::inc_count(){
    count+=1;
}

void Model::dec_count(){
    count-=1;
}

long Model::get_count(){
    return count;
}

QString Model::get_model_name(){
    return model_name;
}

QList<Mesh*> Model::get_meshs(){
    return meshs;
}



bool Model::load_file_obj(QString path)
{
    qDebug(" \nModel loading...");

    QStringList pathlist = path.split("/",QString::KeepEmptyParts); //KeepEmptyParts
    model_name = pathlist.last();


    //check if model is already exists
    Model* temp = resman->get_model_loaded(model_name);

    //MODEL ALREADY LOADED SO JUST POINT TO THE RESOURCES OF EXISTING DATA :D
    if(temp != NULL){
        meshs = resman->get_model_meshs(temp);
        resman->add_model(this);
        return true;
    }

    //MODEL ISNT LOADED YET SO WE NEED TO LOAD THE DATA FROM FILES AND CREATE OBJECTS :(

    //LOAD MESH DATA
    QFile file(path);
    if (!file.open (QIODevice::ReadOnly))
    {
        qDebug("Model import: Error 1: Model file could not be loaded...");
        return false;
    }
    QTextStream stream ( &file );
    QString line;

    QString mtllib;

    QString current_mesh;
    QMap<QString,QVector<int> >mesh_faces;
    QMap<QString,QString> mesh_mtl;
    QVector<QVector3D> model_vertices;
    QVector<QVector3D> model_vertex_normals;
    QVector<QVector3D> model_vertex_texture_coordinates;


    while( !stream.atEnd() ) {
         line = stream.readLine();
         QStringList list = line.split(QRegExp("\\s+"),QString::SkipEmptyParts); //SkipEmptyParts

         if(!list.empty()){
             if(list.first() == "mtllib"){
                 mtllib = list.last();
             }

             else if(list.first() == "v"){
                 model_vertices.append(QVector3D(  list.value(1).toFloat(),
                                                   list.value(2).toFloat(),
                                                   list.value(3).toFloat()));
             }
             else if(list.first() == "vn"){
                 model_vertex_normals.append(QVector3D(  list.value(1).toFloat(),
                                                         list.value(2).toFloat(),
                                                         list.value(3).toFloat()));
             }
             else if(list.first() == "vt"){
                 model_vertex_texture_coordinates.append(QVector3D(  list.value(1).toFloat(),
                                                                     list.value(2).toFloat(),
                                                                     list.value(3).toFloat()));
             }
             else if(list.first() == "g"){
                 current_mesh = list.value(1);
             }
             else if(list.first() == "usemtl"){
                 mesh_mtl[current_mesh] = list.value(1);
             }
             else if(list.first() == "f"){
                 QStringList face_part_1_list = list.value(1).split("/",QString::SkipEmptyParts); //SkipEmptyParts
                 QStringList face_part_2_list = list.value(2).split("/",QString::SkipEmptyParts); //SkipEmptyParts
                 QStringList face_part_3_list = list.value(3).split("/",QString::SkipEmptyParts); //SkipEmptyParts
                 mesh_faces[current_mesh].append(face_part_1_list.value(0).toInt());
                 mesh_faces[current_mesh].append(face_part_1_list.value(1).toInt());
                 mesh_faces[current_mesh].append(face_part_1_list.value(2).toInt());

                 mesh_faces[current_mesh].append(face_part_2_list.value(0).toInt());
                 mesh_faces[current_mesh].append(face_part_2_list.value(1).toInt());
                 mesh_faces[current_mesh].append(face_part_2_list.value(2).toInt());

                 mesh_faces[current_mesh].append(face_part_3_list.value(0).toInt());
                 mesh_faces[current_mesh].append(face_part_3_list.value(1).toInt());
                 mesh_faces[current_mesh].append(face_part_3_list.value(2).toInt());

             }
          }

    }
    file.close();



    //LOAD MTL DATA

    pathlist.removeAt(pathlist.length()-1);
    QString mtl_path = pathlist.join("/") + "/" + mtllib;

    QFile mtlfile(mtl_path);
    if (!mtlfile.open (QIODevice::ReadOnly))
    {
        qDebug("Model import: Error 2: Model material file could not be loaded...");
        return false;
    }
    QTextStream mtlstream ( &mtlfile );
    QString mtlline;


    QString current_mtl;
    QMap<QString,QVector3D> mtl_ambient_c;          //Ka
    QMap<QString,QVector3D> mtl_diffuse_c;          //Kd
    QMap<QString,QVector3D> mtl_specular_c;         //Ks
    QMap<QString,float>     mtl_specular_ns;        //Ns
    QMap<QString,float>     mtl_transparency_d;     //d
    QMap<QString,float>     mtl_transparency_tr;    //Tr
    QMap<QString,QVector3D> mtl_transparency_tf;    //Tf
    QMap<QString,QString>   mtl_ambient_map;        //map_Ka
    QMap<QString,QString>   mtl_diffuse_map;        //map_Kd
    QMap<QString,QString>   mtl_specular_map;       //map_Ks
    QMap<QString,QString>   mtl_bump_map;           //map_bump
    QMap<QString,int>       mtl_illumination;       //illum

    while( !mtlstream.atEnd() ) {
         mtlline = mtlstream.readLine();
         QStringList list = mtlline.split(QRegExp("\\s+"),QString::SkipEmptyParts); //SkipEmptyParts
         if(!list.empty()){
             if(list.first() == "newmtl"){
                current_mtl = list.last();
             }
             else if(list.first() == "Ka"){
                 mtl_ambient_c[current_mtl] = QVector3D(list.value(1).toFloat(),
                                                        list.value(2).toFloat(),
                                                        list.value(3).toFloat());
             }
             else if(list.first() == "Kd"){
                 mtl_diffuse_c[current_mtl] = QVector3D(list.value(1).toFloat(),
                                                        list.value(2).toFloat(),
                                                        list.value(3).toFloat());
             }
             else if(list.first() == "Ks"){
                 mtl_specular_c[current_mtl] = QVector3D(list.value(1).toFloat(),
                                                        list.value(2).toFloat(),
                                                        list.value(3).toFloat());
             }
             else if(list.first() == "Ns"){
                 mtl_specular_ns[current_mtl] = list.value(1).toFloat();

             }
             else if(list.first() == "d"){
                 mtl_transparency_d[current_mtl] = list.value(1).toFloat();

             }
             else if(list.first() == "Tr"){
                 mtl_transparency_tr[current_mtl] = list.value(1).toFloat();

             }
             else if(list.first() == "Tf"){
                 mtl_transparency_tf[current_mtl] = QVector3D(list.value(1).toFloat(),
                                                        list.value(2).toFloat(),
                                                        list.value(3).toFloat());
             }
             else if(list.first() == "map_Ka"){
                 mtl_ambient_map[current_mtl] = list.value(1).split("\\",QString::SkipEmptyParts).last().toUtf8();
             }
             else if(list.first() == "map_Kd"){
                 mtl_diffuse_map[current_mtl] = list.value(1).split("\\",QString::SkipEmptyParts).last().toUtf8();
             }
             else if(list.first() == "map_Ks"){
                 mtl_specular_map[current_mtl] = list.value(1).split("\\",QString::SkipEmptyParts).last().toUtf8();
             }
             else if((list.first() == "map_bump") || (list.first() == "bump")){
                 mtl_bump_map[current_mtl] = list.value(1).split("\\",QString::SkipEmptyParts).last().toUtf8();
             }
             else if(list.first() == "illum"){
                 mtl_illumination[current_mtl] = list.value(1).toInt();
             }
         }

    }


    //CREATE MTLS (if needed...)

        //using diffues mat cause its the major map used (other maps are optional)...
    QList<QString> mtl_names = mtl_diffuse_c.keys();
    for(int i = 0; i < mtl_names.length(); i++){
        //checked if MTL already created...
        if(resman->check_material_loaded(mtl_names.value(i))){
            Material* mtl = resman->get_material_loaded(mtl_names.value(i));
            mtl->inc_count();
        }
        //create new MTL if not created... (pass all params...)
        else{
            Material* mtl = new Material(mtl_names.value(i));
            mtl->set_mtl_ambient_c(mtl_ambient_c[mtl_names.value(i)]);
            mtl->set_mtl_diffuse_c(mtl_diffuse_c[mtl_names.value(i)]);
            mtl->set_mtl_specular_c(mtl_specular_c[mtl_names.value(i)]);
            mtl->set_mtl_specular_ns(mtl_specular_ns[mtl_names.value(i)]);
            mtl->set_mtl_transparency_d(mtl_transparency_d[mtl_names.value(i)]);
            mtl->set_mtl_transparency_tr(mtl_transparency_tr[mtl_names.value(i)]);
            mtl->set_mtl_transparency_tf(mtl_transparency_tf[mtl_names.value(i)]);
            mtl->set_mtl_ambient_map(mtl_ambient_map[mtl_names.value(i)]);
            mtl->set_mtl_diffuse_map(mtl_diffuse_map[mtl_names.value(i)]);
            mtl->set_mtl_specular_map(mtl_specular_map[mtl_names.value(i)]);
            mtl->set_mtl_bump_map(mtl_bump_map[mtl_names.value(i)]);
            mtl->set_mtl_illumination(mtl_illumination[mtl_names.value(i)]);

            qDebug("MTL ambient m:   " + mtl->get_mtl_ambient_map().toUtf8());
            qDebug("MTL diffuse m:   " + mtl->get_mtl_diffuse_map().toUtf8());
            qDebug("MTL specular m:  " + mtl->get_mtl_specular_map().toUtf8());
            qDebug("MTL bump m:      " + mtl->get_mtl_bump_map().toUtf8());
            qDebug("MTL init:        " + QString::number(mtl->init()).toUtf8());   //textures get loaded !!!
            resman->add_material(mtl);
        }
    }


    //CREATE MESHS (if needed...)
    //QMap<QString,QVector<QVector3D> > mesh_faces;
    //QMap<QString,QString> mesh_mtl;
    //QVector<QVector3D> model_vertices;
    //QVector<QVector3D> model_vertex_normals;
    //QVector<QVector3D> model_vertex_texture_coordinates;

        //using mesh_mtl to iterate ...
    QList<QString> mesh_names = mesh_mtl.keys();
    for(int i = 0; i < mesh_names.length(); i++){
        //checked if Mesh already created...
        if(resman->check_mesh_loaded(mesh_names.value(i))){
            Mesh* mesh = resman->get_mesh_loaded(mesh_names.value(i));
            mesh->inc_count();
        }
        else{
                //needs .constData() every render pass in future...
            //QVector<QVector3D> vertices;
            //QVector<QVector2D> texcoords;
            //QVector<QVector3D> normals;
                //should be constant data and no need to convert...
            //qDebug(QString::number(mesh_faces[mesh_names.value(i)].size()).toUtf8());
            //qDebug(QString::number(mesh_faces[mesh_names.value(i)].size()*3).toUtf8());
            int triangle_count = mesh_faces[mesh_names.value(i)].size() / 3 / 3;
            qDebug(QString::number(triangle_count).toUtf8());
            GLfloat* vertices = new GLfloat[mesh_faces[mesh_names.value(i)].size()];
            GLfloat* texcoords = new GLfloat[mesh_faces[mesh_names.value(i)].size()]; //should be wrong ... 108/3*2 is right ...
            GLfloat* normals = new GLfloat[mesh_faces[mesh_names.value(i)].size()];

        //VAO and VBOs
            GLuint vertex_vbo;
            GLuint texcoord_vbo;
            GLuint normal_vbo;


            //qDebug("Mesh...");

            for(int j = 0; j < mesh_faces[mesh_names.value(i)].size(); j+=9){
                //  1 v/vt/vn   2 v/vt/vn   3 v/vt/vn

                //  v
                vertices[j]     = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j)  -1).x() * 0.01;
                vertices[j+1]   = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j)  -1).y() * 0.01;
                vertices[j+2]   = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j)  -1).z() * 0.01;

                vertices[3+j]   = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+3)-1).x() * 0.01;
                vertices[3+j+1] = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+3)-1).y() * 0.01;
                vertices[3+j+2] = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+3)-1).z() * 0.01;

                vertices[6+j]   = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+6)-1).x() * 0.01;
                vertices[6+j+1] = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+6)-1).y() * 0.01;
                vertices[6+j+2] = (GLfloat) model_vertices.value(mesh_faces[mesh_names.value(i)].value(j+6)-1).z() * 0.01;

                //  vt
                texcoords[j]     = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+1)-1).x();
                texcoords[j+1]   = (GLfloat) -model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+1)-1).y();
                texcoords[j+2]   = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+1)-1).z();

                texcoords[3+j]   = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+4)-1).x();
                texcoords[3+j+1] = (GLfloat) -model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+4)-1).y();
                texcoords[3+j+2] = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+4)-1).z();

                texcoords[6+j]   = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+7)-1).x();
                texcoords[6+j+1] = (GLfloat) -model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+7)-1).y();
                texcoords[6+j+2] = (GLfloat) model_vertex_texture_coordinates.value(mesh_faces[mesh_names.value(i)].value(j+7)-1).z();

                //  vn
                normals[j]     = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+2)-1).x();
                normals[j+1]   = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+2)-1).y();
                normals[j+2]   = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+2)-1).z();

                normals[3+j]   = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+5)-1).x();
                normals[3+j+1] = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+5)-1).y();
                normals[3+j+2] = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+5)-1).z();

                normals[6+j]   = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+8)-1).x();
                normals[6+j+1] = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+8)-1).y();
                normals[6+j+2] = (GLfloat) model_vertex_normals.value(mesh_faces[mesh_names.value(i)].value(j+8)-1).z();

            }


            glGenBuffers(1, &vertex_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
            glBufferData(GL_ARRAY_BUFFER, triangle_count * 3* 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

            glGenBuffers(1, &texcoord_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord_vbo);
            glBufferData(GL_ARRAY_BUFFER, triangle_count * 3 * 3 * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

            glGenBuffers(1, &normal_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
            glBufferData(GL_ARRAY_BUFFER, triangle_count * 3 * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);


            //Create Mesh and add it to the Mesh-list and register those in the resman...
            Mesh* mesh = new Mesh(mesh_names.value(i),triangle_count,vertices,texcoords,normals,
                                  vertex_vbo,texcoord_vbo,normal_vbo,
                                  resman->get_material_loaded(mesh_mtl.value(mesh_names.value(i))));
            resman->add_mesh(mesh);
            meshs.append(mesh);
        }

    }

    //finally add it to resources...
    resman->add_model(this);
    return true;
}
