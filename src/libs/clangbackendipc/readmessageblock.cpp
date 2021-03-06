/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "readmessageblock.h"

#include <QDataStream>
#include <QDebug>
#include <QIODevice>
#include <QVariant>

namespace ClangBackEnd {

ReadMessageBlock::ReadMessageBlock(QIODevice *ioDevice)
    : ioDevice(ioDevice),
      messageCounter(0),
      blockSize(0)
{
}

void ReadMessageBlock::checkIfMessageIsLost(QDataStream &in)
{
    qint64 currentMessageCounter;

    in >> currentMessageCounter;

#ifndef DONT_CHECK_MESSAGE_COUNTER
    bool messageLost = !((currentMessageCounter == 0 && messageCounter == 0) || (messageCounter + 1 == currentMessageCounter));
    if (messageLost)
        qWarning() << "client message lost: " << messageCounter <<  currentMessageCounter;
#endif

    messageCounter = currentMessageCounter;
}

QVariant ReadMessageBlock::read()
{
    QDataStream in(ioDevice);

    QVariant message;

    if (isTheWholeMessageReadable(in)) {
        checkIfMessageIsLost(in);
        in >> message;
    }

    return message;
}

QVector<QVariant> ReadMessageBlock::readAll()
{
    QVector<QVariant> messages;

    while (true) {
        const QVariant message = read();
        if (message.isValid())
            messages.append(message);
        else
            return messages;
    }

    Q_UNREACHABLE();
}

void ReadMessageBlock::resetCounter()
{
    messageCounter = 0;
}

bool ReadMessageBlock::isTheWholeMessageReadable(QDataStream &in)
{
    if (ioDevice->bytesAvailable() == 0)
        return false;

    if (blockSize == 0)
        in >> blockSize;

    if (ioDevice->bytesAvailable() < blockSize)
        return false;

    blockSize = 0;

    return true;
}

} // namespace ClangBackEnd

