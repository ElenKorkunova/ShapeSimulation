#ifndef SHAPE_MODEL_H
#define SHAPE_MODEL_H

#include <QtQml/qqmlregistration.h>
#include <QAbstractListModel>
#include <vector>

class ShapeModel : public QAbstractListModel {
    Q_OBJECT

    QML_ELEMENT // Automatically registers this class as a QML type inside Simulation.Backend

    private:
        static constexpr float kBasicSizeOrRadius = 80.0f;

    public:
        // Strongly typed enums for QML roles instead of raw ints or old Qt enums
        enum class ShapeRoles { TypeRole = Qt::UserRole + 1, XRole, YRole, SizeOrRadiusRole };
        Q_ENUM(ShapeRoles)

        //
        enum class ShapeType { Circle = 0, Square, Triangle };
        Q_ENUM(ShapeType)

        // Dummy structure to represent shapes until we implement the full physics engine
        /**
 * @brief Plain Old Data (POD) structure representing the state of a simulated geometric shape.
 *
 * This structure acts as a data transfer object (DTO) between the core physics engine
 * and the Qt Quick QML view layer. It stores spatial, kinematic, and structural properties
 * aligned with the Qt Quick 2D coordinate system (where the origin (0,0) is located at the
 * top-left corner and the Y-axis points downwards).
 */
        struct ShapeData
        {
            // Unique identifier for tracking the shape instance across simulation frames.
            int id;

            ShapeType type;

            // Horizontal coordinate of the shape's center of mass (X-axis increases to the right).
            float posX;

            // Vertical coordinate of the shape's center of mass (Y-axis increases downwards).
            float posY;

            // Size or Radius of the shape.
            float sizeOrRadius;

            // Horizontal velocity component measured in pixels per second (or pixels per frame interval).
            float vx;

            // Vertical velocity component measured in pixels per second (or pixels per frame interval).
            float vy;

            float mass;
        };

        explicit ShapeModel(QObject *parent = nullptr);

        //        Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

        // Core QAbstractListModel interface overrides (required for QML view bindings)
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        [[nodiscard]] const std::vector<ShapeData> &shapes() const noexcept { return m_shapes; }

        void updatePositions(float delta, float screenWidth, float screenHeight);

        void refreshAllData();

    private:
        std::vector<ShapeData> m_shapes;
};

#endif // SHAPE_MODEL_H
