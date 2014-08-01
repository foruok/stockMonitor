#include "stockModel.h"
#include <QXmlStreamReader>
#include <QVector>
#include <QHash>
#include <QFile>
#include <QDebug>
#include "stockProvider.h"

class StockModelPrivate
{
public:
    StockModelPrivate(StockModel *m)
        : m_model(m)
    {
        int role = Qt::UserRole;
        m_roleNames.insert(role++, "code");
        m_roleNames.insert(role++, "name");
        m_roleNames.insert(role++, "current");
        m_roleNames.insert(role++, "offset");
        m_roleNames.insert(role++, "percent");
        m_roleNames.insert(role++, "handover");
        m_roleNames.insert(role++, "gainAndLose");
        m_roleNames.insert(role++, "upDown");
        m_roleNames.insert(role++, "turnover");
        m_currentStock.setProvider(&m_stocks);
    }

    StockModel * m_model;
    QHash<int, QByteArray> m_roleNames;
    StockProvider m_stocks;
    StockState m_currentStock;
};

StockModel::StockModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_dptr(new StockModelPrivate(this))
{
    connect( &(m_dptr->m_stocks), SIGNAL(refreshed()),
             this, SLOT(onStockDataRefreshed()) );
    connect( &(m_dptr->m_stocks), SIGNAL(beginRefresh()),
             this, SLOT(onStockDataBeginRefresh()) );
}

StockModel::~StockModel()
{
    delete m_dptr;
}

int StockModel::rowCount(const QModelIndex &parent) const
{
    return m_dptr->m_stocks.stockCount();
}

QVariant StockModel::data(const QModelIndex &index, int role) const
{
    StockDetail *stock = m_dptr->m_stocks.stockAt(index.row());
    switch(role - Qt::UserRole)
    {
    case 0: // code
        return stock->code();
    case 1: // name
        return stock->name();
    case 2: // current
        return stock->currentPrice();
    case 3: // offset
        return stock->rise();
    case 4: // percent
        return stock->risePercent();
    case 5: // handover
        return stock->handoverRate();
    case 6: // stopLose
        return QString("%1/%2")
                .arg(stock->stopWinPrice(), 0, 'f', 2)
                .arg(stock->stopLosePrice(), 0, 'f', 2);
    case 7: // upDown
        return QString("%1/%2").arg(stock->todayHighestPrice().data())
                .arg(stock->todayLowestPrice().data());
    case 8: // turnover
        return stock->transactionMoney();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> StockModel::roleNames() const
{
    return m_dptr->m_roleNames;
}

int StockModel::updateInterval()
{
    return m_dptr->m_stocks.updateInterval();
}

void StockModel::setUpdateInterval(int seconds)
{
    m_dptr->m_stocks.setUpdateInterval(seconds);
}

int StockModel::stateOf(int row)
{
    StockDetail *stock = m_dptr->m_stocks.stockAt(row);
    return stock->riseMode();
}

void StockModel::addStock(QString code)
{
    int count = m_dptr->m_stocks.stockCount();
    if(m_dptr->m_stocks.addStock(code.toLatin1()))
    {
        QModelIndex parent;
        beginInsertRows(parent, count, count);
        endInsertRows();
    }
}

void StockModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_dptr->m_stocks.removeStock(row);
    endRemoveRows();
}

void StockModel::updateCurrentStock(int row)
{
    m_dptr->m_currentStock.setIndex(row);
}

void StockModel::saveState()
{
    m_dptr->m_stocks.saveState();
    int row = m_dptr->m_currentStock.index();
    QModelIndex topLeft = createIndex(row, 2);
    QModelIndex bottomRight = createIndex(row, 8);
    emit dataChanged(topLeft, bottomRight);
}

StockState *StockModel::currentStock()
{
    return &m_dptr->m_currentStock;
}

void StockModel::connectToReminder(QObject *receiver, const char *method)
{
    QObject::connect(&m_dptr->m_stocks, SIGNAL(remind(QString)),
                     receiver, method);
}

void StockModel::onStockDataBeginRefresh()
{
    //beginResetModel();
}

void StockModel::onStockDataRefreshed()
{
    //qDebug() << "onStockDataChange";
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(m_dptr->m_stocks.stockCount() - 1, 8);
    emit dataChanged(topLeft, bottomRight);
}
