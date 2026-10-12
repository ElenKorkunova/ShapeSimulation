#include "engine.h"

PhysicsEngine::PhysicsEngine(QObject *parent)
    : QObject(parent)
    , m_width(kScreenWidth)
    , m_height(kScreenHeight)
{
    // Connect the high-precision timer to our physics update function
    connect(&m_updateTimer, &QTimer::timeout, this, &PhysicsEngine::updatePhysics);
}

void PhysicsEngine::updatePhysics()
{
    // Безопасность превыше всего: если модель еще не привязана, ничего не делаем
    if (!m_shapeModel)
        return;

    m_shapeModel->updatePositions(m_updateTimer.remainingTime() / 10, m_width, m_height);

    // 2. Resolve wall bounds boundaries
    //handleWallCollisions();

    // 3. Resolve shapes-intersections (Your old physics logic goes here)
    //handleBodyCollisions();

    m_shapeModel->refreshAllData();
}

void PhysicsEngine::startSimulation()
{
    // ~60 frames per second update rate (16 milliseconds interval)
    m_updateTimer.start(16);
}

void PhysicsEngine::stopSimulation()
{
    m_updateTimer.stop();
}

// void PhysicsEngine::handleWallCollisions() noexcept {
//     for (auto &body : m_shapeModel->shapes()) {
//         // C++17 std::visit approach to extract dynamic properties without explicit dynamic_cast
//         float extentX = std::visit([](const auto& shape) -> float {
//             using T = std::decay_t<decltype(shape)>;
//             if constexpr (std::is_same_v<T, Circle>) return shape.radius;
//             else if constexpr (std::is_same_v<T, Square>) return shape.size / 2.0f;
//         }, body.geometry);

//         float extentY = extentX; // Simplified for basic bounding box evaluation

//         // Horizontal bounce
//         if (body.posX - extentX < 0.0f) {
//             body.posX = extentX;
//             body.vx *= -1.0f;
//         } else if (body.posX + extentX > m_containerWidth) {
//             body.posX = m_containerWidth - extentX;
//             body.vx *= -1.0f;
//         }

//         // Vertical bounce
//         if (body.posY - extentY < 0.0f) {
//             body.posY = extentY;
//             body.vy *= -1.0f;
//         } else if (body.posY + extentY > m_containerHeight) {
//             body.posY = m_containerHeight - extentY;
//             body.vy *= -1.0f;
//         }
//     }
// }

// void PhysicsEngine::handleBodyCollisions() noexcept {
//     // Placeholder: integrate your original shape-to-shape impulse logic here
// }
