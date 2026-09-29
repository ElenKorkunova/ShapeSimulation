#include "shape_model.h"

ShapeModel::ShapeModel(QObject *parent) : QAbstractListModel(parent) {
    // Populate with mock data for initial visual validation (C++17 aggregate initialization)
    m_shapes = {
        { 0, 0, 200.0f, 150.0f, 40.0f, 3.0f,  2.0f  }, // Circle
        { 1, 1, 400.0f, 300.0f, 60.0f, -2.0f, 4.0f  }, // Square
        { 2, 0, 600.0f, 450.0f, 25.0f, 4.0f,  -3.0f },  // Circle
        { 3, 2, 600.0f, 450.0f, 70.0f,  4.0f,  -3.0f }  // Type 2: Triangle (size 70)
    };

    // Connect the high-precision timer to our physics update function
    connect(&m_updateTimer, &QTimer::timeout, this, &ShapeModel::updatePhysicsTick);
}

int ShapeModel::rowCount(const QModelIndex &parent) const {
    // For list models, row count is valid only if the parent index is invalid (root item)
    if (parent.isValid()) return 0;
    return static_cast<int>(m_shapes.size());
}

QVariant ShapeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    const auto row = static_cast<size_t>(index.row());
    if (row >= m_shapes.size()) return QVariant();

    const auto &shape = m_shapes[row];

    using ShapeRoles = ShapeModel::ShapeRoles;
    // C++17 compile-time safety check for enum class switching
    switch (static_cast<ShapeRoles>(role)) {
    case ShapeRoles::TypeRole:   return shape.type;
    case ShapeRoles::XRole:      return shape.posX;
    case ShapeRoles::YRole:      return shape.posY;
    case ShapeRoles::SizeOrRadiusRole: return shape.sizeOrRadius;
    }
    return QVariant();
}

QHash<int, QByteArray> ShapeModel::roleNames() const {
    // Map C++ enum roles to string names used as properties inside QML delegates
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(ShapeRoles::TypeRole)]   = "shapeType";
    roles[static_cast<int>(ShapeRoles::XRole)]       = "posX";
    roles[static_cast<int>(ShapeRoles::YRole)]       = "posY";
    roles[static_cast<int>(ShapeRoles::SizeOrRadiusRole)]  = "sizeOrRadius";
    return roles;
}

void ShapeModel::startSimulation() {
    // ~60 frames per second update rate (16 milliseconds interval)
    m_updateTimer.start(16);
}

void ShapeModel::stopSimulation() {
    m_updateTimer.stop();
}

void ShapeModel::updatePhysicsTick() {
    // Temporary linear movement and screen-edge bounce logic before full physics integration
    for (auto &shape : m_shapes) {
        shape.posX += shape.vx;
        shape.posY += shape.vy;

        // Determine bounding boxes based on geometry type
        float extentX = (shape.type == 0) ? shape.sizeOrRadius : shape.sizeOrRadius / 2.0f;
        float extentY = (shape.type == 0) ? shape.sizeOrRadius : shape.sizeOrRadius / 2.0f;

        // Boundary collision handling (simple velocity vector inversion)
        if (shape.posX - extentX < 0.0f || shape.posX + extentX > kScreenWidth)  shape.vx *= -1.0f;
        if (shape.posY - extentY < 0.0f || shape.posY + extentY > kScreenHeight) shape.vy *= -1.0f;
    }

    // Crucial Qt Quick Step: notify the UI layer that ALL elements have updated positions
    emit dataChanged(index(0), index(rowCount() - 1));
}
