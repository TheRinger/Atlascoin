// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017 The Atlas Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ATLAS_QT_ATLASADDRESSVALIDATOR_H
#define ATLAS_QT_ATLASADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class AtlasAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AtlasAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Atlas address widget validator, checks for a valid atlas address.
 */
class AtlasAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AtlasAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // ATLAS_QT_ATLASADDRESSVALIDATOR_H
