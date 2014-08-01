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
    stock.cpp \
    qDebug2Logcat.cpp


# Installation path
# target.path =

RESOURCES += \
    stockMonitor.qrc

android-g++ {
    LIBS += -lgnustl_shared
    QT += androidextras
    SOURCES += notifyAndroid.cpp
    HEADERS += notifyAndroid.h
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
    android/src/an/qt/stockMonitor/StockMonitorActivity.java

TRANSLATIONS = stockMonitor_zh.ts

HEADERS += \
    stockModel.h \
    stockProvider.h \
    stock.h \
    qDebug2Logcat.h
