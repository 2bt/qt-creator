/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
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
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef CUSTOMEXECUTABLECONFIGURATIONWIDGET_H
#define CUSTOMEXECUTABLECONFIGURATIONWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLineEdit;
class QComboBox;
class QLabel;
class QAbstractButton;
QT_END_NAMESPACE

namespace Utils {
class DetailsWidget;
class PathChooser;
}

namespace ProjectExplorer {
class ArgumentsAspect;
class TerminalAspect;
}

namespace QtSupport {
class CustomExecutableRunConfiguration;

namespace Internal {

class CustomExecutableConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    enum ApplyMode { InstantApply, DelayedApply};
    CustomExecutableConfigurationWidget(CustomExecutableRunConfiguration *rc, ApplyMode mode);
    ~CustomExecutableConfigurationWidget();

    void apply(); // only used for DelayedApply
    bool isValid() const;

signals:
    void validChanged();

private:
    void changed();
    void executableEdited();
    void workingDirectoryEdited();
    void environmentWasChanged();

    bool m_ignoreChange = false;
    CustomExecutableRunConfiguration *m_runConfiguration = 0;
    ProjectExplorer::ArgumentsAspect *m_temporaryArgumentsAspect = 0;
    ProjectExplorer::TerminalAspect *m_temporaryTerminalAspect = 0;
    Utils::PathChooser *m_executableChooser = 0;
    Utils::PathChooser *m_workingDirectory = 0;
    Utils::DetailsWidget *m_detailsContainer = 0;
};

} // namespace Internal
} // namespace QtSupport

#endif // CUSTOMEXECUTABLECONFIGURATIONWIDGET_H