/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock.h"

#include "gtest-qt-printing.h"

#include <ipcclientproxy.h>
#include <ipcserverproxy.h>
#include <clangipcserver.h>
#include <translationunitdoesnotexistsexception.h>
#include <translationunitparseerrorexception.h>

#include <cmbcodecompletedcommand.h>
#include <cmbcompletecodecommand.h>
#include <cmbechocommand.h>
#include <cmbregisterfilesforcodecompletioncommand.h>
#include <cmbunregisterfilesforcodecompletioncommand.h>
#include <cmbregisterprojectsforcodecompletioncommand.h>
#include <cmbunregisterprojectsforcodecompletioncommand.h>
#include <translationunitdoesnotexistscommand.h>

#include <QBuffer>
#include <QFile>

#include "mockipclient.h"

using testing::Property;
using testing::Contains;
using testing::Not;

namespace {

using CodeModelBackEnd::RegisterFilesForCodeCompletionCommand;
using CodeModelBackEnd::UnregisterFilesForCodeCompletionCommand;
using CodeModelBackEnd::RegisterProjectsForCodeCompletionCommand;
using CodeModelBackEnd::UnregisterProjectsForCodeCompletionCommand;
using CodeModelBackEnd::CompleteCodeCommand;
using CodeModelBackEnd::CodeCompletedCommand;
using CodeModelBackEnd::CodeCompletion;
using CodeModelBackEnd::FileContainer;
using CodeModelBackEnd::ProjectContainer;
using CodeModelBackEnd::TranslationUnitDoesNotExistsCommand;


class ClangIpcServer : public ::testing::Test
{
protected:
    void SetUp() override;

    void registerFiles();
    void registerProject();
    void changeProjectArguments();
    void changeProjectArgumentsToWrongValues();
    static const Utf8String unsavedContent(const QString &unsavedFilePath);

protected:
    MockIpcClient mockIpcClient;
    CodeModelBackEnd::ClangIpcServer clangServer;
    const Utf8String projectFilePath = Utf8StringLiteral("pathToProject.pro");
    const Utf8String functionTestFilePath = Utf8StringLiteral("data/complete_extractor_function.cpp");
    const Utf8String variableTestFilePath = Utf8StringLiteral("data/complete_extractor_variable.cpp");
    const QString unsavedTestFilePath = QStringLiteral("data/complete_extractor_function_unsaved.cpp");
    const QString updatedUnsavedTestFilePath = QStringLiteral("data/complete_extractor_function_unsaved_2.cpp");
    const Utf8String parseErrorTestFilePath = Utf8StringLiteral("data/complete_translationunit_parse_error.cpp");
};


void ClangIpcServer::SetUp()
{
    clangServer.addClient(&mockIpcClient);
    registerProject();
    registerFiles();
}

void ClangIpcServer::registerFiles()
{
    RegisterFilesForCodeCompletionCommand command({FileContainer(functionTestFilePath, projectFilePath, unsavedContent(unsavedTestFilePath), true),
                                                   FileContainer(variableTestFilePath, projectFilePath)});

    clangServer.registerFilesForCodeCompletion(command);
}

void ClangIpcServer::registerProject()
{
    RegisterProjectsForCodeCompletionCommand command({ProjectContainer(projectFilePath)});

    clangServer.registerProjectsForCodeCompletion(command);
}

void ClangIpcServer::changeProjectArguments()
{
    RegisterProjectsForCodeCompletionCommand command({ProjectContainer(projectFilePath, {Utf8StringLiteral("-DArgumentDefinition")})});

    clangServer.registerProjectsForCodeCompletion(command);
}

void ClangIpcServer::changeProjectArgumentsToWrongValues()
{
    RegisterProjectsForCodeCompletionCommand command({ProjectContainer(projectFilePath, {Utf8StringLiteral("-blah")})});

    clangServer.registerProjectsForCodeCompletion(command);
}

const Utf8String ClangIpcServer::unsavedContent(const QString &unsavedFilePath)
{
    QFile unsavedFileContentFile(unsavedFilePath);
    bool isOpen = unsavedFileContentFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!isOpen)
        ADD_FAILURE() << "File with the unsaved content cannot be opened!";

    return Utf8String::fromByteArray(unsavedFileContentFile.readAll());
}

TEST_F(ClangIpcServer, GetCodeCompletion)
{
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    CodeCompletion codeCompletion(Utf8StringLiteral("Function"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::FunctionCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Contains(codeCompletion))))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetCodeCompletionDependingOnArgumets)
{
    CompleteCodeCommand completeCodeCommand(variableTestFilePath,
                                            35,
                                            1,
                                            projectFilePath);
    CodeCompletion codeCompletion(Utf8StringLiteral("ArgumentDefinitionVariable"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::VariableCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Contains(codeCompletion))))
        .Times(1);

    changeProjectArguments();
    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetTranslationUnitDoesNotExistsForCodeCompletionOnNonExistingTranslationUnit)
{
    CompleteCodeCommand completeCodeCommand(Utf8StringLiteral("dontexists.cpp"),
                                            34,
                                            1,
                                            Utf8String());
    TranslationUnitDoesNotExistsCommand translationUnitDoesNotExistsCommand(Utf8StringLiteral("dontexists.cpp"), Utf8String());

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExists(translationUnitDoesNotExistsCommand))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}


TEST_F(ClangIpcServer, GetTranslationUnitDoesNotExistsForCompletingUnregisteredFile)
{
    CompleteCodeCommand completeCodeCommand(parseErrorTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    TranslationUnitDoesNotExistsCommand translationUnitDoesNotExistsCommand(parseErrorTestFilePath, projectFilePath);

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExists(translationUnitDoesNotExistsCommand))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetCodeCompletionForUnsavedFile)
{
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    CodeCompletion codeCompletion(Utf8StringLiteral("Method2"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::FunctionCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Contains(codeCompletion))))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetNoCodeCompletionAfterRemovingUnsavedFile)
{
    clangServer.registerFilesForCodeCompletion(RegisterFilesForCodeCompletionCommand({FileContainer(functionTestFilePath, projectFilePath)}));
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    CodeCompletion codeCompletion(Utf8StringLiteral("Method2"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::FunctionCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Not(Contains(codeCompletion)))))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetNewCodeCompletionAfterUpdatingUnsavedFile)
{
    clangServer.registerFilesForCodeCompletion(RegisterFilesForCodeCompletionCommand({FileContainer(functionTestFilePath,
                                                                                                    projectFilePath,
                                                                                                    unsavedContent(updatedUnsavedTestFilePath),
                                                                                                    true)}));
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    CodeCompletion codeCompletion(Utf8StringLiteral("Method3"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::FunctionCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Contains(codeCompletion))))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, GetTranslationUnitDoesNotExistsForUnregisterTranslationUnitWithWrongFilePath)
{
    FileContainer fileContainer(Utf8StringLiteral("foo.cpp"), projectFilePath);
    UnregisterFilesForCodeCompletionCommand command({fileContainer});
    TranslationUnitDoesNotExistsCommand translationUnitDoesNotExistsCommand(fileContainer);

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExists(translationUnitDoesNotExistsCommand))
        .Times(1);

    clangServer.unregisterFilesForCodeCompletion(command);
}

TEST_F(ClangIpcServer, GetTranslationUnitDoesNotExistsForUnregisterTranslationUnitWithWrongProjectFilePath)
{
    FileContainer fileContainer(functionTestFilePath, Utf8StringLiteral("bar.pro"));
    UnregisterFilesForCodeCompletionCommand command({fileContainer});
    TranslationUnitDoesNotExistsCommand translationUnitDoesNotExistsCommand(fileContainer);

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExists(translationUnitDoesNotExistsCommand))
        .Times(1);

    clangServer.unregisterFilesForCodeCompletion(command);
}

TEST_F(ClangIpcServer, UnregisterTranslationUnitAndTestFailingCompletion)
{
    FileContainer fileContainer(functionTestFilePath, projectFilePath);
    UnregisterFilesForCodeCompletionCommand command({fileContainer});
    clangServer.unregisterFilesForCodeCompletion(command);
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            projectFilePath);
    TranslationUnitDoesNotExistsCommand translationUnitDoesNotExistsCommand(fileContainer);

    EXPECT_CALL(mockIpcClient, translationUnitDoesNotExists(translationUnitDoesNotExistsCommand))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

TEST_F(ClangIpcServer, UnregisterProjectAndCompletionIsStillWorking)
{
    UnregisterProjectsForCodeCompletionCommand command({projectFilePath});
    clangServer.unregisterProjectsForCodeCompletion(command);
    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
                                            20,
                                            1,
                                            Utf8String());
    CodeCompletion codeCompletion(Utf8StringLiteral("Method2"),
                                  Utf8String(),
                                  Utf8String(),
                                  34,
                                  CodeCompletion::FunctionCompletionKind);

    EXPECT_CALL(mockIpcClient, codeCompleted(Property(&CodeCompletedCommand::codeCompletions, Contains(codeCompletion))))
        .Times(1);

    clangServer.completeCode(completeCodeCommand);
}

//TEST_F(ClangIpcServer, ThrowForTranslationUnitParsingError)
//{
//    changeProjectArgumentsToWrongValues();

//    CompleteCodeCommand completeCodeCommand(functionTestFilePath,
//                                            20,
//                                            1,
//                                            projectFilePath);

//    ASSERT_THROW(clangServer.completeCode(completeCodeCommand), CodeModelBackEnd::TranslationUnitParseErrorException);
//}
}
