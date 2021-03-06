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

#ifndef CLANGCODEMODEL_INTERNAL_CLANGASSISTPROPOSALITEM_H
#define CLANGCODEMODEL_INTERNAL_CLANGASSISTPROPOSALITEM_H

#include <codecompletion.h>

#include <texteditor/codeassist/assistproposalitem.h>

namespace ClangCodeModel {
namespace Internal {

class ClangAssistProposalItem : public TextEditor::AssistProposalItem
{
    friend bool operator<(const ClangAssistProposalItem &first, const ClangAssistProposalItem &second);
public:
    ClangAssistProposalItem() {}

    bool prematurelyApplies(const QChar &c) const override;
    void applyContextualContent(TextEditor::TextEditorWidget *editorWidget, int basePosition) const override;

    void keepCompletionOperator(unsigned compOp);

    bool isOverloaded() const;
    void addOverload(const ClangBackEnd::CodeCompletion &ccr);

    void setCodeCompletion(const ClangBackEnd::CodeCompletion &codeCompletion);
    const ClangBackEnd::CodeCompletion &codeCompletion() const;

private:
    ClangBackEnd::CodeCompletion m_codeCompletion;
    QList<ClangBackEnd::CodeCompletion> m_overloads;
    unsigned m_completionOperator;
    mutable QChar m_typedChar;
};

} // namespace Internal
} // namespace ClangCodeModel

#endif // CLANGCODEMODEL_INTERNAL_CLANGASSISTPROPOSALITEM_H
