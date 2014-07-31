#include "stockProvider.h"
#include <QTimerEvent>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QDebug>
#include <QObjectCleanupHandler>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QNetworkReply>

#define REFRESH_PERIOD  1000
#define STOCK_START_STRING "var hq_str_"
#define STOCK_START_STRING_LEN  11

enum {
    stock_name = 0,
    today_open,
    yesterday_close,
    current_price,
    today_highest_price,
    today_lowest_price,
    buy_1_price_0,
    sale_1_price_0,
    transaction_stocks,
    transaction_money,
    buy_1_amount,
    buy_1_price,
    buy_2_amount,
    buy_2_price,
    buy_3_amount,
    buy_3_price,
    buy_4_amount,
    buy_4_price,
    buy_5_amount,
    buy_5_price,
    sale_1_amount,
    sale_1_price,
    sale_2_amount,
    sale_2_price,
    sale_3_amount,
    sale_3_price,
    sale_4_amount,
    sale_4_price,
    sale_5_amount,
    sale_5_price,
    stock_date,
    stock_time
};

StockDetail::StockDetail(const QByteArray &stockCode) : m_strCode(stockCode)
  , m_stopWinPrice(-1), m_stopLosePrice(-1), m_totalTransactStocks(-1)
{
    QByteArray dummy("-");
    for(int i = 0; i <= stock_time; i++)
    {
        m_details.append(dummy);
    }
}

StockDetail::~StockDetail()
{
}

const bool StockDetail::detailAvailable() const
{
    return m_details.size() > stock_time;
}

const QByteArray & StockDetail::name() const
{
    return m_details.at(stock_name);
}

const QByteArray & StockDetail::code() const
{
    return m_strCode;
}

const QByteArray & StockDetail::todayOpenPrice() const
{
    return m_details.at(today_open);
}

const QByteArray & StockDetail::yesterdayClosePrice() const
{
    return m_details.at(yesterday_close);
}

const QByteArray & StockDetail::currentPrice() const
{
    return m_details.at(current_price);
}

const QByteArray & StockDetail::todayHighestPrice() const
{
    return m_details.at(today_highest_price);
}

const QByteArray & StockDetail::todayLowestPrice() const
{
    return m_details.at(today_lowest_price);
}

const QByteArray & StockDetail::transactionStocks() const
{
    return m_details.at(transaction_stocks);
}

const QByteArray & StockDetail::transactionMoney() const
{
    return m_details.at(transaction_money);
}

const QByteArray & StockDetail::buyOneAmount() const
{
    return m_details.at(buy_1_amount);
}

const QByteArray & StockDetail::buyOnePrice() const
{
    return m_details.at(buy_1_price);
}

const QByteArray & StockDetail::buyTwoAmount() const
{
    return m_details.at(buy_2_amount);
}

const QByteArray & StockDetail::buyTwoPrice() const
{
    return m_details.at(buy_2_price);
}

const QByteArray & StockDetail::buyThreeAmount() const
{
    return m_details.at(buy_3_amount);
}

const QByteArray & StockDetail::buyThreePrice() const
{
    return m_details.at(buy_3_price);
}

const QByteArray & StockDetail::buyFourAmount() const
{
    return m_details.at(buy_4_amount);
}

const QByteArray & StockDetail::buyFourPrice() const
{
    return m_details.at(buy_4_price);
}

const QByteArray & StockDetail::buyFiveAmount() const
{
    return m_details.at(buy_5_amount);
}

const QByteArray & StockDetail::buyFivePrice() const
{
    return m_details.at(buy_5_price);
}

const QByteArray & StockDetail::saleOneAmount() const
{
    return m_details.at(sale_1_amount);
}

const QByteArray & StockDetail::saleOnePrice() const
{
    return m_details.at(sale_1_price);
}

const QByteArray & StockDetail::saleTwoAmount() const
{
    return m_details.at(sale_2_amount);
}

const QByteArray & StockDetail::saleTwoPrice() const
{
    return m_details.at(sale_2_price);
}

const QByteArray & StockDetail::saleThreeAmount() const
{
    return m_details.at(sale_3_amount);
}

const QByteArray & StockDetail::saleThreePrice() const
{
    return m_details.at(sale_3_price);
}

const QByteArray & StockDetail::saleFourAmount() const
{
    return m_details.at(sale_4_amount);
}

const QByteArray & StockDetail::saleFourPrice() const
{
    return m_details.at(sale_4_price);
}

const QByteArray & StockDetail::saleFiveAmount() const
{
    return m_details.at(sale_5_amount);
}

const QByteArray & StockDetail::saleFivePrice() const
{
    return m_details.at(sale_5_price);
}

const QByteArray & StockDetail::date() const
{
    return m_details.at(stock_date);
}

const QByteArray & StockDetail::time() const
{
    return m_details.at(stock_time);
}

const QString StockDetail::rise() const
{
    const QByteArray & baCurrent = currentPrice();
    if(baCurrent != "-")
    {
        float yesterday = yesterdayClosePrice().toFloat();
        float current = baCurrent.toFloat();
        if(current < 0.001)
        {
            return "-";
        }
        float rise = current - yesterday;
        return QString("%1").arg(rise, 0, 'f', 2);
    }
    return "-";
}

const QString StockDetail::risePercent() const
{
    const QByteArray & baCurrent = currentPrice();
    if(baCurrent != "-")
    {
        float yesterday = yesterdayClosePrice().toFloat();
        float current = currentPrice().toFloat();
        if(current < 0.001)
        {
            return "-";
        }
        float risePercent = (current - yesterday)*100 / yesterday;
        return QString("%1%").arg(risePercent, 0, 'f', 2);
    }

    return "-";
}

const int StockDetail::riseMode() const
{
    const QByteArray & baCurrent = currentPrice();
    if(baCurrent != "-")
    {
        float yesterday = yesterdayClosePrice().toFloat();
        float current = baCurrent.toFloat();
        if(current < 0.001)
        {
            return 0;
        }
        float rise = current - yesterday;
        if(rise < -0.001) return -1;
        if(rise > 0.001) return 1;
    }
    return 0;
}

const bool StockDetail::arriveStopWin() const
{
    if(m_details.size() > 0 && m_stopWinPrice > 0.001)
    {
        double currentPrice = m_details.at(current_price).toDouble();
        if(currentPrice < 0.001)
        {
            return false;
        }
        return currentPrice >= m_stopWinPrice;
    }

    return false;
}

const bool StockDetail::arriveStopLose() const
{
    if(m_details.size() > 0 && m_stopLosePrice > 0.001)
    {
        double currentPrice = m_details.at(current_price).toDouble();
        if(currentPrice < 0.001)
        {
            return false;
        }
        return currentPrice <= m_stopLosePrice;
    }

    return false;
}

void StockDetail::setStopWinPrice(double price)
{
    m_stopWinPrice = price;
}

const double StockDetail::stopWinPrice() const
{
    return m_stopWinPrice;
}

void StockDetail::setStopLosePrice(double price)
{
    m_stopLosePrice = price;
}

const double StockDetail::stopLosePrice() const
{
    return m_stopLosePrice;
}

const bool StockDetail::isDetailsValid() const
{
    return currentPrice() != "-";
}

const QString StockDetail::handoverRate() const
{
    if(m_totalTransactStocks > 0)
    {
        const QByteArray & transact = m_details.at(transaction_stocks);
        if(transact != "-")
        {
            double dbRate = transact.toDouble()/m_totalTransactStocks;
            return QString("%1%").arg(dbRate, 0, 'f',2);
        }
    }
    return "-";
}

//
// class StockProvider
//
StockProvider::StockProvider(QObject *parent)
    : QObject(parent)
    , m_iRefreshTimer(0)
    , m_iRefreshTimeoutTimer(0)
    , m_nam(this)
    , m_iRemindMode(0)
    , m_refreshInterval(3000)
    , m_reply(0)
    , m_bShouldSave(false)
{
    readFromFile();
    if(m_stocks.size() > 0)
    {
        m_iRefreshTimer = startTimer(m_refreshInterval);
    }
}

StockProvider::~StockProvider()
{
    int count = m_stocks.count();
    if(count > 0)
    {
        saveToFile();
        for(int i = 0; i < count; i++)
        {
            delete m_stocks.at(i);
        }
        m_stocks.clear();
    }
}

bool StockProvider::addStock(const QByteArray &stockCode)
{
    QByteArray code(stockCode);
    if(code.startsWith('6'))
    {
        code.prepend("sh");
    }
    else
    {
        code.prepend("sz");
    }
    int count = m_stocks.count();
    int i = 0;
    for(; i < count; i++)
    {
        if( m_stocks.at(i)->code() == code )
        {
            break;
        }
    }

    if(i == count)
    {
        m_stocks.append(new StockDetail(code));
        saveToFile();
    }

    if(count == 0 && m_stocks.count() > 0)
    {
        startUpdate();
    }

    return i == count;
}

void StockProvider::removeStock(const QByteArray &stockCode)
{
    int count = m_stocks.count();
    int i = 0;
    for(; i < count; i++)
    {
        if( m_stocks.at(i)->code() == stockCode )
        {
            break;
        }
    }

    if(i < count)
    {
        delete m_stocks.takeAt(i);
        saveToFile();
    }
}

void StockProvider::removeStock(int index)
{
    if(index >=0 && index < m_stocks.size())
    {
        delete m_stocks.takeAt(index);
        saveToFile();
    }
}

StockDetail * StockProvider::stock(const QByteArray &stockCode)
{
    int count = m_stocks.count();
    int i = 0;
    StockDetail * detail = 0;
    for(; i < count; i++)
    {
        detail = m_stocks[i];
        if( detail->code() == stockCode )
        {
            return detail;
        }
    }

    return 0;
}

int StockProvider::stockCount()
{
    return m_stocks.count();
}

StockDetail * StockProvider::stockAt(int index)
{
    return m_stocks[index];
}

int StockProvider::remindMode()
{
    return m_iRemindMode;
}

void StockProvider::setRemindMode(int mode)
{
    m_iRemindMode = mode;
}

void StockProvider::saveState()
{
    saveToFile();
}

void StockProvider::setUpdateInterval(int seconds)
{
    m_refreshInterval = seconds;
    //TODO: update
}

void StockProvider::onRefreshError(QNetworkReply::NetworkError code)
{
    m_reply->disconnect(this);
    m_iRefreshTimer = startTimer(0);
}

void StockProvider::onRefreshFinished()
{
    m_reply->disconnect(this);
    if(m_reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "StockProvider::refreshFinished, error - " << m_reply->errorString();
        return;
    }
    else if(m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        qDebug() << "StockProvider::refreshFinished, but server return - " << m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        return;
    }
    //qDebug() << "onRefreshFinished";

    parseData(m_reply->readAll());
    m_reply->deleteLater();
    m_reply = 0;

    int elapsed = m_updateElapsed.elapsed();

    m_iRefreshTimer = startTimer(m_refreshInterval > elapsed ? m_refreshInterval - elapsed : 0);
}

void StockProvider::timerEvent(QTimerEvent *e)
{
    int id = e->timerId();
    if(id == m_iRefreshTimer)
    {
        killTimer(m_iRefreshTimer);
        m_iRefreshTimer = 0;
        if(m_iRefreshTimeoutTimer > 0)
        {
            killTimer(m_iRefreshTimeoutTimer);
            m_iRefreshTimeoutTimer = 0;
        }

        int count = m_stocks.size();

        if(count > 0)
        {
            startUpdate();
        }
    }
    else if(id == m_iRefreshTimeoutTimer)
    {
        killTimer(m_iRefreshTimeoutTimer);
        m_iRefreshTimeoutTimer = 0;

        if(m_iRefreshTimer > 0)
        {
            killTimer(m_iRefreshTimer);
            m_iRefreshTimer = 0;
        }
        if(m_reply)
        {
            m_reply->disconnect(this);
            m_reply->abort();
            m_reply->deleteLater();
            m_reply = 0;
        }
        if(m_stocks.size() > 0)
        {
            startUpdate();
        }
    }
}

void StockProvider::saveToFile()
{
    m_bShouldSave = false;
    int count = m_stocks.size();
    if(count == 0)
    {
        return;
    }

    QFile file("stocks");
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "open stocks for write failed";
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    out << "<stocks>\n";

    out << "  <remindMode>" << m_iRemindMode << "</remindMode>\n";

    StockDetail * detail = 0;
    for(int i = 0; i < count; i++)
    {
        detail = m_stocks.at(i);
        out << "  <stock>\n";
        out << "    <code>" << detail->code() << "</code>\n";

        QString strName = detail->name().data();
        if(strName != "-")
        {
            out << "    <name>" << strName << "</name>\n";
        }

        if(detail->m_stopWinPrice >= 0)
        {
            out << "    <stopwin>" << detail->m_stopWinPrice << "</stopwin>\n";
        }
        if(detail->m_stopLosePrice >= 0)
        {
            out << "    <stoplose>" << detail->m_stopLosePrice << "</stoplose>\n";
        }
        out << "  </stock>\n";
    }

    out << "</stocks>";
    file.close();
}

void StockProvider::readFromFile()
{
    QFile file("stocks");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open stocks for read failed";
        return;
    }

    QXmlStreamReader reader;
    reader.setDevice(&file);
    QStringRef elementName;
    StockDetail * stockinfo = 0;

    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            elementName = reader.name();
            if( elementName == "code")
            {
                stockinfo = new StockDetail(reader.readElementText().toLatin1());
            }
            else if( elementName == "name")
            {
                stockinfo->m_details[stock_name] = reader.readElementText().toUtf8();
            }
            else if( elementName == "stopwin")
            {
                stockinfo->setStopWinPrice(reader.readElementText().toDouble());
            }
            else if( elementName == "stoplose" )
            {
                stockinfo->setStopLosePrice(reader.readElementText().toDouble());
            }
            else if( elementName == "remindMode")
            {
                m_iRemindMode = reader.readElementText().toInt();
            }
        }
        else if(reader.isEndElement())
        {
            elementName = reader.name();
            if( elementName == "stock")
            {
                m_stocks.append(stockinfo);
                stockinfo = 0;
            }
        }
    }
    file.close();
}

void StockProvider::startUpdate()
{
    if(m_stocks.size() == 0 ) return;

    QString strUrl("http://hq.sinajs.cn/list=");
    int count = m_stocks.size();
    StockDetail * detail = 0;
    for(int i = 0; i < count; i++)
    {
        detail = m_stocks.at(i);
        strUrl.append(detail->code());
        strUrl.append(',');
    }
    if(strUrl.endsWith(','))
    {
        strUrl.chop(1);
    }

    //qDebug() << "start get - " << strUrl;
    QUrl qurl(strUrl);
    QNetworkRequest req(qurl);
    m_reply = m_nam.get(req);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onRefreshError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(onRefreshFinished()));

    m_iRefreshTimeoutTimer = startTimer(20000);
    m_updateElapsed.restart();
    emit beginRefresh();
}

void StockProvider::parseData(const QByteArray &data)
{
    QList<QByteArray> stocksData = data.split(';');
    int size = stocksData.size();
    if(size == 0) return;

    m_strRemind.clear();

    for(int i = 0; i < size; i++)
    {
        QByteArray  d = stocksData.at(i).trimmed();
        if(d.startsWith(STOCK_START_STRING))
        {
            int start = STOCK_START_STRING_LEN;
            int index = d.indexOf('=');
            if(index > 0)
            {
                QByteArray stockCode = d.mid(start, index - start);
                QByteArray perData = d.mid(index+2);
                if(perData.endsWith('\"'))
                {
                    perData.chop(1);
                }
                updateStockDetail(stockCode, perData);
            }
        }
    }
    if(m_bShouldSave) saveToFile();

    emit refreshed();

    if(m_strRemind.length() > 0) emit remind(m_strRemind);
}

void StockProvider::updateStockDetail(const QByteArray &stockCode, const QByteArray &data)
{
    if(data.length() < 64) return;

    int count = m_stocks.count();
    int i = 0;
    for(; i < count; i++)
    {
        if( m_stocks.at(i)->code() == stockCode )
        {
            break;
        }
    }

    if(i < count)
    {
        StockDetail * detail = m_stocks.at(i);
        QByteArray stockName = detail->m_details[stock_name];
        if(stockName == "-")
        {
            m_bShouldSave = true;
        }
        detail->m_details = data.split(',');
        detail->m_details[transaction_stocks].chop(2);
        double money = detail->m_details.at(transaction_money).toDouble() / 10000;
        detail->m_details[transaction_money] = QByteArray::number(money,'f',0);
        if(stockName != "-")
        {
            detail->m_details[stock_name] = stockName;
        }
        else
        {
            QTextCodec * gbkCodec = QTextCodec::codecForName("GBK");
            if(gbkCodec)
            {
                detail->m_details[stock_name] = gbkCodec->toUnicode(detail->m_details.at(stock_name).data()).toUtf8();
            }
        }
        updateRemindContents(detail);
    }
}

void StockProvider::updateRemindContents(StockDetail *stock)
{
    if(stock->arriveStopLose())
    {
        m_strRemind += QString("%1 : %2\n")
                .arg(stock->code().data())
                .arg(stock->currentPrice().data());
    }
    else if(stock->arriveStopWin())
    {
        m_strRemind += QString("%1: %2\n")
                .arg(stock->code().data())
                .arg(stock->currentPrice().data());
    }
}
