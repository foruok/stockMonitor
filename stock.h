#ifndef STOCK_H
#define STOCK_H
#include <QObject>

class StockDetail;
class StockProvider;

class StockState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loss READ loss WRITE setLoss)
    Q_PROPERTY(QString gain READ gain WRITE setGain)
public:
    StockState();
    StockState(const StockState & rhs);
    ~StockState();
    StockState & operator=(const StockState &rhs)
    {
        m_detail = rhs.m_detail;
        m_provider = rhs.m_provider;
        m_indexInProvider = rhs.m_indexInProvider;
        return *this;
    }

    QString loss();
    void setLoss(QString loss);
    QString gain();
    void setGain(QString gain);
    Q_INVOKABLE QString code();
    Q_INVOKABLE QString name();
    Q_INVOKABLE QString price();
    Q_INVOKABLE int index();

    void setProvider(StockProvider *provider)
    {
        m_provider = provider;
    }

    void setIndex(int index);

private:
    StockDetail *m_detail;
    StockProvider *m_provider;
    int m_indexInProvider;
};

#endif // STOCK_H
