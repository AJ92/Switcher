#ifndef DRIVEMANAGER_H
#define DRIVEMANAGER_H

#include <f32file.h>
#include <QList>
#include <QString>

class Drivemanager
{
public:
    Drivemanager();
    QList<QString> get_drives();



    //Drive attribs
    QString map_type_to_string(int drive_type);
    int get_drive_type(QString drive);

    QString map_media_attribute_to_string(int drive_attribute);
    int get_media_attributes(QString drive);

    QString map_drive_attribute_to_string(int drive_attribute);
    int get_drive_attributes(QString drive);

    QString map_connection_type_to_string(int connection_type);
    int get_drive_connection_type(QString drive);

    QString map_battery_state_to_string(int battery_state);
    int get_drive_battery_state(QString drive);


    QString get_drive_name(QString drive);


    //volume attribs
    qint64 get_volume_size_b(QString drive);
    qint64 get_volume_free_size_b(QString drive);

    QString map_volume_cache_flag_to_string(int volume_cache_flags);
    int get_volume_cache_flags(QString drive);


};

#endif // DRIVEMANAGER_H
