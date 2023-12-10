#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Conversion.h"
#include "Time.h"
#include <iostream>
#include <vector>

void renderClock(int width, int height, float outlineWidth, float handWidth, unsigned int shader, unsigned int vao, unsigned int vbo);

void updateSize(GLFWwindow* window, int* width, int* height, unsigned int shader);

void drawClockHand(float x, float y, float radius, float percent, unsigned int vao, unsigned int vbo);

void drawShaderCircle(float centerX, float centerY, float radius, float thickness, unsigned int shader, unsigned int vao, unsigned int vbo);
void drawCircle(float x, float y, float radius, bool filled, unsigned int vao, unsigned int vbo, float lineWidth = 1, int startDegree = 0, int endDegree = 360);
void drawRect(float startX, float startY, float endX, float endY, unsigned int vao, unsigned int vbo);
void drawLine(float startX, float startY, float endX, float endY, unsigned int vao, unsigned int vbo);

void setColor(unsigned int shader, glm::vec4 color);
void setColor(unsigned int shader, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShader(const std::string& vertexSource, const std::string& fragmentSource);