#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "stockModel.h"
#include "stock.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QTranslator t;
    t.load(":/stockMonitor_zh.qm");
    app.installTranslator(&t);

    QQmlApplicationEngine engine;
    StockModel *model = new StockModel;
    engine.rootContext()->setContextProperty("stockModel", model);
    engine.rootContext()->setContextProperty("currentStock", model->currentStock());
    engine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
