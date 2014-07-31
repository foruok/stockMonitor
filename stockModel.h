#ifndef STOCKMODEL_H
#define STOCKMODEL_H
#include <QAbstractListModel>
#include "stock.h"

class StockModelPrivate;
class StockModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval)
public:
    StockModel(QObject *parent = 0);
    ~StockModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    int updateInterval();
    void setUpdateInterval(int seconds);

    Q_INVOKABLE int stateOf(int row);
    Q_INVOKABLE void addStock(QString code);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void updateCurrentStock(int row);
    Q_INVOKABLE void saveState();
    StockState* currentStock();
    void connectToReminder(QObject *receiver, const char *method);

protected slots:
    void onStockDataBeginRefresh();
    void onStockDataRefreshed();

private:
    StockModelPrivate * m_dptr;
};

#endif // STOCKMODEL_H
