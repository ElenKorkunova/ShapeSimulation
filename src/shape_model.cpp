#include "shape_model.h"
#include <variant>

ShapeModel::ShapeModel(QObject *parent) : QAbstractListModel(parent) {
    int id{0};
    // Populate with mock data for initial visual validation (C++17 aggregate initialization)
    m_shapes = {
        {id, ShapeType::Circle, 200.0f, 150.0f, 40.0f, 3.0f, 2.0f},    // Circle
        {++id, ShapeType::Square, 400.0f, 300.0f, 60.0f, -2.0f, 4.0f}, // Square
        {++id, ShapeType::Circle, 600.0f, 450.0f, 25.0f, 4.0f, -3.0f}, // Circle
        {++id, ShapeType::Triangle, 600.0f, 450.0f, 70.0f, 4.0f, -3.0f}
        // Type 2: Triangle (size 70)
    };
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
    case ShapeRoles::TypeRole:
        return static_cast<int>(shape.type);
    case ShapeRoles::XRole:      return shape.posX;
    case ShapeRoles::YRole:      return shape.posY;
    case ShapeRoles::SizeOrRadiusRole:
        return shape.sizeOrRadius;
    }
    return QVariant();
}

QHash<int, QByteArray> ShapeModel::roleNames() const {
    // Map C++ enum roles to string names used as properties inside QML delegates
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(ShapeRoles::TypeRole)]   = "shapeType";
    roles[static_cast<int>(ShapeRoles::XRole)]       = "posX";
    roles[static_cast<int>(ShapeRoles::YRole)]       = "posY";
    roles[static_cast<int>(ShapeRoles::SizeOrRadiusRole)] = "sizeOrRadius";
    return roles;
}

void ShapeModel::updatePositions(float delta, float screenWidth, float screenHeight)
{
    // 1. Integrate positions (Linear motion)
    for (auto &shape : m_shapes) {
        shape.posX += shape.vx * delta;
        shape.posY += shape.vy * delta;

        // Determine bounding boxes based on geometry type
        float extentX = (shape.type == ShapeType::Circle) ? shape.sizeOrRadius
                                                          : shape.sizeOrRadius / 2.0f;
        float extentY = (shape.type == ShapeType::Circle) ? shape.sizeOrRadius
                                                          : shape.sizeOrRadius / 2.0f;

        // Boundary collision handling (simple velocity vector inversion)
        if (shape.posX - extentX < 0.0f || shape.posX + extentX > screenWidth)
            shape.vx *= -1.0f;
        if (shape.posY - extentY < 0.0f || shape.posY + extentY > screenHeight)
            shape.vy *= -1.0f;
    }
}

void ShapeModel::refreshAllData()
{
    if (m_shapes.empty())
        return;

    // Создаем индексы для первой и последней строки
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, 0);

    // Передаем вектор измененных ролей. Это остановит бесконечный цикл обновления!
    QVector<int> rolesChange = {static_cast<int>(ShapeRoles::XRole),
                                static_cast<int>(ShapeRoles::YRole)};

    emit dataChanged(topLeft, bottomRight, rolesChange);
}
