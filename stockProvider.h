#ifndef STOCKPROVIDER_H
#define STOCKPROVIDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QTime>
#include <QByteArray>

class StockProvider;

class StockDetail
{
    friend class StockProvider;
public:
    StockDetail(const QByteArray & stockCode);
    ~StockDetail();

    bool detailAvailable() const;

    const QByteArray & name() const;
    const QByteArray & code() const;
    const QByteArray & todayOpenPrice() const;
    const QByteArray & yesterdayClosePrice() const;
    const QByteArray & currentPrice() const;
    const QByteArray & todayHighestPrice() const;
    const QByteArray & todayLowestPrice() const;
    const QByteArray & transactionStocks() const;
    const QByteArray & transactionMoney() const;
    const QByteArray & buyOneAmount() const;
    const QByteArray & buyOnePrice() const;
    const QByteArray & buyTwoAmount() const;
    const QByteArray & buyTwoPrice() const;
    const QByteArray & buyThreeAmount() const;
    const QByteArray & buyThreePrice() const;
    const QByteArray & buyFourAmount() const;
    const QByteArray & buyFourPrice() const;
    const QByteArray & buyFiveAmount() const;
    const QByteArray & buyFivePrice() const;
    const QByteArray & saleOneAmount() const;
    const QByteArray & saleOnePrice() const;
    const QByteArray & saleTwoAmount() const;
    const QByteArray & saleTwoPrice() const;
    const QByteArray & saleThreeAmount() const;
    const QByteArray & saleThreePrice() const;
    const QByteArray & saleFourAmount() const;
    const QByteArray & saleFourPrice() const;
    const QByteArray & saleFiveAmount() const;
    const QByteArray & saleFivePrice() const;
    const QByteArray & date() const ;
    const QByteArray & time() const;
    const QString rise() const;
    const QString risePercent() const ;
    int riseMode() const;
    bool arriveStopWin() const;
    bool arriveStopLose() const;
    void setStopWinPrice(double price);
    double stopWinPrice() const;
    void setStopLosePrice(double price);
    double stopLosePrice() const;
    bool isDetailsValid() const;
    const QString handoverRate() const;

protected:
    QByteArray m_strCode;
    QList<QByteArray> m_details;
    double m_stopWinPrice;
    double m_stopLosePrice;
    double m_totalTransactStocks;
};

class StockProvider : public QObject
{
    Q_OBJECT
public:
    StockProvider(QObject * parent = 0);
    ~StockProvider();

    bool addStock(const QByteArray & stockCode);
    void removeStock(const QByteArray & stockCode);
    void removeStock(int index);
    StockDetail * stock(const QByteArray & stockCode);
    int stockCount();
    StockDetail * stockAt(int index);

    int remindMode();
    void setRemindMode(int mode);

    void saveState();

    int updateInterval() { return m_refreshInterval; }
    void setUpdateInterval(int seconds);

signals:
    void beginRefresh();
    void refreshed();
    void remind(const QString & contents);

protected slots:
    void onRefreshFinished();
    void onRefreshError(QNetworkReply::NetworkError code);

protected:
    void timerEvent(QTimerEvent *);
    void saveToFile();
    void readFromFile();
    void startUpdate();
    void parseData(const QByteArray & data);
    void updateStockDetail(const QByteArray & stockCode, const QByteArray & data);
    void updateRemindContents(StockDetail * stock);
    bool isTransactTime();
    void startUpdateImmediately();

protected:
    QList<StockDetail*> m_stocks;
    int m_iRefreshTimer;
    int m_iRefreshTimeoutTimer;
    QNetworkAccessManager m_nam;
    QTime m_updateElapsed;
    int m_iRemindMode;
    int m_refreshInterval;
    QNetworkReply *m_reply;
    bool m_bShouldSave;
    QString m_strRemind;
    bool m_bNoRefreshAfterLaunch;
    int m_refreshingCount;
};
#endif // STOCKPROVIDER_H
