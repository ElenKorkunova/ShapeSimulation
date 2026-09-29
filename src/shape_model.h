#ifndef SHAPE_MODEL_H
#define SHAPE_MODEL_H

#include <QtQml/qqmlregistration.h>
#include <QAbstractListModel>
#include <QTimer>
#include <vector>

// Dummy structure to represent shapes until we implement the full physics engine
/**
 * @brief Plain Old Data (POD) structure representing the state of a simulated geometric shape.
 *
 * This structure acts as a data transfer object (DTO) between the core physics engine
 * and the Qt Quick QML view layer. It stores spatial, kinematic, and structural properties
 * aligned with the Qt Quick 2D coordinate system (where the origin (0,0) is located at the
 * top-left corner and the Y-axis points downwards).
 */
struct ShapeData {
    // Unique identifier for tracking the shape instance across simulation frames.
    int id;

    /**
     * Type descriptor identifier matching the QML DelegateChooser state:
     * 0 = Circle, 1 = Square, 2 = Triangle.
     */
    int type;

    /** Horizontal coordinate of the shape's center of mass (X-axis increases to the right). */
    float posX;

    /** Vertical coordinate of the shape's center of mass (Y-axis increases downwards). */
    float posY;

    /** Radius of the shape (applicable only when type is set to 0 / Circle). */
    float sizeOrRadius;

    /** Horizontal velocity component measured in pixels per second (or pixels per frame interval). */
    float vx;

    /** Vertical velocity component measured in pixels per second (or pixels per frame interval). */
    float vy;
};

class ShapeModel : public QAbstractListModel {
    Q_OBJECT

    QML_ELEMENT // Automatically registers this class as a QML type inside Simulation.Backend

    private:
    // Hardcoded screen bounds for simple edge bouncing demo
    static constexpr float kScreenWidth = 800.0f;
    static constexpr float kScreenHeight = 600.0f;
    static constexpr float kBasicSizeOrRadius = kScreenHeight / 2.0f;


    public:
             // Strongly typed enums for QML roles instead of raw ints or old Qt enums
             enum class ShapeRoles {
                 TypeRole = Qt::UserRole + 1,
                 XRole,
                 YRole,
                 SizeOrRadiusRole
             };

    explicit ShapeModel(QObject *parent = nullptr);

    // Core QAbstractListModel interface overrides (required for QML view bindings)
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Invocable methods accessible directly from JavaScript context in QML
    Q_INVOKABLE void startSimulation();
    Q_INVOKABLE void stopSimulation();

private:
    void updatePhysicsTick();

    std::vector<ShapeData> m_shapes;
    QTimer m_updateTimer;
};


#endif // SHAPE_MODEL_H
