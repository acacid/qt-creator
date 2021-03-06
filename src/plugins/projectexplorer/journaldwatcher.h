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

#ifndef JOURNALDWATCHER_H
#define JOURNALDWATCHER_H

#include <QByteArray>
#include <QMap>
#include <QObject>

#include <functional>

namespace ProjectExplorer {

class ProjectExplorerPlugin;

class JournaldWatcher : public QObject
{
    Q_OBJECT

public:
    typedef QMap<QByteArray, QByteArray> LogEntry;
    typedef std::function<void(LogEntry)> Subscription;

    ~JournaldWatcher();

    static JournaldWatcher *instance();

    static const QByteArray &machineId();

    static bool subscribe(QObject *subscriber, const Subscription &subscription);
    static void unsubscribe(QObject *subscriber);

signals:
    void journaldOutput(quint64 pid, const QString &message);

private:
    JournaldWatcher();

    void handleEntry();

    static JournaldWatcher *m_instance;

    friend class ProjectExplorerPlugin;
};

} // namespace ProjectExplorer

#endif // JOURNALDWATCHER_H
