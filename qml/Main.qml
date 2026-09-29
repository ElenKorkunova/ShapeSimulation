import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import Qt.labs.qmlmodels
// Required for DelegateChooser in Qt6

// Import the C++ backend module defined in CMakeLists.txt
import Simulation.Backend 1.0

Window {
    id: rootWindow

    // Window base parameters
    width: 800
    height: 600
    visible: true
    title: qsTr("Shape Physics Simulation (Qt6 + C++17)")

    // Professional dark background for the simulation canvas
    color: "#1e1e1e"

    // Instantiate the C++ model directly in QML declarative tree
    ShapeModel {
        id: physicsModel

        // Component.onCompleted is a built-in lifecycle signal.
        // It triggers as soon as the QML engine successfully loads this window.
        Component.onCompleted: physicsModel.startSimulation()
    }

    // Instantiates visual items for each row provided by the C++ QAbstractListModel
    Repeater {
        model: physicsModel

        // DelegateChooser acts as a polymorphism handler on the UI side.
        // It inspects the "shapeType" role from C++ and selects the matching delegate.
        delegate: DelegateChooser {
            role: "shapeType"

            // Case 0: Rendering a Circle (matches ShapeType::Circle in C++)
            DelegateChoice {
                roleValue: 0

                Rectangle {
                    // C++ model provides center coordinates (posX, posY).
                    // QML positions items via top-left corner. We apply a mathematical offset on the fly.
                    x: model.posX - model.sizeOrRadius
                    y: model.posY - model.sizeOrRadius

                    width: model.sizeOrRadius * 2
                    height: model.sizeOrRadius * 2
                    radius: model.sizeOrRadius // Setting radius to half of width/height makes it a perfect circle

                    color: "#00adb5" // Cyan accent color
                    border.color: "#ffffff"
                    border.width: 1
                }
            }

            // Case 1: Rendering a Square (matches ShapeType::Square in C++)
            DelegateChoice {
                roleValue: 1

                Rectangle {
                    // Apply top-left positioning offset based on the square's total size
                    x: model.posX - (model.sizeOrRadius / 2)
                    y: model.posY - (model.sizeOrRadius / 2)

                    width: model.sizeOrRadius
                    height: model.sizeOrRadius

                    color: "#ff5722" // Orange accent color
                    border.color: "#ffffff"
                    border.width: 1
                }
            }

            // Case 2: Rendering a Triangle (matches ShapeType::Triangle / type 2 in C++)
            DelegateChoice {
                roleValue: 2

                // The Shape component handles modern GPU-accelerated vector drawing
                Shape {
                    // Center the bounding box of the shape around posX and posY
                    x: model.posX - (model.sizeOrRadius / 2)
                    y: model.posY - (model.sizeOrRadius / 2)
                    width: model.sizeOrRadius
                    height: model.sizeOrRadius

                    // Antialiasing is critical for smooth vector edges on high-DPI screens
                    layer.enabled: true
                    layer.samples: 4

                    ShapePath {
                        strokeColor: "#ffffff"
                        strokeWidth: 1
                        fillColor: "#e1b12c" // Nice amber/yellow color for the triangle

                        // 1. Move the virtual pen to the top-center point of the bounding box
                        startX: model.sizeOrRadius / 2
                        startY: 0

                        // 2. Draw a line to the bottom-right corner
                        PathLine {
                            x: model.sizeOrRadius
                            y: model.sizeOrRadius
                        }

                        // 3. Draw a line to the bottom-left corner
                        PathLine {
                            x: 0
                            y: model.sizeOrRadius
                        }

                        // 4. Draw a line back to the start point to close the geometry
                        PathLine {
                            x: model.sizeOrRadius / 2
                            y: 0
                        }
                    }
                }
            }
        }
    }

    // Simple status text layout for real-time diagnostics and debugging
    Text {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 15

        // Dynamic binding: automatically re-evaluates whenever rowCount changes in C++
        text: "Active Shapes: " + physicsModel.rowCount()
        color: "#888888"
        font.pointSize: 11
        font.family: "Monospace"
    }
}
