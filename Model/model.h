#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QtGui/qvector3d.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qmatrix4x4.h>
#include <QString>
#include <QStringList>

#include <QMap>

#include "resources.h"
#include "mesh.h"
#include "material.h"

//load files
#include <qfile.h>
#include <qtextstream.h>

class Resources;
class Mesh;
class Material;

class Model
{
public:
    Model(QString path, Resources* resources,bool &ok);

    QMatrix4x4 m_rotation;
    QMatrix4x4 m_translation;
    QMatrix4x4 m_scale;
    //final matrix
    QMatrix4x4 m_modelview;

    void rotate         (float angle, float x_axis, float y_axis, float z_axis);
    void rotate_reset   ();
    void translate      (float x_pos, float y_pos,  float z_pos);
    void scale          (float x_sca, float y_sca,  float z_sca);

    void build_model_view();

    QVector3D get_view_dir();
    QVector3D get_pos();
    QVector3D get_scale();


    void inc_count();
    void dec_count();
    long get_count();

    QString get_model_name();
    QList<Mesh*> get_meshs();

private:

    //position rotation scalation...
    float x;
    float y;
    float z;

    float x_scale;
    float y_scale;
    float z_scale;

    int debug;

    //MODEL ATTRIBUTES:
    int visible;
    int lit;
    QVector3D light_color;
    float light_strength;
    int alpha_transparency;
    int black_transparency;
    float transparency;
    int sort;

    //MODEL DATA:
    QString model_name;
    long count; //used to count current uses

    //MODEL RESOURCE DATA:
    Resources* resman;
    QList<Mesh*> meshs;

    bool load_file_obj(QString path);
};

#endif // MODEL_H
