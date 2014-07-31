#ifndef TRAYICON_H
#define TRAYICON_H
#include <QSystemTrayIcon>
#include <QWindow>

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    TrayIcon(QObject *parent = 0);
    ~TrayIcon();

    void setWindow(QWindow *window)
    {
        m_window = window;
    }

protected slots:
    void onTrayIconActivated( QSystemTrayIcon::ActivationReason reason);
    void onExit();
    void onRemind(const QString &contents);

protected:
    QSystemTrayIcon * m_trayIcon;
    QMenu * m_trayMenu;
    QWindow * m_window;
};

#endif // TRAYICON_H
