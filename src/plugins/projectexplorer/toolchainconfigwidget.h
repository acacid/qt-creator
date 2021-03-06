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

#ifndef TOOLCHAINCONFIGWIDGET_H
#define TOOLCHAINCONFIGWIDGET_H

#include "projectexplorer_export.h"

#include <QScrollArea>

QT_BEGIN_NAMESPACE
class QFormLayout;
class QGridLayout;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE

namespace ProjectExplorer {

class ToolChain;

// --------------------------------------------------------------------------
// ToolChainConfigWidget
// --------------------------------------------------------------------------

class PROJECTEXPLORER_EXPORT ToolChainConfigWidget : public QScrollArea
{
    Q_OBJECT

public:
    ToolChainConfigWidget(ToolChain *);

    ToolChain *toolChain() const;

    void apply();
    void discard();
    bool isDirty() const;
    void makeReadOnly();

signals:
    void dirty();

protected slots:
    void setErrorMessage(const QString &);
    void clearErrorMessage();

protected:
    virtual void applyImpl() = 0;
    virtual void discardImpl() = 0;
    virtual bool isDirtyImpl() const = 0;
    virtual void makeReadOnlyImpl() = 0;

    void addErrorLabel();
    QFormLayout *m_mainLayout;
    QLineEdit *m_nameLineEdit;

private:
    ToolChain *m_toolChain;
    QLabel *m_errorLabel;
};

} // namespace ProjectExplorer

#endif // TOOLCHAINCONFIGWIDGET_H
