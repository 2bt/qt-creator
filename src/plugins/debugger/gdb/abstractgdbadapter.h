/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef DEBUGGER_ABSTRACT_GDB_ADAPTER
#define DEBUGGER_ABSTRACT_GDB_ADAPTER

#include <QtCore/QObject>
#include <QtCore/QProcess>

namespace Debugger {
namespace Internal {

class GdbEngine;

// AbstractGdbAdapter is inherited by PlainGdbAdapter used for local
// debugging and TrkGdbAdapter used for on-device debugging.
// In the PlainGdbAdapter case it's just a wrapper around a QProcess running
// gdb, in the TrkGdbAdapter case it's the interface to the gdb process in
// the whole rfomm/gdb/gdbserver combo.
class AbstractGdbAdapter : public QObject
{
    Q_OBJECT

public:
    AbstractGdbAdapter(QObject *parent = 0) : QObject(parent) {}

    virtual void setEngine(GdbEngine *engine) { m_engine = engine; }
    virtual void start(const QString &program, const QStringList &args,
        QIODevice::OpenMode mode = QIODevice::ReadWrite) = 0;
    virtual void kill() = 0;
    virtual void terminate() = 0;
    //virtual bool waitForStarted(int msecs = 30000) = 0;
    virtual bool waitForFinished(int msecs = 30000) = 0;
    virtual QProcess::ProcessState state() const = 0;
    virtual QString errorString() const = 0;
    virtual QByteArray readAllStandardError() = 0;
    virtual QByteArray readAllStandardOutput() = 0;
    virtual qint64 write(const char *data) = 0;
    virtual void setWorkingDirectory(const QString &dir) = 0;
    virtual void setEnvironment(const QStringList &env) = 0;
    virtual bool isAdapter() const = 0;

    virtual void attach() = 0;
    virtual void interruptInferior() = 0;

signals:
    void error(QProcess::ProcessError);
    void started();
    void readyReadStandardOutput();
    void readyReadStandardError();
    void finished(int, QProcess::ExitStatus);

protected:
    GdbEngine *m_engine;
};

} // namespace Internal
} // namespace Debugger

#endif // DEBUGGER_ABSTRACT_GDB_ADAPTER
