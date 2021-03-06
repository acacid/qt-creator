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

#ifndef DEBUGGER_DEBUGGERKITINFORMATION_H
#define DEBUGGER_DEBUGGERKITINFORMATION_H

#include "debugger_global.h"
#include "debuggerconstants.h"

#include <projectexplorer/kitinformation.h>

namespace Debugger {
class DebuggerItem;

class DEBUGGER_EXPORT DebuggerKitInformation : public ProjectExplorer::KitInformation
{
    Q_OBJECT

public:
    DebuggerKitInformation();

    QVariant defaultValue(ProjectExplorer::Kit *k) const override;

    QList<ProjectExplorer::Task> validate(const ProjectExplorer::Kit *k) const override
        { return DebuggerKitInformation::validateDebugger(k); }

    void setup(ProjectExplorer::Kit *k) override;
    void fix(ProjectExplorer::Kit *k) override;

    static const DebuggerItem *debugger(const ProjectExplorer::Kit *kit);

    static QList<ProjectExplorer::Task> validateDebugger(const ProjectExplorer::Kit *k);
    static bool isValidDebugger(const ProjectExplorer::Kit *k);

    ProjectExplorer::KitConfigWidget *createConfigWidget(ProjectExplorer::Kit *k) const override;
    void addToMacroExpander(ProjectExplorer::Kit *kit, Utils::MacroExpander *expander) const override;

    ItemList toUserOutput(const ProjectExplorer::Kit *k) const override;

    static void setDebugger(ProjectExplorer::Kit *k, const QVariant &id);

    static Core::Id id();
    static Utils::FileName debuggerCommand(const ProjectExplorer::Kit *k);
    static DebuggerEngineType engineType(const ProjectExplorer::Kit *k);
    static QString displayString(const ProjectExplorer::Kit *k);
};

} // namespace Debugger

#endif // DEBUGGER_DEBUGGERKITINFORMATION_H
