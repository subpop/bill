/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "journalmodel.h"
#include "version-bill.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "billconfig.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("bill"));

    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("bill"),
                         // A displayable program name string.
                         i18nc("@title", "Bullet Bill"),
                         // The program version string.
                         QStringLiteral(BILL_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Simple bullet journal"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("© 2022"));
    aboutData.addAuthor(i18nc("@info:credit", "Link Dupont"),
                        i18nc("@info:credit", "Author"),
                        QStringLiteral("link@sub-pop.net"),
                        QStringLiteral("https://sub-pop.net"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    auto config = billConfig::self();

    qmlRegisterSingletonInstance("org.kde.bill", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.bill", 1, 0, "AboutType", &about);

    qmlRegisterType<JournalModel>("org.kde.bill", 1, 0, "JournalModel");

    App application;
    qmlRegisterSingletonInstance("org.kde.bill", 1, 0, "App", &application);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
