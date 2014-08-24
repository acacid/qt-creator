static const unsigned minimumRequiredVersion = 0x010702;
static const char RC_GIT_MIME_XML[] = ":/git/Git.mimetypes.xml";
static const VcsBase::VcsBaseEditorParameters editorParameters[] = {
Q_DECLARE_METATYPE(Git::Internal::GitClientMemberFunc)

using namespace Git;
using namespace Git::Internal;
GitPlugin *GitPlugin::m_instance = 0;
    const int mid = qRegisterMetaType<GitClientMemberFunc>();
    Q_UNUSED(mid)
static const VcsBase::VcsBaseSubmitEditorParameters submitParameters = {
    VcsBase::VcsBaseSubmitEditorParameters::DiffRows
ParameterActionCommandPair
        GitPlugin::createParameterAction(Core::ActionContainer *ac,
                                         const QString &defaultText, const QString &parameterText,
                                         Core::Id id, const Core::Context &context,
                                         bool addToLocator)
{
    Utils::ParameterAction *action = new Utils::ParameterAction(defaultText, parameterText,
                                                                Utils::ParameterAction::EnabledWithParameter,
                                                                this);
    Core::Command *command = Core::ActionManager::registerAction(action, id, context);
    return ParameterActionCommandPair(action, command);
ParameterActionCommandPair
        GitPlugin::createFileAction(Core::ActionContainer *ac,
                                    const QString &defaultText, const QString &parameterText,
                                    Core::Id id, const Core::Context &context, bool addToLocator,
                                    const char *pluginSlot)
    const ParameterActionCommandPair rc = createParameterAction(ac, defaultText, parameterText, id, context, addToLocator);
    m_fileActions.push_back(rc.first);
    connect(rc.first, SIGNAL(triggered()), this, pluginSlot);
    return rc;
ParameterActionCommandPair
        GitPlugin::createProjectAction(Core::ActionContainer *ac,
                                       const QString &defaultText, const QString &parameterText,
                                       Core::Id id, const Core::Context &context, bool addToLocator,
                                       const char *pluginSlot)
    const ParameterActionCommandPair rc = createParameterAction(ac, defaultText, parameterText, id, context, addToLocator);
    m_projectActions.push_back(rc.first);
    connect(rc.first, SIGNAL(triggered()), this, pluginSlot);
    return rc;
ActionCommandPair
        GitPlugin::createRepositoryAction(Core::ActionContainer *ac,
                                          const QString &text, Core::Id id,
                                          const Core::Context &context, bool addToLocator)
{
    QAction  *action = new QAction(text, this);
    Core::Command *command = Core::ActionManager::registerAction(action, id, context);
    return ActionCommandPair(action, command);
ActionCommandPair
        GitPlugin::createRepositoryAction(Core::ActionContainer *ac,
                                          const QString &text, Core::Id id,
                                          const Core::Context &context, bool addToLocator,
                                          const char *pluginSlot)
    const ActionCommandPair rc = createRepositoryAction(ac, text, id, context, addToLocator);
    connect(rc.first, SIGNAL(triggered()), this, pluginSlot);
    rc.first->setData(id.uniqueIdentifier());
    return rc;
// taking the directory. Store the member function as data on the action.
ActionCommandPair
        GitPlugin::createRepositoryAction(Core::ActionContainer *ac,
                                          const QString &text, Core::Id id,
                                          const Core::Context &context, bool addToLocator,
                                          GitClientMemberFunc func)
    const ActionCommandPair rc = createRepositoryAction(ac, text, id, context, addToLocator);
    rc.first->setData(qVariantFromValue(func));
    connect(rc.first, SIGNAL(triggered()), this, SLOT(gitClientMemberFuncRepositoryAction()));
    return rc;
    m_settings.readSettings(Core::ICore::settings());
    typedef VcsBase::VcsEditorFactory<GitEditor> GitEditorFactory;
    typedef VcsBase::VcsSubmitEditorFactory<GitSubmitEditor> GitSubmitEditorFactory;
    Core::Context globalcontext(Core::Constants::C_GLOBAL);
    const int editorCount = sizeof(editorParameters)/sizeof(VcsBase::VcsBaseEditorParameters);
    m_commandLocator = new Core::CommandLocator("Git", prefix, prefix);
    Core::ActionContainer *toolsContainer =
        Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    Core::ActionContainer *gitContainer = Core::ActionManager::createMenu("Git");
    Core::ActionContainer *currentFileMenu = Core::ActionManager::createMenu("Git.CurrentFileMenu");
    ParameterActionCommandPair parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Diff Current File"), tr("Diff of \"%1\""),
                               "Git.Diff", globalcontext, true,
                               SLOT(diffCurrentFile()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+D") : tr("Alt+G,Alt+D")));

    parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Log Current File"), tr("Log of \"%1\""),
                               "Git.Log", globalcontext, true, SLOT(logFile()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+L") : tr("Alt+G,Alt+L")));

    parameterActionCommand
                = createFileAction(currentFileMenu,
                                   tr("Blame Current File"), tr("Blame for \"%1\""),
                                   "Git.Blame",
                                   globalcontext, true, SLOT(blameFile()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+B") : tr("Alt+G,Alt+B")));

    // ------
    parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Stage File for Commit"), tr("Stage \"%1\" for Commit"),
                               "Git.Stage", globalcontext, true, SLOT(stageFile()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+A") : tr("Alt+G,Alt+A")));
    parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Unstage File from Commit"), tr("Unstage \"%1\" from Commit"),
                               "Git.Unstage", globalcontext, true, SLOT(unstageFile()));
    parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Undo Unstaged Changes"), tr("Undo Unstaged Changes for \"%1\""),
                               "Git.UndoUnstaged", globalcontext,
                               true, SLOT(undoUnstagedFileChanges()));
    parameterActionCommand
            = createFileAction(currentFileMenu,
                               tr("Undo Uncommitted Changes"), tr("Undo Uncommitted Changes for \"%1\""),
                               "Git.Undo", globalcontext,
                               true, SLOT(undoFileChanges()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+U") : tr("Alt+G,Alt+U")));
    /* \"Current File" menu */

    // ------------
    Core::ActionContainer *currentProjectMenu = Core::ActionManager::createMenu("Git.CurrentProjectMenu");
    parameterActionCommand
            = createProjectAction(currentProjectMenu,
                                  tr("Diff Current Project"), tr("Diff Project \"%1\""),
                                  "Git.DiffProject", globalcontext, true,
                                  SLOT(diffCurrentProject()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+Shift+D") : tr("Alt+G,Alt+Shift+D")));

    parameterActionCommand
            = createProjectAction(currentProjectMenu,
                                  tr("Log Project"), tr("Log Project \"%1\""),
                                  "Git.LogProject", globalcontext, true,
                                  SLOT(logProject()));
    parameterActionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+K") : tr("Alt+G,Alt+K")));
    parameterActionCommand
                = createProjectAction(currentProjectMenu,
                                      tr("Clean Project..."), tr("Clean Project \"%1\"..."),
                                      "Git.CleanProject", globalcontext,
                                      true, SLOT(cleanProject()));
    /* \"Current Project" menu */
    // --------------
    Core::ActionContainer *localRepositoryMenu = Core::ActionManager::createMenu("Git.LocalRepositoryMenu");
    createRepositoryAction(localRepositoryMenu,
                           tr("Diff"), "Git.DiffRepository",
    createRepositoryAction(localRepositoryMenu,
                           tr("Log"), "Git.LogRepository",
    createRepositoryAction(localRepositoryMenu,
                           tr("Reflog"), "Git.ReflogRepository",
    createRepositoryAction(localRepositoryMenu,
                           tr("Clean..."), "Git.CleanRepository",
    createRepositoryAction(localRepositoryMenu,
                           tr("Status"), "Git.StatusRepository",
    ActionCommandPair actionCommand = createRepositoryAction(localRepositoryMenu,
                                                             tr("Commit..."), "Git.Commit",
                                                             globalcontext, true, SLOT(startCommit()));
    actionCommand.second->setDefaultKeySequence(QKeySequence(Core::UseMacShortcuts ? tr("Meta+G,Meta+C") : tr("Alt+G,Alt+C")));
                                   globalcontext, true, SLOT(startFixupCommit())).first;
                                   globalcontext, true, SLOT(startRebase())).first;
                                   globalcontext, true, SLOT(updateSubmodules())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
                                   globalcontext, true, SLOT(continueOrAbortCommand())).first;
    Core::ActionContainer *patchMenu = Core::ActionManager::createMenu("Git.PatchMenu");
    parameterActionCommand =
    m_applyCurrentFilePatchAction = parameterActionCommand.first;
    Core::ActionContainer *stashMenu = Core::ActionManager::createMenu("Git.StashMenu");
    actionCommand = createRepositoryAction(stashMenu,
                                           tr("Stash"), "Git.Stash",
                                           globalcontext, true, SLOT(stash()));
    actionCommand.first->setToolTip(tr("Saves the current state of your work and resets the repository."));
    actionCommand = createRepositoryAction(stashMenu,
                                           tr("Take Snapshot..."), "Git.StashSnapshot",
                                           globalcontext, true, SLOT(stashSnapshot()));
    actionCommand.first->setToolTip(tr("Saves the current state of your work."));
    actionCommand = createRepositoryAction(stashMenu,
                                           tr("Stash Pop"), "Git.StashPop",
                                           globalcontext, true, &GitClient::stashPop);
    actionCommand.first->setToolTip(tr("Restores changes saved to the stash list using \"Stash\"."));
    Core::ActionContainer *remoteRepositoryMenu = Core::ActionManager::createMenu("Git.RemoteRepositoryMenu");
    createRepositoryAction(remoteRepositoryMenu,
                           tr("Fetch"), "Git.Fetch",
    createRepositoryAction(remoteRepositoryMenu,
                           tr("Pull"), "Git.Pull",
    actionCommand = createRepositoryAction(remoteRepositoryMenu,
                                           tr("Push"), "Git.Push",
                                           globalcontext, true, SLOT(push()));
    Core::ActionContainer *subversionMenu = Core::ActionManager::createMenu("Git.Subversion");
    Core::ActionContainer *gitToolsMenu = Core::ActionManager::createMenu("Git.GitToolsMenu");
    parameterActionCommand
            = createFileAction(gitToolsMenu,
                               tr("Gitk Current File"), tr("Gitk of \"%1\""),
                               "Git.GitkFile", globalcontext, true, SLOT(gitkForCurrentFile()));
    parameterActionCommand
            = createFileAction(gitToolsMenu,
                               tr("Gitk for folder of Current File"), tr("Gitk for folder of \"%1\""),
                               "Git.GitkFolder", globalcontext, true, SLOT(gitkForCurrentFolder()));
    m_repositoryBrowserAction
            = createRepositoryAction(gitToolsMenu,
                                     tr("Repository Browser"), "Git.LaunchRepositoryBrowser",
                                     globalcontext, true, &GitClient::launchRepositoryBrowser).first;
                                   globalcontext, true, SLOT(startMergeTool())).first;
    Core::Command *createRepositoryCommand = Core::ActionManager::registerAction(repositoryAction, "Git.CreateRepository", globalcontext);
    Core::Context submitContext(Constants::C_GITSUBMITEDITOR);
    m_submitCurrentAction = new QAction(VcsBase::VcsBaseSubmitEditor::submitIcon(), tr("Commit"), this);
    Core::Command *command = Core::ActionManager::registerAction(m_submitCurrentAction, Constants::SUBMIT_CURRENT, submitContext);
    m_diffSelectedFilesAction = new QAction(VcsBase::VcsBaseSubmitEditor::diffIcon(), tr("Diff &Selected Files"), this);
    command = Core::ActionManager::registerAction(m_diffSelectedFilesAction, Constants::DIFF_SELECTED, submitContext);
    command = Core::ActionManager::registerAction(m_undoAction, Core::Constants::UNDO, submitContext);
    command = Core::ActionManager::registerAction(m_redoAction, Core::Constants::REDO, submitContext);
    connect(Core::VcsManager::instance(), SIGNAL(repositoryChanged(QString)),
    connect(Core::VcsManager::instance(), SIGNAL(repositoryChanged(QString)),
    if (!Core::MimeDatabase::addMimeTypes(QLatin1String(RC_GIT_MIME_XML), errorMessage))
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const int lineNumber = VcsBase::VcsBaseEditorWidget::lineNumberOfCurrentEditor(state.currentFile());
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    Core::FileChangeBlocker fcb(state.currentFile());
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    LogChangeDialog dialog(true, Core::ICore::mainWindow());
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    LogChangeDialog dialog(false, Core::ICore::mainWindow());
    const VcsBase::VcsBasePluginState state = currentState();
    Core::Id id = action ? Core::Id::fromUniqueIdentifier(action->data().toInt()) : Core::Id();
    ChangeSelectionDialog dialog(state.topLevel(), id, Core::ICore::mainWindow());
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
        VcsBase::VcsBaseOutputWindow::instance()->appendWarning(tr("Another submit is currently being executed."));
    const VcsBase::VcsBasePluginState state = currentState();
        VcsBase::VcsBaseOutputWindow::instance()->appendError(errorMessage);
    Utils::TempFileSaver saver;
        VcsBase::VcsBaseOutputWindow::instance()->appendError(saver.errorString());
    Core::IDocument *curDocument = Core::EditorManager::currentDocument();
    Core::InfoBar *infoBar = curDocument->infoBar();
    Core::Id gitVersionWarning("GitVersionWarning");
    infoBar->addInfo(Core::InfoBarEntry(gitVersionWarning,
                        Core::InfoBarEntry::GlobalSuppressionEnabled));
Core::IEditor *GitPlugin::openSubmitEditor(const QString &fileName, const CommitData &cd)
    Core::IEditor *editor = Core::EditorManager::openEditor(fileName, Constants::GITSUBMITEDITOR_ID);
    Core::IDocument *document = submitEditor->document();
    Core::EditorManager::closeEditor();
    Core::IDocument *editorDocument = editor->document();
    const VcsBase::VcsBaseSubmitEditor::PromptSubmitResult answer
    case VcsBase::VcsBaseSubmitEditor::SubmitCanceled:
    case VcsBase::VcsBaseSubmitEditor::SubmitDiscarded:
    VcsBase::SubmitFileModel *model = qobject_cast<VcsBase::SubmitFileModel *>(editor->fileModel());
        if (!Core::DocumentManager::saveDocument(editorDocument))
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
// Retrieve member function of git client stored as user data of action
static inline GitClientMemberFunc memberFunctionFromAction(const QObject *o)
{
    if (o) {
        if (const QAction *action = qobject_cast<const QAction *>(o)) {
            const QVariant v = action->data();
            if (v.canConvert<GitClientMemberFunc>())
                return qvariant_cast<GitClientMemberFunc>(v);
        }
    }
    return 0;
}

void GitPlugin::gitClientMemberFuncRepositoryAction()
{
    const VcsBase::VcsBasePluginState state = currentState();
    QTC_ASSERT(state.hasTopLevel(), return);
    // Retrieve member function and invoke on repository
    GitClientMemberFunc func = memberFunctionFromAction(sender());
    QTC_ASSERT(func, return);
    (m_gitClient->*func)(state.topLevel());
}

    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
    QWidget *parent = Core::ICore::mainWindow();
    VcsBase::CleanDialog dialog(parent);
    const VcsBase::VcsBasePluginState state = currentState();
    Core::IDocument *document = Core::DocumentModel::documentForFilePath(fileName);
    return Core::DocumentManager::saveModifiedDocument(document);
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
        file =  QFileDialog::getOpenFileName(Core::ICore::mainWindow(),
                                             tr("Choose Patch"),
                                             QString(), filter);
    VcsBase::VcsBaseOutputWindow *outwin = VcsBase::VcsBaseOutputWindow::instance();
    if (!Core::DocumentManager::saveAllModifiedDocuments())
    const VcsBase::VcsBasePluginState state = currentState();
    const VcsBase::VcsBasePluginState state = currentState();
        dialog = new NonModalDialog(Core::ICore::mainWindow());
void GitPlugin::updateActions(VcsBase::VcsBasePlugin::ActionState as)
    foreach (Utils::ParameterAction *fileAction, m_fileActions)
    foreach (Utils::ParameterAction *projectAction, m_projectActions)
#include "clonewizardpage.h"

#include <QTest>

Q_DECLARE_METATYPE(FileStates)

    GitEditor editor(editorParameters + 3, 0);
    GitEditor editor(editorParameters + 1, 0);