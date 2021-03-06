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

#include "projectpartcontainer.h"

#include "clangbackendipcdebugutils.h"

#include <QDataStream>
#include <QDebug>

#include <ostream>

namespace ClangBackEnd {

ProjectPartContainer::ProjectPartContainer(const Utf8String &projectPathId,
                                           const Utf8StringVector &arguments)
    : projectPartId_(projectPathId),
      arguments_(arguments)
{
}

const Utf8String &ProjectPartContainer::projectPartId() const
{
    return projectPartId_;
}

const Utf8StringVector &ProjectPartContainer::arguments() const
{
    return arguments_;
}


QDataStream &operator<<(QDataStream &out, const ProjectPartContainer &container)
{
    out << container.projectPartId_;
    out << container.arguments_;

    return out;
}

QDataStream &operator>>(QDataStream &in, ProjectPartContainer &container)
{
    in >> container.projectPartId_;
    in >> container.arguments_;

    return in;
}

bool operator==(const ProjectPartContainer &first, const ProjectPartContainer &second)
{
    return first.projectPartId_ == second.projectPartId_;
}

bool operator<(const ProjectPartContainer &first, const ProjectPartContainer &second)
{
    return first.projectPartId_ < second.projectPartId_;
}

static Utf8String quotedArguments(const Utf8StringVector &arguments)
{
    const Utf8String quote = Utf8String::fromUtf8("\"");
    const Utf8String joined = arguments.join(quote + Utf8String::fromUtf8(" ") + quote);
    return quote + joined + quote;
}

QDebug operator<<(QDebug debug, const ProjectPartContainer &container)
{
    const Utf8String arguments = quotedArguments(container.arguments());
    const Utf8String fileWithArguments = debugWriteFileForInspection(
                                            arguments,
                                            Utf8StringLiteral("projectpartargs-"));

    debug.nospace() << "ProjectPartContainer("
                    << container.projectPartId()
                    << ","
                    << "<" << fileWithArguments << ">"
                    << ")";

    return debug;
}

void PrintTo(const ProjectPartContainer &container, ::std::ostream* os)
{
    *os << "ProjectPartContainer("
        << container.projectPartId().constData()
        << ","
        << container.arguments().constData()
        << ")";
}


} // namespace ClangBackEnd

