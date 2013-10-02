#include "resources.h"

Resources::Resources()
{

}


bool Resources::check_model_loaded(QString id){
    QList<QString> model_ids = id_model.keys();
    if(model_ids.indexOf(id)==-1){
        return false;
    }
    else{
        return true;
    }
}

bool Resources::check_mesh_loaded(QString id){
    QList<QString> mesh_ids = id_mesh.keys();
    if(mesh_ids.indexOf(id)==-1){
        return false;
    }
    else{
        return true;
    }
}

bool Resources::check_material_loaded(QString id){
    QList<QString> material_ids = id_material.keys();
    if(material_ids.indexOf(id)==-1){
        return false;
    }
    else{
        return true;
    }
}

Model* Resources::get_model_loaded(QString id){
    if(check_model_loaded(id) == false){
        return NULL;
    }
    else{
        return id_model.value(id);
    }
}

Mesh* Resources::get_mesh_loaded(QString id){
    if(check_mesh_loaded(id) == false){
        return NULL;
    }
    else{
        return id_mesh.value(id);
    }
}

Material* Resources::get_material_loaded(QString id){
    if(check_material_loaded(id) == false){
        return NULL;
    }
    else{
        return id_material.value(id);
    }
}

QList<Mesh*> Resources::get_model_meshs(Model* temp){
    for(int i = 0; i<temp->get_meshs().length();i++){
        temp->get_meshs().value(i)->inc_count();
        temp->get_meshs().value(i)->get_material()->inc_count();
    }
    temp->inc_count();
    return get_model_loaded(temp->get_model_name())->get_meshs();
}

void Resources::add_model(Model* temp){

    //qDebug("   adding model to resman...");
    if(!check_model_loaded(temp->get_model_name())){
        id_model[temp->get_model_name()] = temp;
        //qDebug("      model wasn't loaded...");
    }

    //sort the meshs by material...
    //qDebug("   sorting by material...");
    for(int i = 0; i< temp->get_meshs().length(); i++){

        //add materials of meshs of model, check if allready inserted
        int index = material_instances.indexOf(temp->get_meshs().value(i)->get_material());
        //qDebug("   searched index of material instances: " + QString::number(index).toUtf8());
        if(index == -1){
            material_instances.append(temp->get_meshs().value(i)->get_material());
            //qDebug("      material added: " + temp->get_meshs().value(i)->get_material()->get_material_name().toUtf8());
            QList<Mesh*> mesh_list;
            mesh_list.append(temp->get_meshs().value(i));
            mesh_instances.append(mesh_list);

            QList<Model*> model_list;
            model_list.append(temp);
            model_instances.append(model_list);
        }
        else{
            mesh_instances[index].append(temp->get_meshs().value(i));
            model_instances[index].append(temp);
        }
        //qDebug("   used index of data insert: " + QString::number(index).toUtf8());
        //qDebug("   material_instances size: " + QString::number(mesh_instances.length()).toUtf8());
        //qDebug("   mesh_instances size of material list: " + QString::number(mesh_instances.value(index).length()).toUtf8());
        //qDebug("   model_instances size of material list: " + QString::number(model_instances.value(index).length()).toUtf8());
    }
}

void Resources::add_mesh(Mesh* temp){
    if(!check_mesh_loaded(temp->get_mesh_name())){
        id_mesh[temp->get_mesh_name()] = temp;
    }
}

void Resources::add_material(Material *temp){
    if(!check_material_loaded(temp->get_material_name())){
        id_material[temp->get_material_name()] = temp;
    }
}

QList< QList<Model*> > Resources::get_model_instances(){
    return model_instances;
}

QList< QList<Mesh*> > Resources::get_mesh_instances(){
    return mesh_instances;
}
QList<Material*> Resources::get_material_instances(){
    return material_instances;
}

