#ifndef RESOURCES_H
#define RESOURCES_H

#include "Model/model.h"
#include "Model/mesh.h"
#include "Model/material.h"

#include <QMap>

class Model;
class Mesh;
class Material;

class Resources
{
public:
    Resources();

    //checks if loaded already
    bool check_model_loaded(QString id);
    bool check_mesh_loaded(QString id);
    bool check_material_loaded(QString id);

    //returns loaded data
    Model* get_model_loaded(QString id);
    Mesh* get_mesh_loaded(QString id);
    Material* get_material_loaded(QString id);

    //returns Mesh list (used after check if a model aready exists
    //and just pointing to existing data instead of creating new)
    QList<Mesh*> get_model_meshs(Model* temp);

    //adds data to the resources
    void add_model(Model* temp);
    void add_mesh(Mesh *temp);
    void add_material(Material* temp);

    QList< QList<Model*> > get_model_instances();
    QList< QList<Mesh*> > get_mesh_instances();
    QList<Material*> get_material_instances();

private:
    //instances of meshs sorted by meshnames to prevent multiple array and texture state switches:


    //instances / pointers to models to be rendered
    QList< QList<Model*> > model_instances;

    QList< QList<Mesh*> > mesh_instances;

    QList<Material*> material_instances;

    //loaded DATA
    QMap<QString,Model*> id_model;
    QMap<QString,Mesh*> id_mesh;
    QMap<QString,Material*> id_material;
};

#endif // RESOURCES_H
