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

#ifndef WINCEQTVERSION_H
#define WINCEQTVERSION_H

#include "baseqtversion.h"

namespace QtSupport {
namespace Internal {

class WinCeQtVersion : public BaseQtVersion
{
public:
    WinCeQtVersion() = default;
    WinCeQtVersion(const Utils::FileName &path, const QString &archType,
                   bool isAutodetected = false, const QString &autodetectionSource = QString());
    ~WinCeQtVersion() = default;
    WinCeQtVersion *clone() const;

    QString type() const;

    QList<ProjectExplorer::Abi> detectQtAbis() const;

    QString description() const;

    void fromMap(const QVariantMap &data);

    QSet<Core::Id> targetDeviceTypes() const;

private:
    ProjectExplorer::Abi::Architecture m_archType = ProjectExplorer::Abi::ArmArchitecture;
};

} // Internal
} // QtSupport

#endif // WINCEQTVERSION_H
