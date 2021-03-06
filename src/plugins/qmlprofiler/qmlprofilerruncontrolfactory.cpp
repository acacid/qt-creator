/****************************************************************************
**
** Copyright (C) 2016 Kläralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#include "qmlprofilerruncontrolfactory.h"
#include "localqmlprofilerrunner.h"
#include "qmlprofilerruncontrol.h"
#include "qmlprofilerrunconfigurationaspect.h"

#include <analyzerbase/ianalyzertool.h>
#include <analyzerbase/analyzerruncontrol.h>
#include <analyzerbase/analyzermanager.h>

#include <debugger/debuggerrunconfigurationaspect.h>

#include <projectexplorer/environmentaspect.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/localapplicationrunconfiguration.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtkitinformation.h>

#include <utils/qtcassert.h>

using namespace Analyzer;
using namespace ProjectExplorer;

namespace QmlProfiler {
namespace Internal {

QmlProfilerRunControlFactory::QmlProfilerRunControlFactory(QObject *parent) :
    IRunControlFactory(parent)
{
}

bool QmlProfilerRunControlFactory::canRun(RunConfiguration *runConfiguration, Core::Id mode) const
{
    return mode == ProjectExplorer::Constants::QML_PROFILER_RUN_MODE
            && (qobject_cast<LocalApplicationRunConfiguration *>(runConfiguration));
}

RunControl *QmlProfilerRunControlFactory::create(RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage)
{
    QTC_ASSERT(canRun(runConfiguration, mode), return 0);

    Kit *kit = runConfiguration->target()->kit();
    // FIXME: This is only used to communicate the connParams settings.
    auto localRunConfiguration = qobject_cast<LocalApplicationRunConfiguration *>(runConfiguration);
    QTC_ASSERT(localRunConfiguration, return 0);
    AnalyzerRunnable runnable;
    runnable.debuggee = localRunConfiguration->executable();
    runnable.debuggeeArgs = localRunConfiguration->commandLineArguments();

    AnalyzerConnection connection;
    const QtSupport::BaseQtVersion *version = QtSupport::QtKitInformation::qtVersion(kit);
    if (version) {
        QtSupport::QtVersionNumber versionNumber = version->qtVersion();
        if (versionNumber.majorVersion >= 5 && versionNumber.minorVersion >= 6)
            connection.analyzerSocket = LocalQmlProfilerRunner::findFreeSocket();
        else
            connection.analyzerPort = LocalQmlProfilerRunner::findFreePort(connection.analyzerHost);
    } else {
        qWarning() << "Running QML profiler on Kit without Qt version??";
        connection.analyzerPort = LocalQmlProfilerRunner::findFreePort(connection.analyzerHost);
    }

    // only desktop device is supported
    const IDevice::ConstPtr device = DeviceKitInformation::device(kit);
    QTC_ASSERT(device->type() == ProjectExplorer::Constants::DESKTOP_DEVICE_TYPE, return 0);

    auto runControl = qobject_cast<QmlProfilerRunControl *>
             (AnalyzerManager::createRunControl(runConfiguration, mode));
    QTC_ASSERT(runControl, return 0);

    runControl->setRunnable(runnable);
    runControl->setConnection(connection);

    LocalQmlProfilerRunner::Configuration conf;
    conf.executable = runnable.debuggee;
    conf.executableArguments = runnable.debuggeeArgs;
    conf.workingDirectory = runControl->workingDirectory();
    conf.socket = connection.analyzerSocket;
    if (EnvironmentAspect *environment = runConfiguration->extraAspect<EnvironmentAspect>())
        conf.environment = environment->environment();
    conf.port = connection.analyzerPort;

    if (conf.executable.isEmpty()) {
        if (errorMessage)
            *errorMessage = tr("No executable file to launch.");
        return 0;
    }

    (void) new LocalQmlProfilerRunner(conf, runControl);
    return runControl;
}

ProjectExplorer::IRunConfigurationAspect *
QmlProfilerRunControlFactory::createRunConfigurationAspect(ProjectExplorer::RunConfiguration *rc)
{
    return new QmlProfilerRunConfigurationAspect(rc);
}

} // namespace Internal
} // namespace QmlProfiler
