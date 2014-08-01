#include "notifyAndroid.h"
#include <QtAndroidExtras/QAndroidJniObject>
#include <QAndroidJniEnvironment>

NotifyAndroid::NotifyAndroid(QObject *parent)
    : QObject(parent)
{}
NotifyAndroid::~NotifyAndroid()
{}

void NotifyAndroid::onRemind(const QString &contents)
{
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(contents);
    QAndroidJniObject::callStaticMethod<void>("an/qt/stockMonitor/StockMonitorActivity",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}
