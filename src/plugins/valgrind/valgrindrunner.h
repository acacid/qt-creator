/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
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

#ifndef VALGRIND_RUNNER_H
#define VALGRIND_RUNNER_H

#include <analyzerbase/analyzerconstants.h>
#include <projectexplorer/applicationlauncher.h>

#include <utils/outputformat.h>
#include <ssh/sshconnection.h>

#include <QProcess>

namespace Utils {
class Environment;
class SshConnectionParameters;
}

namespace Valgrind {

class ValgrindProcess;

class ValgrindRunner : public QObject
{
    Q_OBJECT

public:
    explicit ValgrindRunner(QObject *parent = 0);
    ~ValgrindRunner();

    QString valgrindExecutable() const;
    void setValgrindExecutable(const QString &executable);
    QStringList valgrindArguments() const;
    QStringList fullValgrindArguments() const;
    void setValgrindArguments(const QStringList &toolArguments);
    QString debuggeeExecutable() const;
    void setDebuggeeExecutable(const QString &executable);
    QString debuggeeArguments() const;
    void setDebuggeeArguments(const QString &arguments);

    void setWorkingDirectory(const QString &path);
    QString workingDirectory() const;
    void setEnvironment(const Utils::Environment &environment);
    void setProcessChannelMode(QProcess::ProcessChannelMode mode);

    void setUseStartupProject(bool useStartupProject);
    bool useStartupProject() const;

    void setLocalRunMode(ProjectExplorer::ApplicationLauncher::Mode localRunMode);

    void setConnectionParameters(const QSsh::SshConnectionParameters &connParams);
    const QSsh::SshConnectionParameters &connectionParameters() const;

    void waitForFinished() const;

    QString errorString() const;

    virtual bool start();
    virtual void stop();

    ValgrindProcess *valgrindProcess() const;

protected:
    virtual QString tool() const = 0;

signals:
    void processOutputReceived(const QString &, Utils::OutputFormat);
    void processErrorReceived(const QString &, QProcess::ProcessError);
    void started();
    void finished();

protected slots:
    virtual void processError(QProcess::ProcessError);
    virtual void processFinished(int, QProcess::ExitStatus);
    virtual void localHostAddressRetrieved(const QHostAddress &localHostAddress);

private:
    class Private;
    Private *d;
};

} // namespace Valgrind

#endif // VALGRIND_RUNNER_H
