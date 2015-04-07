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

#ifndef CODEMODELBACKEND_COMPLETECODECOMMAND_H
#define CODEMODELBACKEND_COMPLETECODECOMMAND_H

#include <QMetaType>
#include <QByteArray>

#include "codemodelbackendipc_global.h"

namespace CodeModelBackEnd {

class CMBIPC_EXPORT CompleteCodeCommand
{
    friend QDataStream &operator<<(QDataStream &out, const CompleteCodeCommand &command);
    friend QDataStream &operator>>(QDataStream &in, CompleteCodeCommand &command);
    friend bool operator == (const CompleteCodeCommand &first, const CompleteCodeCommand &second);
    friend bool operator < (const CompleteCodeCommand &first, const CompleteCodeCommand &second);
    friend QDebug operator <<(QDebug debug, const CompleteCodeCommand &command);
    friend void PrintTo(const CompleteCodeCommand &command, ::std::ostream* os);

public:
    CompleteCodeCommand() = default;
    CompleteCodeCommand(const QByteArray &fileName,
                        quint32 line,
                        quint32 column,
                        const QByteArray &commandLine);

    const QByteArray fileName() const;
    quint32 line() const;
    quint32 column() const;
    const QByteArray commandLine() const;

private:
    QByteArray fileName_;
    quint32 line_ = 0;
    quint32 column_ = 0;
    QByteArray commandLine_;
};

QDataStream &operator<<(QDataStream &out, const CompleteCodeCommand &command);
QDataStream &operator>>(QDataStream &in, CompleteCodeCommand &command);
bool operator == (const CompleteCodeCommand &first, const CompleteCodeCommand &second);
bool operator < (const CompleteCodeCommand &first, const CompleteCodeCommand &second);

QDebug operator <<(QDebug debug, const CompleteCodeCommand &command);
void PrintTo(const CompleteCodeCommand &command, ::std::ostream* os);

} // namespace CodeModelBackEnd

Q_DECLARE_METATYPE(CodeModelBackEnd::CompleteCodeCommand)

#endif // CODEMODELBACKEND_COMPLETECODECOMMAND_H
