#ifndef NOTIFYANDROID_H
#define NOTIFYANDROID_H
#include <QObject>

class NotifyAndroid : public QObject
{
    Q_OBJECT
public:
    NotifyAndroid(QObject *parent = 0);
    ~NotifyAndroid();

public slots:
    void onRemind(const QString & contents);
};

#endif // NOTIFYANDROID_H
