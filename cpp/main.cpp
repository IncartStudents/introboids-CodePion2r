#include "UseImGui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

int W = 0;
int H = 0;

struct Vec2 {
    float x, y;

    Vec2 operator+(const Vec2& other) const {
        return { x + other.x, y + other.y };
    }

    Vec2 operator-(const Vec2& other) const {
        return { x - other.x, y - other.y };
    }

    Vec2 operator*(float scalar) const {
        return { x * scalar, y * scalar };
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vec2 normalized() const {
        float len = length();
        if (len == 0) return { 0, 0 }; // Защита от деления на ноль
        return { x / len, y / len };
    }
};

class Boid {
public:
    Vec2 position;
    Vec2 velocity;
    float max_speed;
    float max_force; // Максимальная сила для изменения направления

    Boid(float x, float y) : max_speed(2.0f), max_force(0.03f) { 
        position = { x, y };
        velocity = { static_cast<float>(rand() % 100) / 100 - 0.5f, static_cast<float>(rand() % 100) / 100 - 0.5f };
    }

    void update(const std::vector<Boid>& boids) {
        Vec2 alignment = computeAlignment(boids);
        Vec2 cohesion = computeCohesion(boids);
        Vec2 separation = computeSeparation(boids);

        // Комбинирование сил
        velocity = velocity + alignment + cohesion + separation;

        // Ограничение скорости
        if (velocity.length() > max_speed) {
            velocity = velocity.normalized() * max_speed;
        }

        // Обновление позиции
        position = position + velocity;

        // Обработка выхода за границы экрана
        wrapAround();
    }

private:
    Vec2 computeAlignment(const std::vector<Boid>& boids) {
        Vec2 steering = { 0, 0 };
        int total = 0;
        for (const Boid& other : boids) {
            float distance = (position - other.position).length();
            if (&other != this && distance < 25 * 2) {
                steering = steering + other.velocity;
                total++;
            }
        }
        if (total > 0) {
            steering = steering * (1.0f / total);
            steering = steering.normalized() * max_speed;
            steering = steering - velocity; // Изменение направления
            if (steering.length() > max_force) {
                steering = steering.normalized() * max_force;
            }
        }
        return steering;
    }

    Vec2 computeCohesion(const std::vector<Boid>& boids) {
        Vec2 steering = { 0, 0 };
        int total = 0;
        for (const Boid& other : boids) {
            float distance = (position - other.position).length();
            if (&other != this && distance < 25) {
                steering = steering + other.position;
                total++;
            }
        }
        if (total > 0) {
            steering = steering * (4.0f / total);
            steering = steering - position; // Движение к среднему
            steering = steering.normalized() * max_speed;
            steering = steering - velocity; // Изменение направления
            if (steering.length() > max_force) {
                steering = steering.normalized() * max_force;
            }
        }
        return steering;
    }

    Vec2 computeSeparation(const std::vector<Boid>& boids) {
        Vec2 steering = { 0, 0 };
        int total = 0;
        for (const Boid& other : boids) {
            float distance = (position - other.position).length();
            if (&other != this && distance < 25) {
                Vec2 diff = position - other.position;
                diff = diff.normalized() * (1.0f / distance); // Увеличение силы при близком расстоянии
                steering = steering + diff;
                total++;
            }
        }
        if (total > 0) {
            steering = steering * (1.0f / total);
            steering = steering.normalized() * max_speed;
            steering = steering - velocity; // Изменение направления
            if (steering.length() > max_force) {
                steering = steering.normalized() * max_force;
            }
        }
        return steering;
    }

    void wrapAround() {
        if (position.x < 0) position.x += W;
        if (position.x > W) position.x -= W;
        if (position.y < 0) position.y += H;
        if (position.y > H) position.y -= H;
    }
};

int main()
{
    // Setup window
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
        throw("Unable to context to OpenGL");

    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);    

    auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    W = mode->width;
    H = mode->height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, W, H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const int NUM_BOIDS = 100;
    std::vector<Boid> boids;

    for (int i = 0; i < NUM_BOIDS; ++i) {
        boids.emplace_back(rand() % W, rand() % H);
    }

while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    for (Boid& boid : boids) {
        boid.update(boids);
    }

    for (const Boid& boid : boids) {
        // Определяем угол направления
        float angle = std::atan2(boid.velocity.y, boid.velocity.x);

        // Размер треугольника
        float size = 10.0f;

        // Вершины треугольника
        Vec2 v1 = { boid.position.x + size * std::cos(angle), boid.position.y + size * std::sin(angle) };
        Vec2 v2 = { boid.position.x + size * std::cos(angle + 2.0f * M_PI / 3.0f), boid.position.y + size * std::sin(angle + 2.0f * M_PI / 3.0f) };
        Vec2 v3 = { boid.position.x + size * std::cos(angle + 4.0f * M_PI / 3.0f), boid.position.y + size * std::sin(angle + 4.0f * M_PI / 3.0f) };

        // Рисуем треугольник
        glBegin(GL_TRIANGLES);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
        glVertex2f(v3.x, v3.y);
        glEnd();
    }
    glfwSwapBuffers(window);
}
    return 0;
}