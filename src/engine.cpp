#include "engine.h"

PhysicsEngine::PhysicsEngine(float width, float height)
    : m_containerWidth(width), m_containerHeight(height) {}

void PhysicsEngine::initializeSampleData() {
    // Populate using C++17 aggregate initialization
    m_bodies = {
        { m_nextId++, 200.0f, 150.0f, 150.0f, 100.0f, 40.0f, Circle{40.0f} },
        { m_nextId++, 400.0f, 300.0f, -100.0f, 200.0f, 60.0f, Square{60.0f} },
        { m_nextId++, 600.0f, 450.0f, 200.0f, -150.0f, 25.0f, Circle{25.0f} }
    };
}

/*
    { 0, 0, 200.0f, 150.0f, 40.0f, 0.0f,  3.0f,  2.0f  }, // Circle
    { 1, 1, 400.0f, 300.0f, 0.0f,  60.0f, -2.0f, 4.0f  }, // Square
    { 2, 0, 600.0f, 450.0f, 25.0f, 0.0f,  4.0f,  -3.0f },  // Circle
    { 3, 2, 600.0f, 450.0f, 0.0f,  70.0f,  4.0f,  -3.0f }  // Type 2: Triangle (size 70)
};
*/

void PhysicsEngine::updatePhysics(float deltaTime) {
    // 1. Integrate positions (Linear motion)
    for (auto &body : m_bodies) {
        body.posX += body.vx * deltaTime;
        body.posY += body.vy * deltaTime;
    }

    // 2. Resolve wall bounds boundaries
    handleWallCollisions();

    // 3. Resolve shapes-intersections (Your old physics logic goes here)
    handleBodyCollisions();
}

void PhysicsEngine::handleWallCollisions() noexcept {
    for (auto &body : m_bodies) {
        // C++17 std::visit approach to extract dynamic properties without explicit dynamic_cast
        float extentX = std::visit([](const auto& shape) -> float {
            using T = std::decay_t<decltype(shape)>;
            if constexpr (std::is_same_v<T, Circle>) return shape.radius;
            else if constexpr (std::is_same_v<T, Square>) return shape.size / 2.0f;
        }, body.geometry);

        float extentY = extentX; // Simplified for basic bounding box evaluation

        // Horizontal bounce
        if (body.posX - extentX < 0.0f) {
            body.posX = extentX;
            body.vx *= -1.0f;
        } else if (body.posX + extentX > m_containerWidth) {
            body.posX = m_containerWidth - extentX;
            body.vx *= -1.0f;
        }

        // Vertical bounce
        if (body.posY - extentY < 0.0f) {
            body.posY = extentY;
            body.vy *= -1.0f;
        } else if (body.posY + extentY > m_containerHeight) {
            body.posY = m_containerHeight - extentY;
            body.vy *= -1.0f;
        }
    }
}

void PhysicsEngine::handleBodyCollisions() noexcept {
    // Placeholder: integrate your original shape-to-shape impulse logic here
}
