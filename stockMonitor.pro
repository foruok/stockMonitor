QT += qml quick network

win32|unix|macx {
    QT += widgets
    SOURCES += trayIcon.cpp
    HEADERS += trayIcon.h
}
# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    stockModel.cpp \
    stockProvider.cpp \
    stock.cpp

# Installation path
# target.path =

RESOURCES += \
    stockMonitor.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

TRANSLATIONS = stockMonitor_zh.ts

HEADERS += \
    stockModel.h \
    stockProvider.h \
    stock.h
