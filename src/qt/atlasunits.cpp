// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Atlas Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "atlasunits.h"

#include "primitives/transaction.h"

#include <QStringList>

AtlasUnits::AtlasUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<AtlasUnits::Unit> AtlasUnits::availableUnits()
{
    QList<AtlasUnits::Unit> unitlist;
    unitlist.append(ATL);
    unitlist.append(mATL);
    unitlist.append(uATL);
    return unitlist;
}

bool AtlasUnits::valid(int unit)
{
    switch(unit)
    {
    case ATL:
    case mATL:
    case uATL:
        return true;
    default:
        return false;
    }
}

QString AtlasUnits::name(int unit)
{
    switch(unit)
    {
    case ATL: return QString("ATL");
    case mATL: return QString("mATL");
    case uATL: return QString::fromUtf8("μATL");
    default: return QString("???");
    }
}

QString AtlasUnits::description(int unit)
{
    switch(unit)
    {
    case ATL: return QString("Atlass");
    case mATL: return QString("Milli-Atlass (1 / 1" THIN_SP_UTF8 "000)");
    case uATL: return QString("Micro-Atlass (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 AtlasUnits::factor(int unit)
{
    switch(unit)
    {
    case ATL:  return 100000000;
    case mATL: return 100000;
    case uATL: return 100;
    default:   return 100000000;
    }
}

int AtlasUnits::decimals(int unit)
{
    switch(unit)
    {
    case ATL: return 8;
    case mATL: return 5;
    case uATL: return 2;
    default: return 0;
    }
}

QString AtlasUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators, const int nAssetUnit)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(nAssetUnit < 0 && !valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = nAssetUnit >= 0 ? nAssetUnit : decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (nAssetUnit == MIN_ASSET_UNITS)
        return quotient_str;

    return quotient_str + QString(".") + remainder_str;
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString AtlasUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString AtlasUnits::formatWithCustomName(QString customName, const CAmount& amount, int unit, bool plussign, SeparatorStyle separators)
{
    return format(ATL, amount, plussign, separators, unit) + QString(" ") + customName;
}

QString AtlasUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool AtlasUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString AtlasUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (AtlasUnits::valid(unit))
    {
        amountTitle += " ("+AtlasUnits::name(unit) + ")";
    }
    return amountTitle;
}

int AtlasUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant AtlasUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount AtlasUnits::maxMoney()
{
    return MAX_MONEY;
}
