/****************************************************************************
**
** Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies).
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
#include "gmock/gmock-generated-matchers.h"

#include <project.h>
#include <utf8stringvector.h>
#include <projects.h>
#include <projectdoesnotexistsexception.h>

#include <chrono>

using testing::ElementsAre;
using testing::StrEq;
using testing::Pointwise;
using testing::Contains;
using testing::Gt;

namespace {

TEST(Project, CreateProject)
{
    Utf8String projectPath(Utf8StringLiteral("/tmp/blah.pro"));

    CodeModelBackEnd::Project project(projectPath);

    ASSERT_THAT(project.projectFilePath(), projectPath);
}

TEST(Project, CreateProjectWithProjectContainer)
{
    CodeModelBackEnd::ProjectContainer projectContainer(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-O")});

    CodeModelBackEnd::Project project(projectContainer);

    ASSERT_THAT(project.projectFilePath(), Utf8StringLiteral("pathToProject.pro"));
    ASSERT_THAT(project.arguments(), Contains(StrEq("-O")));
}

TEST(Project, SetArguments)
{
    CodeModelBackEnd::Project project(Utf8StringLiteral("/tmp/blah.pro"));

    project.setArguments(Utf8StringVector({Utf8StringLiteral("-O"), Utf8StringLiteral("-fast")}));

    ASSERT_THAT(project.arguments(), ElementsAre(StrEq("-O"), StrEq("-fast")));
}

TEST(Project, ArgumentCount)
{
    CodeModelBackEnd::Project project(Utf8StringLiteral("/tmp/blah.pro"));

    project.setArguments(Utf8StringVector({Utf8StringLiteral("-O"), Utf8StringLiteral("-fast")}));

    ASSERT_THAT(project.argumentCount(), 2);
}

TEST(Project, TimeStampIsUpdatedAsArgumentChanged)
{
    CodeModelBackEnd::Project project(Utf8StringLiteral("/tmp/blah.pro"));
    auto lastChangeTimePoint = project.lastChangeTimePoint();

    project.setArguments(Utf8StringVector({Utf8StringLiteral("-O"), Utf8StringLiteral("-fast")}));

    ASSERT_THAT(project.lastChangeTimePoint(), Gt(lastChangeTimePoint));

}

TEST(Project, GetNonExistingPoject)
{
    CodeModelBackEnd::Projects projects;

    ASSERT_THROW(projects.project(Utf8StringLiteral("pathToProject.pro")), CodeModelBackEnd::ProjectDoesNotExistsException);
}

TEST(Project, AddProjects)
{
    CodeModelBackEnd::ProjectContainer projectContainer(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-O")});
    CodeModelBackEnd::Projects projects;

    projects.createOrUpdate({projectContainer});

    ASSERT_THAT(projects.project(projectContainer.filePath()), CodeModelBackEnd::Project(projectContainer));
    ASSERT_THAT(projects.project(projectContainer.filePath()).arguments(), ElementsAre(StrEq("-O")));
}

TEST(Project, UpdateProjects)
{
    CodeModelBackEnd::ProjectContainer projectContainer(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-O")});
    CodeModelBackEnd::ProjectContainer projectContainerWithNewArguments(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-fast")});
    CodeModelBackEnd::Projects projects;
    projects.createOrUpdate({projectContainer});

    projects.createOrUpdate({projectContainerWithNewArguments});

    ASSERT_THAT(projects.project(projectContainer.filePath()), CodeModelBackEnd::Project(projectContainer));
    ASSERT_THAT(projects.project(projectContainer.filePath()).arguments(), ElementsAre(StrEq("-fast")));
}

TEST(Project, ThrowExceptionForAccesingRemovedProjects)
{
    CodeModelBackEnd::ProjectContainer projectContainer(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-O")});
    CodeModelBackEnd::Projects projects;
    projects.createOrUpdate({projectContainer});

    projects.remove({projectContainer.filePath()});

    ASSERT_THROW(projects.project(projectContainer.filePath()), CodeModelBackEnd::ProjectDoesNotExistsException);
}

TEST(Project, ProjectFilePathIsEmptyfterRemoving)
{
    CodeModelBackEnd::ProjectContainer projectContainer(Utf8StringLiteral("pathToProject.pro"), {Utf8StringLiteral("-O")});
    CodeModelBackEnd::Projects projects;
    projects.createOrUpdate({projectContainer});
    CodeModelBackEnd::Project project(projects.project(projectContainer.filePath()));

    projects.remove({projectContainer.filePath()});

    ASSERT_TRUE(project.projectFilePath().isEmpty());
}
}
