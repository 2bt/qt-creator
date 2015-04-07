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

#include "codecompletion.h"

#include <QDebug>

namespace CodeModelBackEnd {

CodeCompletion::CodeCompletion(const Utf8String &text,
                               const Utf8String &hint,
                               const Utf8String &snippet,
                               quint32 priority,
                               Kind completionKind,
                               Availability availability,
                               bool hasParameters)
    : text_(text),
      hint_(hint),
      snippet_(snippet),
      priority_(priority),
      completionKind_(completionKind),
      availability_(availability),
      hasParameters_(hasParameters)
{
}

const Utf8String &CodeCompletion::text() const
{
    return text_;
}
const Utf8String &CodeCompletion::hint() const
{
    return hint_;
}

const Utf8String &CodeCompletion::snippet() const
{
    return snippet_;
}

CodeCompletion::Kind CodeCompletion::completionKind() const
{
    return completionKind_;
}

CodeCompletion::Availability CodeCompletion::availability() const
{
    return availability_;
}

bool CodeCompletion::hasParameters() const
{
    return hasParameters_;
}

quint32 CodeCompletion::priority() const
{
    return priority_;
}

QDataStream &operator<<(QDataStream &out, const CodeCompletion &command)
{
    out << command.text_;
    out << command.text_;
    out << command.hint_;
    out << command.snippet_;
    out << command.priority_;
    out << command.completionKindAsInt;
    out << command.availabilityAsInt;
    out << command.hasParameters_;

    return out;
}

QDataStream &operator>>(QDataStream &in, CodeCompletion &command)
{

    in >> command.text_;
    in >> command.text_;
    in >> command.hint_;
    in >> command.snippet_;
    in >> command.priority_;
    in >> command.completionKindAsInt;
    in >> command.availabilityAsInt;
    in >> command.hasParameters_;

    return in;
}

bool operator == (const CodeCompletion &first, const CodeCompletion &second)
{
    return first.text_ == second.text_
            && first.completionKind_ == second.completionKind_;
}

bool operator < (const CodeCompletion &first, const CodeCompletion &second)
{
    return first.text_ < second.text_;
}

QDebug operator <<(QDebug debug, const CodeCompletion &command)
{
    debug.nospace() << "CodeCompletion(";

    debug.nospace() << command.text_ << ", ";
    debug.nospace() << command.hint_ << ", ";
    debug.nospace() << command.snippet_ << ", ";
    debug.nospace() << command.priority_ << ", ";
    debug.nospace() << command.completionKind_ << ", ";
    debug.nospace() << command.availability_ << ", ";
    debug.nospace() << command.hasParameters_;

    debug.nospace() << ")";

    return debug;
}

void PrintTo(const CodeCompletion &command, ::std::ostream* os)
{
    *os << "CodeCompletion(";

    *os << command.text_.constData() << ", ";
    *os << command.hint_.constData() << ", ";
    *os << command.snippet_.constData() << ", ";
    *os << command.priority_ << ", ";
    *os << command.completionKind_ << ", ";
    *os << command.availability_ << ", ";
    *os << command.hasParameters_;

    *os << ")";
}

} // namespace CodeModelBackEnd

