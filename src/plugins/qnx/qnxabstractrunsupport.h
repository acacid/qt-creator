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

#ifndef QNXABSTRACTRUNSUPPORT_H
#define QNXABSTRACTRUNSUPPORT_H

#include <projectexplorer/devicesupport/idevice.h>
#include <utils/environment.h>
#include <utils/portlist.h>

#include <QObject>
#include <QString>

namespace ProjectExplorer {
class DeviceApplicationRunner;
class DeviceUsedPortsGatherer;
}

namespace Qnx {
namespace Internal {

class QnxRunConfiguration;

class QnxAbstractRunSupport : public QObject
{
    Q_OBJECT
protected:
    enum State {
        Inactive,
        GatheringPorts,
        StartingRemoteProcess,
        Running
    };
public:
    QnxAbstractRunSupport(QnxRunConfiguration *runConfig, QObject *parent = 0);

protected:
    bool setPort(int &port);
    virtual void startExecution() = 0;

    virtual QString executable() const;
    Utils::Environment environment() const { return m_environment; }
    QString workingDirectory() const { return m_workingDir; }

    void setFinished();

    State state() const;
    void setState(State state);

    ProjectExplorer::DeviceApplicationRunner *appRunner() const;
    const ProjectExplorer::IDevice::ConstPtr device() const;

protected slots:
    virtual void handleAdapterSetupRequested();

    virtual void handleRemoteProcessStarted();
    virtual void handleRemoteProcessFinished(bool);
    virtual void handleProgressReport(const QString &progressOutput);
    virtual void handleRemoteOutput(const QByteArray &output);
    virtual void handleError(const QString &);

private slots:
    void handlePortListReady();

private:
    ProjectExplorer::DeviceUsedPortsGatherer * m_portsGatherer;
    Utils::PortList m_portList;
    const QString m_remoteExecutable;
    ProjectExplorer::IDevice::ConstPtr m_device;
    ProjectExplorer::DeviceApplicationRunner *m_runner;
    State m_state;
    Utils::Environment m_environment;
    QString m_workingDir;
};

} // namespace Internal
} // namespace Qnx

#endif // QNXABSTRACTRUNSUPPORT_H
