#ifndef ENGINE_H
#define ENGINE_H

#include "shape_model.h"

#include <QObject> // Обязателен для макроса Q_OBJECT и наследования от QObject
//#include <QtQml/qqmlregistration.h> // Обязателен для макроса QML_ELEMENT в Qt 6
#include <QtQml> // Альтернативный (общий) заголовок для QML (можно использовать вместо qqmlregistration.h)

#include <QTimer>
#include <cmath>
#include <vector>

// Modern C++17 strongly typed geometries
// struct Circle {
//     float radius;
// };

// struct Square {
//     float size;
// };

// // Physics and transform state common to all shapes
// struct PhysicsBody {
//     int id;
//     float posX;
//     float posY;
//     float vx;
//     float vy;
//     float mass;
//     // std::variant acts as a type-safe union for our shapes (no virtual table overhead)
//     std::variant<Circle, Square> geometry;
// };

class PhysicsEngine : public QObject
{
    Q_OBJECT

    QML_ELEMENT // Automatically registers this class as a QML type inside Simulation.Backend

    // Hardcoded screen bounds for simple edge bouncing demo
    static constexpr float kScreenWidth = 800.0f;
    static constexpr float kScreenHeight = 600.0f;

public:
    explicit PhysicsEngine(QObject *parent = nullptr);

    //    void updatePhysics(float deltaTime);

    // Объявляем свойство, которое будет доступно в QML
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(ShapeModel *shapeModel READ shapeModel WRITE setShapeModel NOTIFY shapeModelChanged)

    // Invocable methods accessible directly from JavaScript context in QML
    Q_INVOKABLE void startSimulation();
    Q_INVOKABLE void stopSimulation();

    int width() const { return m_width; }
    void setWidth(int w)
    {
        if (m_width != w) {
            m_width = w;
            emit widthChanged();
        }
    }

    int height() const { return m_height; }
    void setHeight(int h)
    {
        if (m_height != h) {
            m_height = h;
            emit heightChanged();
        }
    }

    ShapeModel *shapeModel() const { return m_shapeModel; }

    void setShapeModel(ShapeModel *model)
    {
        if (m_shapeModel != model) {
            m_shapeModel = model;
            emit shapeModelChanged();
        }
    }

private:
    // void handleWallCollisions() noexcept;
    // void handleBodyCollisions() noexcept;
    void updatePhysics();

    // float m_containerWidth;
    // float m_containerHeight;
    int m_nextId{0};
    float m_width{kScreenWidth};
    float m_height{kScreenHeight};

    ShapeModel *m_shapeModel;

    QTimer m_updateTimer;

signals:
    void widthChanged();
    void heightChanged();
    void shapeModelChanged();
};

#endif // ENGINE_H
