#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "stockModel.h"
#include "stock.h"
#include <QTranslator>
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
#include <QApplication>
#include "trayIcon.h"
#endif
#include <QDebug>

int main(int argc, char *argv[])
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    QTranslator t;
    t.load(":/stockMonitor_zh.qm");
    app.installTranslator(&t);  

    QQmlApplicationEngine engine;
    StockModel *model = new StockModel;
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("stockModel", model);
    ctx->setContextProperty("currentStock", model->currentStock());
    engine.load(QUrl("qrc:/qml/main.qml"));

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    TrayIcon *tray = new TrayIcon();
    QList<QObject*> objects = engine.rootObjects();
    foreach (QObject* obj, objects)
    {
        if(obj->objectName() == "rootWindow")
        {
            tray->setWindow(qobject_cast<QWindow*>(obj));
            model->connectToReminder(tray, SLOT(onRemind(QString)));
            break;
        }
    }
#endif

    return app.exec();
}
