// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

#include "app.h"

#include <KSharedConfig>
#include <KWindowConfig>

#include <QDate>
#include <QDir>
#include <QErrorMessage>
#include <QQuickWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>

Q_LOGGING_CATEGORY(app, "bill.app")

App::App()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QDir filePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    qCDebug(app) << "dir:" << filePath;
    filePath.mkpath(".");
    db.setDatabaseName( filePath.absoluteFilePath("bill.db"));
    if (!db.open()) {
        QSqlError err = db.lastError();
        qCritical() << "cannot connect to database:" << err.text();
        QErrorMessage msg;
        msg.showMessage(err.text());
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS journal (id INTEGER PRIMARY KEY, date TEXT NOT NULL UNIQUE ON CONFLICT IGNORE, content TEXT);")) {
        QSqlError err = query.lastError();
        qCritical() << "cannot execute query:" << err.text();
        QErrorMessage msg;
        msg.showMessage(err.text());
    }

    m_model = new JournalModel;
    m_model->setTable("journal");
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->setSort(m_model->fieldIndex("date"), Qt::DescendingOrder);

    if (!m_model->select()) {
        QSqlError err = m_model->lastError();
        qCritical() << "cannot select:" << err.text();
        QErrorMessage msg;
        msg.showMessage(err.text());
    }

    m_model->setFilter("date=" + QDate::currentDate().toString(Qt::ISODate));

    if (m_model->rowCount() == 0) {
        QSqlField field("date", QVariant::Date);
        field.setValue(QDate::currentDate());
        QSqlRecord record;
        record.append(field);
        if (!m_model->insertRecord(-1, record)) {
            QSqlError err = m_model->lastError();
            qCritical() << "cannot insert record:" << err.text();
            QErrorMessage msg;
            msg.showMessage(err.text());
        }
    }

    m_model->setFilter("");
}

App::~App() noexcept
{
    delete m_model;
}


void App::restoreWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

JournalModel *App::journal() const
{
    return m_model;
}
