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

#include "semantichighlighter.h"

#include "syntaxhighlighter.h"

#include <utils/qtcassert.h>

#include <QTextDocument>
#include <QTextBlock>

using namespace TextEditor;
using namespace TextEditor::SemanticHighlighter;

static QColor blendColors(const QColor& x, const QColor& y, float k)
{
    float r = x.redF() + (y.redF() - x.redF()) * k;
    float g = x.greenF() + (y.greenF() - x.greenF()) * k;
    float b = x.blueF() + (y.blueF() - x.blueF()) * k;
    return QColor(r*255, g*255, b*255);
}

RainbowHash::RainbowHash()
{
    rbPower = 0.40;
    nSteps = 10;
}

QTextCharFormat SemanticHighlighter::RainbowHash::generate(unsigned h, const QTextCharFormat& f)
{
    auto i = hash.find(h);
    if(i != hash.end())
        return *i;

    float cStep = float(hash.size() % nSteps) / nSteps;
    int prevStep = cStep * colors.size();
    if(prevStep >= colors.size())
        prevStep = 0;

    int nextStep = prevStep + 1;
    if(nextStep >= colors.size())
        nextStep = 0;

    float stepRel = cStep - (float(prevStep)/colors.size());
    float k = stepRel * colors.size();
    if(k < 0) k = 0;
    if(k > 1) k = 1;
    QColor newc = blendColors(colors[prevStep], colors[nextStep], k);

    QTextCharFormat f2 = f;
    QColor fgc = f.foreground().color();
    f2.setForeground(QBrush(blendColors(fgc, newc, rbPower)));
    hash.insert(h, f2);
    return f2;
}

void SemanticHighlighter::incrementalApplyExtraAdditionalFormats(SyntaxHighlighter *highlighter,
        const QFuture<HighlightingResult> &future,
        int from, int to,
        const QHash<int, QTextCharFormat> &kindToFormat,
        RainbowHash &rainbowHash)
{
    if (to <= from)
        return;

    const int firstResultBlockNumber = future.resultAt(from).line - 1;

    // blocks between currentBlockNumber and the last block with results will
    // be cleaned of additional extra formats if they have no results
    int currentBlockNumber = 0;
    for (int i = from - 1; i >= 0; --i) {
        const HighlightingResult &result = future.resultAt(i);
        const int blockNumber = result.line - 1;
        if (blockNumber < firstResultBlockNumber) {
            // stop! found where last format stopped
            currentBlockNumber = blockNumber + 1;
            // add previous results for the same line to avoid undoing their formats
            from = i + 1;
            break;
        }
    }

    QTextDocument *doc = highlighter->document();
    QTC_ASSERT(currentBlockNumber < doc->blockCount(), return);
    QTextBlock b = doc->findBlockByNumber(currentBlockNumber);

    HighlightingResult result = future.resultAt(from);
    for (int i = from; i < to && b.isValid(); ) {
        const int blockNumber = result.line - 1;
        QTC_ASSERT(blockNumber < doc->blockCount(), return);

        // clear formats of blocks until blockNumber
        while (currentBlockNumber < blockNumber) {
            QList<QTextLayout::FormatRange> noFormats;
            highlighter->setExtraAdditionalFormats(b, noFormats);
            b = b.next();
            ++currentBlockNumber;
        }

        // collect all the formats for the current line
        QList<QTextLayout::FormatRange> formats;
        formats.reserve(to - from);
        forever {
            QTextLayout::FormatRange formatRange;

            formatRange.format = kindToFormat.value(result.kind);
            if (formatRange.format.isValid()) {
                if(rainbowHash.kinds.contains(result.kind))
                    formatRange.format = rainbowHash.generate(result.hash, formatRange.format);

                formatRange.start = result.column - 1;
                formatRange.length = result.length;
                formats.append(formatRange);
            }

            ++i;
            if (i >= to)
                break;
            result = future.resultAt(i);
            const int nextBlockNumber = result.line - 1;
            if (nextBlockNumber != blockNumber)
                break;
        }
        highlighter->setExtraAdditionalFormats(b, formats);
        b = b.next();
        ++currentBlockNumber;
    }
}

void SemanticHighlighter::clearExtraAdditionalFormatsUntilEnd(
        SyntaxHighlighter *highlighter,
        const QFuture<HighlightingResult> &future)
{
    // find block number of last result
    int lastBlockNumber = 0;
    for (int i = future.resultCount() - 1; i >= 0; --i) {
        const HighlightingResult &result = future.resultAt(i);
        if (result.line) {
            lastBlockNumber = result.line - 1;
            break;
        }
    }

    QTextDocument *doc = highlighter->document();

    const int firstBlockToClear = lastBlockNumber + 1;
    if (firstBlockToClear == doc->blockCount())
        return;
    QTC_ASSERT(firstBlockToClear < doc->blockCount(), return);

    QTextBlock b = doc->findBlockByNumber(firstBlockToClear);

    while (b.isValid()) {
        QList<QTextLayout::FormatRange> noFormats;
        highlighter->setExtraAdditionalFormats(b, noFormats);
        b = b.next();
    }
}
