/*
 * SPDX-FileCopyrightText: 2019 David Edmundson <kde@davidedmundson.co.uk>
 * SPDX-FileCopyrightText: 2020 Noah Davis <noahadvs@gmail.com>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
import QtQuick
import QtQuick.Layouts
import org.kde.plasma.components as PlasmaComponents
import org.kde.kirigami as Kirigami

ComponentBase {
    id: root
    title: "Plasma Components 3 BusyIndicator"
    contentItem: ColumnLayout {
        spacing: Kirigami.Units.gridUnit

        PlasmaComponents.Label {
            wrapMode: Text.WordWrap
            text: "The BusyIndicator should have a height of 16px and should have a 1:1 aspect ratio"
            Layout.preferredWidth: Math.max(busyIndicatorLayout.width, root.implicitHeaderWidth)
        }
        PlasmaComponents.BusyIndicator {
            Layout.preferredHeight: 16
        }

        PlasmaComponents.Label {
            wrapMode: Text.WordWrap
            text: "The BusyIndicator should use its implicit size."
            Layout.preferredWidth: Math.max(busyIndicatorLayout.width, root.implicitHeaderWidth)
        }
        PlasmaComponents.BusyIndicator {}

        PlasmaComponents.Label {
            wrapMode: Text.WordWrap
            text: "The BusyIndicator should maintain a 1:1 aspect ratio, disappear when unchecked and restart when checked."
            Layout.preferredWidth: Math.max(busyIndicatorLayout.width, root.implicitHeaderWidth)
        }

        RowLayout {
            id: busyIndicatorLayout
            spacing: Kirigami.Units.gridUnit

            PlasmaComponents.BusyIndicator {
                Layout.fillWidth: true
                Layout.fillHeight: true
                running: runningButton.checked
            }

            PlasmaComponents.CheckBox {
                id: runningButton
                text: "Running"
                checked: true
            }
        }
    }
}
