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

#ifndef GITUTILS_H
#define GITUTILS_H

#include <QString>

QT_BEGIN_NAMESPACE
class QDebug;
class QWidget;
QT_END_NAMESPACE

namespace Git {
namespace Internal {

class Stash {
public:
    void clear();
    bool parseStashLine(const QString &l);

    QString name;
    QString branch;
    QString message;
};

QDebug operator<<(QDebug d, const Stash &);

// Make QInputDialog  play nicely
bool inputText(QWidget *parent, const QString &title, const QString &prompt, QString *s);

// Version information following Qt convention
inline unsigned version(unsigned major, unsigned minor, unsigned patch)
{
    return (major << 16) + (minor << 8) + patch;
}

QString versionString(unsigned ver);

} // namespace Internal
} // namespace Git

#endif // GITUTILS_H
