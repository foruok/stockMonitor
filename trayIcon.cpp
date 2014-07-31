#include "trayIcon.h"
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QApplication>
#include <QMenu>

QPixmap drawTrayIcon()
{
    QPixmap pixmap(32,32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QRect rc = pixmap.rect().adjusted(2,2,-2,-2);
    QBrush br(QColor(220,0,0));
    QPen pen(br,2);
    painter.setPen(pen);
    painter.setBrush(br);
    painter.drawEllipse(rc);
    rc.adjust(6, 6,-6,-6);
    painter.fillRect(rc, Qt::green);
    painter.end();
    return pixmap;
}

TrayIcon::TrayIcon(QObject *parent)
    : QObject(parent), m_window(0)
{
    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    m_trayIcon->setIcon(QIcon(drawTrayIcon()));
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
