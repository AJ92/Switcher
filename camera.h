#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector>
#include <QtGui/qvector3d.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qmatrix4x4.h>

class Camera
{
public:
    Camera();
    ~Camera();
    float x;
    float y;
    float z;
    float Z_FAR;
    float Z_NEAR;
    float FOV;
    void rotate(float angle, float axis_x, float axis_y, float axis_z);
    void move(float x_dir, float y_dir, float z_dir);
    void translate(float x_dir, float y_dir, float z_dir);
    void camera_build();


    QMatrix4x4 M_camera_view;
    QMatrix4x4 M_camera_rotation;
    QMatrix4x4 M_camera_translation;

signals:

public slots:

};

#endif // CAMERA_H
