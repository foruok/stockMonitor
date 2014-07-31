#include "stock.h"
#include "stockProvider.h"

StockState::StockState()
    : m_detail(0), m_provider(0)
    , m_indexInProvider(0)
{}
StockState::StockState(const StockState &rhs)
    : m_detail(rhs.m_detail),
      m_provider(rhs.m_provider),
      m_indexInProvider(rhs.m_indexInProvider)
{
}

StockState::~StockState()
{}

QString StockState::loss()
{
    return QString::number(m_detail->stopLosePrice(), 'g', 2);
}

void StockState::setLoss(QString loss)
{
    m_detail->setStopLosePrice(loss.toDouble());
}

QString StockState::gain()
{
    return QString::number(m_detail->stopWinPrice(), 'g', 2);
}

void StockState::setGain(QString gain)
{
    m_detail->setStopWinPrice(gain.toDouble());
}

QString StockState::code()
{
    return m_detail->code();
}

QString StockState::name()
{
    return m_detail->name();
}

QString StockState::price()
{
    return m_detail->currentPrice();
}

int StockState::index()
{
    return m_indexInProvider;
}

void StockState::setIndex(int index)
{
    m_indexInProvider = index;
    m_detail = m_provider->stockAt(index);
}
