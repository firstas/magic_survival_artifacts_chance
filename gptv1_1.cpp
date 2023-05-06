#include "include/GLFW/glfw3.h"
#include <iostream>
#include <cmath>

const int WIDTH = 640;
const int HEIGHT = 480;
const float RADIUS = 0.1f;
const float ELASTICITY = 0.8f;
const float GRAVITY = 9.8f;

struct Ball {
    float x, y;
    float vx, vy;
    float mass;
};

Ball ball1 = { -0.4f, 0.0f, 2.0f, 0.0f, 1.0f };
Ball ball2 = { 0.4f, 0.0f, -2.0f, 0.0f, 0.5f };

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        glVertex2f(x + (radius * cos(i * M_PI / 180.0)),
                   y + (radius * sin(i * M_PI / 180.0)));
    }
    glEnd();
}

void drawBall(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x, y, RADIUS);
}

void updateBall(Ball& ball, float deltaTime) {
    float fx = 0.0f;
    float fy = -GRAVITY * ball.mass;
    ball.vx += fx / ball.mass * deltaTime;
    ball.vy += fy / ball.mass * deltaTime;
    ball.x += ball.vx * deltaTime;
    ball.y += ball.vy * deltaTime;
    if (ball.y - RADIUS < -1 || ball.y + RADIUS > 1) {
        ball.vy = -ball.vy * ELASTICITY;
    }
    if (ball.x + RADIUS > 1 || ball.x - RADIUS < -1) {
        ball.vx = -ball.vx * ELASTICITY;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing Balls", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float deltaTime = glfwGetTime();
        glfwSetTime(0.0);

        updateBall(ball1, deltaTime);
        drawBall(ball1.x, ball1.y);

        updateBall(ball2, deltaTime);
        drawBall(ball2.x, ball2.y);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
