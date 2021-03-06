/****************************************************************************
**
** Copyright (C) 2016 Petar Perisin.
** Contact: petar.perisin@gmail.com
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

#ifndef GERRITPUSHDIALOG_H
#define GERRITPUSHDIALOG_H

#include <QDialog>
#include <QMultiMap>
#include <QDate>

namespace Git {
namespace Internal { class GitClient; }
}

namespace Gerrit {
namespace Internal {

namespace Ui { class GerritPushDialog; }

class GerritPushDialog : public QDialog
{
    Q_OBJECT

public:
    GerritPushDialog(const QString &workingDir, const QString &reviewerList, QWidget *parent);
    ~GerritPushDialog();

    QString selectedCommit() const;
    QString selectedRemoteName() const;
    QString selectedRemoteBranchName() const;
    QString selectedPushType() const;
    QString selectedTopic() const;
    QString reviewers() const;
    bool isValid() const;

private slots:
    void setChangeRange();
    void setRemoteBranches(bool includeOld = false);
    void updateCommits(int index);

private:
    typedef QPair<QString, QDate> BranchDate;
    typedef QMultiMap<QString, BranchDate> RemoteBranchesMap;

    QString determineRemoteBranch(const QString &localBranch);
    void initRemoteBranches();
    QString calculateChangeRange(const QString &branch);
    QString m_workingDir;
    QString m_suggestedRemoteBranch;
    Ui::GerritPushDialog *m_ui;
    RemoteBranchesMap m_remoteBranches;
    bool m_isValid;
    Git::Internal::GitClient *m_client;
};


} // namespace Internal
} // namespace Gerrit

#endif // GERRITPUSHDIALOG_H
