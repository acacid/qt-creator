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

#ifndef BUILDENVIRONMENTWIDGET_H
#define BUILDENVIRONMENTWIDGET_H

#include <projectexplorer/namedwidget.h>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

namespace ProjectExplorer {

class EnvironmentWidget;
class BuildConfiguration;

class PROJECTEXPLORER_EXPORT BuildEnvironmentWidget : public NamedWidget
{
    Q_OBJECT

public:
    BuildEnvironmentWidget(BuildConfiguration *bc);

private slots:
    void environmentModelUserChangesChanged();
    void clearSystemEnvironmentCheckBoxClicked(bool checked);
    void environmentChanged();

private:
    EnvironmentWidget *m_buildEnvironmentWidget;
    QCheckBox *m_clearSystemEnvironmentCheckBox;
    BuildConfiguration *m_buildConfiguration;
};

} // namespace ProjectExplorer

#endif // BUILDENVIRONMENTWIDGET_H
