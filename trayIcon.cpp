#include "trayIcon.h"
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QApplication>
#include <QMenu>

TrayIcon::TrayIcon(QObject *parent)
    : QObject(parent), m_window(0)
    , m_icon(":/res/stockIcon_32x32.png")
{
    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    m_trayIcon->setIcon(m_icon);
    m_trayIcon->setToolTip(tr("stock monitor"));

    ///< 将托盘菜单的父设置为桌面，托盘菜单在失去焦点时会自动消失
    m_trayMenu = new QMenu((QWidget*)QApplication::desktop());
    m_trayMenu->addAction(tr("Exit"), this, SLOT(onExit()));
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->show();
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick && m_window)
    {
        switch(m_window->windowState())
        {
        case Qt::WindowMinimized:
            m_window->showNormal();
            break;
        }

        m_window->requestActivate();
    }
}

void TrayIcon::onExit()
{
    m_trayIcon->hide();
    QCoreApplication::exit(0);
}

void TrayIcon::onRemind(const QString &contents)
{
    m_trayIcon->showMessage(tr("Notification"), contents);
}
