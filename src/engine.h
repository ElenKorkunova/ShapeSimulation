#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <variant>
#include <cmath>

// Modern C++17 strongly typed geometries
struct Circle {
    float radius;
};

struct Square {
    float size;
};

// Physics and transform state common to all shapes
struct PhysicsBody {
    int id;
    float posX;
    float posY;
    float vx;
    float vy;
    float mass;
    // std::variant acts as a type-safe union for our shapes (no virtual table overhead)
    std::variant<Circle, Square> geometry;
};

class PhysicsEngine {
public:
    PhysicsEngine(float width, float height);

    void initializeSampleData();
    void updatePhysics(float deltaTime);

    // Read-only getter following C++17 noexcept and modern naming conventions
    [[nodiscard]] const std::vector<PhysicsBody>& bodies() const noexcept { return m_bodies; }

private:
    void handleWallCollisions() noexcept;
    void handleBodyCollisions() noexcept;

    std::vector<PhysicsBody> m_bodies;
    float m_containerWidth;
    float m_containerHeight;
    int m_nextId{0};
};

#endif // ENGINE_H
