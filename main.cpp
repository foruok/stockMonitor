#include <QQmlContext>
#include "stockModel.h"
#include "stock.h"
#include <QTranslator>
#if defined(Q_OS_ANDROID)
#include <QtGui/QGuiApplication>
#include "notifyAndroid.h"
#elif defined(Q_OS_IOS)
#else
#include <QApplication>
#include "trayIcon.h"
#endif
#include <QDebug>
#include "qDebug2Logcat.h"
#include <QQuickView>

int main(int argc, char *argv[])
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    installLogcatMessageHandler("stockMonitor");
    QTranslator t;
    t.load(":/stockMonitor_zh.qm");
    app.installTranslator(&t);  

    StockModel *model = new StockModel;
    QQuickView viewer;
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext *ctx = viewer.rootContext();
    ctx->setContextProperty("stockModel", model);
    ctx->setContextProperty("currentStock", model->currentStock());
    viewer.setSource(QUrl("qrc:/qml/main.qml"));
#if defined(Q_OS_ANDROID)
    NotifyAndroid *notify = new NotifyAndroid;
    model->connectToReminder(notify, SLOT(onRemind(QString)));
#elif defined(Q_OS_IOS)
#else
    TrayIcon *tray = new TrayIcon();
    tray->setWindow(&viewer);
    model->connectToReminder(tray, SLOT(onRemind(QString)));

    viewer.setIcon(tray->icon());
    viewer.setTitle(QObject::tr("stock monitor"));
    viewer.setMinimumHeight(480);
    viewer.setMinimumWidth(800);
#endif

    viewer.show();

    return app.exec();
}
