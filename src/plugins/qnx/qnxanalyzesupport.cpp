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

#include "qnxanalyzesupport.h"

#include "qnxdeviceconfiguration.h"
#include "qnxrunconfiguration.h"
#include "slog2inforunner.h"

#include <analyzerbase/analyzerruncontrol.h>
#include <projectexplorer/devicesupport/deviceapplicationrunner.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>

#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>
#include <qmldebug/qmldebugcommandlinearguments.h>

using namespace ProjectExplorer;

using namespace Qnx;
using namespace Qnx::Internal;

QnxAnalyzeSupport::QnxAnalyzeSupport(QnxRunConfiguration *runConfig,
                                     Analyzer::AnalyzerRunControl *runControl)
    : QnxAbstractRunSupport(runConfig, runControl)
    , m_runControl(runControl)
    , m_qmlPort(-1)
{
    const DeviceApplicationRunner *runner = appRunner();
    connect(runner, SIGNAL(reportError(QString)), SLOT(handleError(QString)));
    connect(runner, SIGNAL(remoteProcessStarted()), SLOT(handleRemoteProcessStarted()));
    connect(runner, SIGNAL(finished(bool)), SLOT(handleRemoteProcessFinished(bool)));
    connect(runner, SIGNAL(reportProgress(QString)), SLOT(handleProgressReport(QString)));
    connect(runner, SIGNAL(remoteStdout(QByteArray)), SLOT(handleRemoteOutput(QByteArray)));
    connect(runner, SIGNAL(remoteStderr(QByteArray)), SLOT(handleRemoteOutput(QByteArray)));

    connect(m_runControl, SIGNAL(starting(const Analyzer::AnalyzerRunControl*)),
            SLOT(handleAdapterSetupRequested()));
    connect(&m_outputParser, SIGNAL(waitingForConnectionOnPort(quint16)),
            SLOT(remoteIsRunning()));

    IDevice::ConstPtr dev = DeviceKitInformation::device(runConfig->target()->kit());
    QnxDeviceConfiguration::ConstPtr qnxDevice = dev.dynamicCast<const QnxDeviceConfiguration>();

    const QString applicationId = Utils::FileName::fromString(runConfig->remoteExecutableFilePath()).fileName();
    m_slog2Info = new Slog2InfoRunner(applicationId, qnxDevice, this);
    connect(m_slog2Info, SIGNAL(output(QString,Utils::OutputFormat)), this, SLOT(showMessage(QString,Utils::OutputFormat)));
    connect(runner, SIGNAL(remoteProcessStarted()), m_slog2Info, SLOT(start()));
    if (qnxDevice->qnxVersion() > 0x060500)
        connect(m_slog2Info, SIGNAL(commandMissing()), this, SLOT(printMissingWarning()));
}

void QnxAnalyzeSupport::handleAdapterSetupRequested()
{
    QTC_ASSERT(state() == Inactive, return);

    showMessage(tr("Preparing remote side...") + QLatin1Char('\n'), Utils::NormalMessageFormat);
    QnxAbstractRunSupport::handleAdapterSetupRequested();
}

void QnxAnalyzeSupport::startExecution()
{
    if (state() == Inactive)
        return;

    if (!setPort(m_qmlPort) && m_qmlPort == -1)
        return;

    setState(StartingRemoteProcess);

    const QStringList args = QStringList()
            << Utils::QtcProcess::splitArgs(m_runControl->runnable().debuggeeArgs)
            << QmlDebug::qmlDebugTcpArguments(QmlDebug::QmlProfilerServices, m_qmlPort);

    appRunner()->setEnvironment(environment());
    appRunner()->setWorkingDirectory(workingDirectory());
    appRunner()->start(device(), executable(), args);
}

void QnxAnalyzeSupport::handleRemoteProcessFinished(bool success)
{
    if (!m_runControl)
        return;

    if (!success)
        showMessage(tr("The %1 process closed unexpectedly.").arg(executable()),
                    Utils::NormalMessageFormat);
    m_runControl->notifyRemoteFinished();

    m_slog2Info->stop();
}

void QnxAnalyzeSupport::handleProfilingFinished()
{
    setFinished();
}

void QnxAnalyzeSupport::handleProgressReport(const QString &progressOutput)
{
    showMessage(progressOutput + QLatin1Char('\n'), Utils::NormalMessageFormat);
}

void QnxAnalyzeSupport::handleRemoteOutput(const QByteArray &output)
{
    QTC_ASSERT(state() == Inactive || state() == Running, return);

    showMessage(QString::fromUtf8(output), Utils::StdOutFormat);
}

void QnxAnalyzeSupport::handleError(const QString &error)
{
    if (state() == Running) {
        showMessage(error, Utils::ErrorMessageFormat);
    } else if (state() != Inactive) {
        showMessage(tr("Initial setup failed: %1").arg(error), Utils::NormalMessageFormat);
        setFinished();
    }
}

void QnxAnalyzeSupport::remoteIsRunning()
{
    if (m_runControl)
        m_runControl->notifyRemoteSetupDone(m_qmlPort);
}

void QnxAnalyzeSupport::showMessage(const QString &msg, Utils::OutputFormat format)
{
    if (state() != Inactive && m_runControl)
        m_runControl->logApplicationMessage(msg, format);
    m_outputParser.processOutput(msg);
}

void QnxAnalyzeSupport::printMissingWarning()
{
    showMessage(tr("Warning: \"slog2info\" is not found on the device, debug output not available."), Utils::ErrorMessageFormat);
}
