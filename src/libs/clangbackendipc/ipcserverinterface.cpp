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

#include "ipcserverinterface.h"

#include "cmbcompletecodemessage.h"
#include "cmbregisterprojectsforeditormessage.h"
#include "cmbregistertranslationunitsforeditormessage.h"
#include "cmbunregisterprojectsforeditormessage.h"
#include "cmbunregistertranslationunitsforeditormessage.h"
#include "registerunsavedfilesforeditormessage.h"
#include "requestdiagnosticsmessage.h"
#include "requesthighlightingmessage.h"
#include "unregisterunsavedfilesforeditormessage.h"
#include "updatetranslationunitsforeditormessage.h"
#include "updatevisibletranslationunitsmessage.h"

#include <QDebug>
#include <QVariant>

namespace ClangBackEnd {

void IpcServerInterface::dispatch(const QVariant &message)
{
    static const int endMessageType = QMetaType::type("ClangBackEnd::EndMessage");
    static const int registerTranslationUnitsForEditorMessageType = QMetaType::type("ClangBackEnd::RegisterTranslationUnitForEditorMessage");
    static const int updateTranslationUnitsForEditorMessageType = QMetaType::type("ClangBackEnd::UpdateTranslationUnitsForEditorMessage");
    static const int unregisterTranslationUnitsForEditorMessageType = QMetaType::type("ClangBackEnd::UnregisterTranslationUnitsForEditorMessage");
    static const int registerProjectPartsForEditorMessageType = QMetaType::type("ClangBackEnd::RegisterProjectPartsForEditorMessage");
    static const int unregisterProjectPartsForEditorMessageType = QMetaType::type("ClangBackEnd::UnregisterProjectPartsForEditorMessage");
    static const int registerUnsavedFilesForEditorMessageType = QMetaType::type("ClangBackEnd::RegisterUnsavedFilesForEditorMessage");
    static const int unregisterUnsavedFilesForEditorMessageType = QMetaType::type("ClangBackEnd::UnregisterUnsavedFilesForEditorMessage");
    static const int completeCodeMessageType = QMetaType::type("ClangBackEnd::CompleteCodeMessage");
    static const int requestDiagnosticsMessageType = QMetaType::type("ClangBackEnd::RequestDiagnosticsMessage");
    static const int requestHighlightingTypeMessage = QMetaType::type("ClangBackEnd::RequestHighlightingMessage");
    static const int updateVisibleTranslationUnitsMessageType = QMetaType::type("ClangBackEnd::UpdateVisibleTranslationUnitsMessage");

    int type = message.userType();

    if (type == endMessageType)
        end();
    else if (type == registerTranslationUnitsForEditorMessageType)
        registerTranslationUnitsForEditor(message.value<RegisterTranslationUnitForEditorMessage>());
    else if (type == updateTranslationUnitsForEditorMessageType)
        updateTranslationUnitsForEditor(message.value<UpdateTranslationUnitsForEditorMessage>());
    else if (type == unregisterTranslationUnitsForEditorMessageType)
        unregisterTranslationUnitsForEditor(message.value<UnregisterTranslationUnitsForEditorMessage>());
    else if (type == registerProjectPartsForEditorMessageType)
        registerProjectPartsForEditor(message.value<RegisterProjectPartsForEditorMessage>());
    else if (type == unregisterProjectPartsForEditorMessageType)
        unregisterProjectPartsForEditor(message.value<UnregisterProjectPartsForEditorMessage>());
    else if (type == registerUnsavedFilesForEditorMessageType)
        registerUnsavedFilesForEditor(message.value<RegisterUnsavedFilesForEditorMessage>());
    else if (type == unregisterUnsavedFilesForEditorMessageType)
        unregisterUnsavedFilesForEditor(message.value<UnregisterUnsavedFilesForEditorMessage>());
    else if (type == completeCodeMessageType)
        completeCode(message.value<CompleteCodeMessage>());
    else if (type == requestDiagnosticsMessageType)
        requestDiagnostics(message.value<RequestDiagnosticsMessage>());
    else if (type == requestHighlightingTypeMessage)
        requestHighlighting(message.value<RequestHighlightingMessage>());
    else if (type == updateVisibleTranslationUnitsMessageType)
        updateVisibleTranslationUnits(message.value<UpdateVisibleTranslationUnitsMessage>());
    else
        qWarning() << "Unknown IpcServerMessage";
}

void IpcServerInterface::addClient(IpcClientInterface *client)
{
    clientDispatcher.addClient(client);
}

void IpcServerInterface::removeClient(IpcClientInterface *client)
{
    clientDispatcher.removeClient(client);
}

IpcClientInterface *IpcServerInterface::client()
{
    return &clientDispatcher;
}

} // namespace ClangBackEnd

