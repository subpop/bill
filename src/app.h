// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

#pragma once

#include "journalmodel.h"

#include <QLoggingCategory>
#include <QObject>

class QQuickWindow;

Q_DECLARE_LOGGING_CATEGORY(app)

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY (JournalModel* journal READ journal NOTIFY journalChanged)

public:

    App();

    ~App();

    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    JournalModel *journal() const;

Q_SIGNALS:
    void journalChanged();

private:
    JournalModel *m_model;
};
