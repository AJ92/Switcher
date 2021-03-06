#include "camera.h"

Camera::Camera()
{
    M_camera_view.setToIdentity();
    M_camera_rotation.setToIdentity();
    M_camera_translation.setToIdentity();
    Z_FAR = 1000.0f;
    Z_NEAR = 1.0;
    FOV = 45.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    camera_build();
}

void Camera::rotate(float angle, float axis_x, float axis_y, float axis_z)
{
    QMatrix4x4 camerarotation;
    camerarotation.rotate(angle, axis_x, axis_y, axis_z);
    M_camera_rotation = camerarotation * M_camera_rotation;
    camera_build();
}

void Camera::move(float x_dir, float y_dir, float z_dir)
{
    /*
            right     up    forward
                0     1     2               3
                4     5     6               7
                8     9    10               11


                12    13   14               15
    */

    x = x + (x_dir * M_camera_view.data()[0]);        //right / left
    y = y + (x_dir * M_camera_view.data()[4]);
    z = z + (x_dir * M_camera_view.data()[8]);

    x = x + (y_dir * M_camera_view.data()[1]);        //up / down
    y = y + (y_dir * M_camera_view.data()[5]);
    z = z + (y_dir * M_camera_view.data()[9]);

    x = x + (z_dir * M_camera_view.data()[2]);        //forward / backward
    y = y + (z_dir * M_camera_view.data()[6]);
    z = z + (z_dir * M_camera_view.data()[10]);

    M_camera_translation.translate(x,y,z);
    camera_build();
}

void Camera::translate(float x_dir, float y_dir, float z_dir)
{
    M_camera_translation.setToIdentity();
    M_camera_translation.translate(x_dir,y_dir,z_dir);
    camera_build();
}

void Camera::camera_build()
{
    M_camera_view = M_camera_rotation * M_camera_translation;
    //M_camera_view = M_camera_view.transposed();
}

Camera::~Camera()
{
    //destructor
}
