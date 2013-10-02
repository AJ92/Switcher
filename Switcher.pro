# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0x2006B9CB


# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
# symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += PowerMgmt \ ProtServ \ #NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
 CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    taskmanager.cpp \
    ckeycapturer.cpp \
    tasksettings.cpp \
    taskevent.cpp \
    taskprioritythread.cpp \
    taskpriorityprocess.cpp \
    taskpriority.cpp \
    rcqlistwidget.cpp \
    taskaccessmanager.cpp \
    systeminfo.cpp \
    drivemanager.cpp \
    energymanager.cpp \
    settings.cpp \
    info.cpp \
    halmanager.cpp \
    log.cpp \
    settingsstore.cpp \
    test.cpp
HEADERS += mainwindow.h \
    taskmanager.h \
    ckeycapturer.h \
    tasksettings.h \
    taskevent.h \
    taskprioritythread.h \
    taskpriorityprocess.h \
    taskpriority.h \
    rcqlistwidget.h \
    taskaccessmanager.h \
    systeminfo.h \
    drivemanager.h \
    energymanager.h \
    settings.h \
    info.h \
    halmanager.h \
    log.h \
    settingsstore.h \
    test.h
FORMS += mainwindow.ui \
    tasksettings.ui \
    taskevent.ui \
    taskprioritythread.ui \
    taskpriorityprocess.ui \
    taskpriority.ui \
    systeminfo.ui \
    settings.ui \
    info.ui \
    log.ui

symbian:LIBS += -lcone  -leikcore  -lavkon  -lapgrfx  -lws32  -lgdi -lws32 -lefsrv -lhal

    # Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

symbian:TARGET.EPOCHEAPSIZE = 0x800000


 symbian {
      TARGET.UID3 = 0x2006B9CB
      DEPLOYMENT.installer_header=0x2002CCCF

      vendorinfo = \
      "%{\"3Devs\"}" \
      ":\"3Devs\""

      my_deployment.pkg_prerules = vendorinfo
      DEPLOYMENT += my_deployment
  }

    # For enabling hardware floats.
symbian:MMP_RULES += "OPTION gcce -march=armv6"
symbian:MMP_RULES += "OPTION gcce -mfpu=vfp"
symbian:MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
symbian:MMP_RULES += "OPTION gcce -marm"

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES +=
