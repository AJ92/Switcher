#include "drivemanager.h"

Drivemanager::Drivemanager()
{
}

QList<QString> Drivemanager::get_drives(){

    TInt err = KErrNone;
    QList<int> drives;

    for(int i = 0; i < 28; i++){
        TBool isvalid = RFs :: IsValidDrive((TInt) i);
        if(isvalid){
            drives.append(i);
        }
    }

    QList<QString> drives_letter;
    for(int i = 0; i < drives.length(); i++){
        TChar drive_char;
        err = RFs :: DriveToChar((TInt) drives.at(i), drive_char);
        if(err == KErrNone){
            if(get_drive_type(QString((char) drive_char))!=0/* &&
                    get_drive_type(QString((char) drive_char))!= 1*/){

                drives_letter.append(QString((char) drive_char));
                qDebug(QString((char) drive_char).toUtf8());
            }
        }
    }
    qDebug(QString::number(drives_letter.length()).toUtf8());
    return drives_letter;
}


//Drive attribs
QString Drivemanager::map_type_to_string(int drive_type){
    QString type = "none";
    switch(drive_type){
    case 0:
        type =  "NotPresent";
        break;

    case 1:
        type =  "Unknown";
        break;

    case 2:
        type =  "Floppy";
        break;

    case 3:
        type =  "HardDisk";
        break;

    case 4:
        type =  "CdRom";
        break;

    case 5:
        type =  "Ram";
        break;

    case 6:
        type =  "Flash";
        break;

    case 7:
        type =  "Rom";
        break;

    case 8:
        type =  "Remote";
        break;

    case 9:
        type =  "NANDFlash";
        break;

    case 10:
        type =  "RotatingMedia";
    }
    return type;
}

int Drivemanager::get_drive_type(QString drive){
    TInt err = KErrNone;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    int ret_value = -1;

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            TMediaType mt = driveInfo.iType;
            switch(mt){
            case EMediaNotPresent:
                ret_value =  0;
                break;

            case EMediaUnknown:
                ret_value =  1;
                break;

            case EMediaFloppy:
                ret_value =  2;
                break;

            case EMediaHardDisk:
                ret_value =  3;
                break;

            case EMediaCdRom:
                ret_value =  4;
                break;

            case EMediaRam:
                ret_value =  5;
                break;

            case EMediaFlash:
                ret_value =  6;
                break;

            case EMediaRom:
                ret_value =  7;
                break;

            case EMediaRemote:
                ret_value =  8;
                break;

            case EMediaNANDFlash:
                ret_value =  9;
                break;

            case EMediaRotatingMedia:
                ret_value =  10;
            }

        }
        else
            err = KErrArgument;
    }
    fs.Close();
    return ret_value;
}


QString Drivemanager::map_media_attribute_to_string(int drive_attribute){
    QString attribute = "none";
    switch(drive_attribute){

    case 0:
        attribute =  "VariableSize";
        break;

    case 1:
        attribute =  "DualDensity";
        break;

    case 2:
        attribute =  "Formattable";
        break;

    case 3:
        attribute =  "WriteProtected";
        break;

    case 4:
        attribute =  "Lockable";
        break;

    case 5:
        attribute =  "Locked";
    }
    return attribute;
}

int Drivemanager::get_media_attributes(QString drive){
    TInt err = KErrNone;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    int ret_value = -1;

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            TUint da = driveInfo.iMediaAtt;
            switch(da){
            case KMediaAttVariableSize:
                ret_value = 0;
                break;

            case KMediaAttDualDensity:
                ret_value = 1;
                break;

            case KMediaAttFormattable:
                ret_value = 2;
                break;

            case KMediaAttWriteProtected:
                ret_value = 3;
                break;

            case KMediaAttLockable:
                ret_value = 4;
                break;

            case KMediaAttLocked:
                ret_value = 5;

            }

        }
        else
            err = KErrArgument;
    }
    fs.Close();
    return ret_value;
}


QString Drivemanager::map_drive_attribute_to_string(int drive_attribute){
    QString attribute = "none";
    switch(drive_attribute){
    case 0:
        attribute =  "Local";
        break;

    case 1:
        attribute =  "Rom";
        break;

    case 2:
        attribute =  "Redirected";
        break;

    case 3:
        attribute =  "Substed";
        break;

    case 4:
        attribute =  "Internal";
        break;

    case 5:
        attribute =  "Removable";
    }
    return attribute;
}

int Drivemanager::get_drive_attributes(QString drive){
    TInt err = KErrNone;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    int ret_value = -1;

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            if(driveInfo.iDriveAtt == KDriveAttLocal){
                ret_value = 0;
            }
            else if(driveInfo.iDriveAtt == KDriveAttRom){
                ret_value = 1;
            }
            else if(driveInfo.iDriveAtt == KDriveAttRedirected){
                ret_value = 2;
            }
            else if(driveInfo.iDriveAtt == KDriveAttSubsted){
                ret_value = 3;
            }
            else if(driveInfo.iDriveAtt == KDriveAttInternal){
                ret_value = 4;
            }
            else if(driveInfo.iDriveAtt == KDriveAttRemovable){
                ret_value = 5;
            }
        }
        else
            err = KErrArgument;
    }
    fs.Close();
    qDebug(QString::number(ret_value).toUtf8());
    return ret_value;
}


QString Drivemanager::map_connection_type_to_string(int connection_type){
    QString type = "none";
    switch(connection_type){
    case 0:
        type =  "Local";
        break;

    case 1:
        type =  "USB";
    }
    return type;
}

int Drivemanager::get_drive_connection_type(QString drive){
    TInt err = KErrNone;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    int ret_value = -1;

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            TConnectionBusType cbt = driveInfo.iConnectionBusType;
            switch(cbt){
            case EConnectionBusInternal:
                ret_value = 0;
                break;

            case EConnectionBusUsb:
                ret_value = 1;
            }

        }
        else
            err = KErrArgument;
    }
    fs.Close();
    return ret_value;
}


QString Drivemanager::map_battery_state_to_string(int battery_state){
    QString state = "none";
    switch(battery_state){
    case 0:
        state = "NotSupported";
        break;

    case 1:
        state = "Good";
        break;

    case 2:
        state = "Low";
    }
    return state;
}

int Drivemanager::get_drive_battery_state(QString drive){
    TInt err = KErrNone;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    int ret_value = -1;

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            TBatteryState bs = driveInfo.iBattery;
            switch(bs){
            case EBatNotSupported:
                ret_value = 0;
                break;

            case EBatGood:
                ret_value = 1;
                break;

            case EBatLow:
                ret_value = 2;
            }

        }
        else
            err = KErrArgument;
    }
    fs.Close();
    return ret_value;
}



QString Drivemanager::get_drive_name(QString drive){
    //TODO
    //GetDriveName ( TInt, TDes & );
    return QString("so far none...");
}


//volume attribs
qint64 Drivemanager::get_volume_size_b(QString drive){
    TInt err = KErrNone;

    TInt64 aDriveTotalSize = 0;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );

        if(err == KErrNone)
        {
            if(driveInfo.iMediaAtt & KMediaAttFormattable) // size defined only for formattable drives
            {
                TVolumeInfo volumeInfo;
                err = fs.Volume(volumeInfo, driveIndex);
                if(err == KErrNone)
                {
                    // now we can get drive size info
                    aDriveTotalSize = volumeInfo.iSize;
                }
            }
            else
                err = KErrArgument;
        }
        else
            err = KErrArgument;
    }
    fs.Close();

    return aDriveTotalSize;
}

qint64 Drivemanager::get_volume_free_size_b(QString drive){
    TInt err = KErrNone;

    TInt64 aDriveFreeSize = 0;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            if(driveInfo.iMediaAtt & KMediaAttFormattable) // size defined only for formattable drives
            {
                TVolumeInfo volumeInfo;
                err = fs.Volume(volumeInfo, driveIndex);
                if(err == KErrNone)
                {
                    // now we can get drive size info
                    aDriveFreeSize = volumeInfo.iFree;
                }
            }
            else
                err = KErrArgument;
        }
        else
            err = KErrArgument;
    }
    fs.Close();

    return aDriveFreeSize;
}


QString Drivemanager::map_volume_cache_flag_to_string(int volume_cache_flags){
    QString cache_flag = "none";
    switch(volume_cache_flags){
    case 0:
        cache_flag = "ReadEnabled";
        break;

    case 1:
        cache_flag = "ReadOn";
        break;

    case 2:
        cache_flag = "ReadAheadEnabled";
        break;

    case 3:
        cache_flag = "ReadAheadOn";
        break;

    case 4:
        cache_flag = "WriteEnabled";
        break;

    case 5:
        cache_flag = "WriteOn";

    }

    return cache_flag;
}

int Drivemanager::get_volume_cache_flags(QString drive){
    TInt err = KErrNone;

    int volume_cache_flag;

    RFs fs;
    User::LeaveIfError( fs.Connect() );

    TInt driveIndex;

    QByteArray byteArray = drive.toUtf8();
    const char* cString = byteArray.constData();

    err = RFs :: CharToDrive( (TChar) cString[0], driveIndex );
    if(err == KErrNone)
    {
        TDriveInfo driveInfo;
        err = fs.Drive( driveInfo, driveIndex );
        if(err == KErrNone)
        {
            if(driveInfo.iMediaAtt & KMediaAttFormattable)
            {
                TVolumeInfo volumeInfo;
                err = fs.Volume(volumeInfo, driveIndex);
                if(err == KErrNone)
                {

                    if(volumeInfo.iFileCacheFlags == EFileCacheReadEnabled){
                        volume_cache_flag = 0;
                    }
                    else if(volumeInfo.iFileCacheFlags & EFileCacheReadOn){
                        volume_cache_flag = 1;
                    }
                    else if(volumeInfo.iFileCacheFlags & EFileCacheReadAheadEnabled){
                        volume_cache_flag = 2;
                    }
                    else if(volumeInfo.iFileCacheFlags & EFileCacheReadAheadOn){
                        volume_cache_flag = 3;
                    }
                    else if(volumeInfo.iFileCacheFlags & EFileCacheWriteEnabled){
                        volume_cache_flag = 4;
                    }
                    else if(volumeInfo.iFileCacheFlags & EFileCacheWriteOn){
                        volume_cache_flag = 5;
                    }
                }
            }
            else
                err = KErrArgument;
        }
        else
            err = KErrArgument;
    }
    fs.Close();

    return volume_cache_flag;
}
