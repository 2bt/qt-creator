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

#include "projectdoesnotexistscommand.h"

#include <QDebug>

#include <QDataStream>

namespace CodeModelBackEnd {

ProjectDoesNotExistCommand::ProjectDoesNotExistCommand(const Utf8StringVector &projectFilePaths)
    : projectFilePaths_(projectFilePaths)
{
}


const Utf8StringVector &ProjectDoesNotExistCommand::projectFilePaths() const
{
    return projectFilePaths_;
}

QDataStream &operator<<(QDataStream &out, const ProjectDoesNotExistCommand &command)
{
    out << command.projectFilePaths_;

    return out;
}

QDataStream &operator>>(QDataStream &in, ProjectDoesNotExistCommand &command)
{
    in >> command.projectFilePaths_;

    return in;
}

bool operator == (const ProjectDoesNotExistCommand &first, const ProjectDoesNotExistCommand &second)
{
    return first.projectFilePaths_ == second.projectFilePaths_;
}

bool operator < (const ProjectDoesNotExistCommand &first, const ProjectDoesNotExistCommand &second)
{
    return first.projectFilePaths_ < second.projectFilePaths_;
}

QDebug operator <<(QDebug debug, const ProjectDoesNotExistCommand &command)
{
    debug.nospace() << "ProjectDoesNotExistCommand(";

    debug.nospace() << command.projectFilePaths_;

    debug.nospace() << ")";

    return debug;
}

void PrintTo(const ProjectDoesNotExistCommand &command, ::std::ostream* os)
{
    QString output;
    QDebug debug(&output);

    debug << command;

    *os << output.toUtf8().constData();
}


} // namespace CodeModelBackEnd

