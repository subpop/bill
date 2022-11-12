// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.bill 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Bullet Bill")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    onClosing: App.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    Component.onCompleted: App.restoreWindowGeometry(root)

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    property int counter: 0

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("Bullet Bill")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("About bill")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: page

    Kirigami.ScrollablePage {
        id: page

        Layout.fillWidth: true

        title: i18n("Journal")

        ColumnLayout {
            id: layout
            anchors.fill: parent

            Kirigami.CardsListView {
                id: listView

                model: App.journal

                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: Kirigami.AbstractCard {

                    contentItem: Item {
                        implicitWidth: delegateLayout.implicitWidth
                        implicitHeight: delegateLayout.implicitHeight

                        ColumnLayout {
                            id: delegateLayout

                            anchors.fill: parent

                            spacing: Kirigami.Units.mediumSpacing

                            Kirigami.Heading {
                                Layout.fillWidth: true

                                level: 2
                                text: Qt.formatDate(model.date)
                            }

                            Controls.TextArea {
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                text: model.content
                                wrapMode: TextEdit.WordWrap
                                onEditingFinished: {
                                    model.content = text
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
