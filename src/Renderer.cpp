#include "Renderer.h"

void renderClock(int width, int height, float outlineWidth, float handWidth, unsigned int shader, unsigned int vao, unsigned int vbo) {
    const float radius = 250.0f;

    const float x = width * 0.5f;
    const float y = height * 0.5f;

    setColor(shader, glm::vec4(1.0, 1.0, 1.0, 1.0));

    if (shader != 0) {
        drawShaderCircle(x, y, radius, 0.025, shader, vao, vbo);
        drawShaderCircle(x, y, 0.5 * handWidth, 1, shader, vao, vbo);
    } else {
        drawCircle(x, y, radius, false, outlineWidth);
        drawCircle(x, y,  0.5 * handWidth, true);
    }

    glLineWidth(handWidth);

    drawLine(x, y + radius - 20, x, y + radius - 2.5f, vao, vbo);
    drawLine(x, y - radius + 20, x, y - radius + 2.5f, vao, vbo);

    drawLine(x + radius - 20, y, x + radius - 2.5f, y, vao, vbo);
    drawLine(x - radius + 20, y, x - radius + 2.5f, y, vao, vbo);

    const struct tm* time = localTime();
    const double fractionalMillis = currentTime<milliseconds>() % 1000 / 1000.0;
    const double secondsPercent = (time->tm_sec + fractionalMillis) / 60.0;
    const double minutesPercent = (time->tm_min + secondsPercent) / 60.0;
    const double hoursPercent = (to12Hour(time->tm_hour) + minutesPercent) / 12.0;

    drawClockHand(x, y, radius * 0.8, secondsPercent, vao, vbo);
    drawClockHand(x, y, radius * 0.75, minutesPercent, vao, vbo);
    drawClockHand(x, y, radius * 0.5, hoursPercent, vao, vbo);
}

void drawClockHand(float x, float y, float radius, float percent, unsigned int vao, unsigned int vbo) {
    const double radians = percent * (2 * M_PI);

    drawLine(
        x, y,
        x + radius * sin(radians), y + radius * cos(radians),
        vao, vbo
    );
}

void drawShaderCircle(float centerX, float centerY, float radius, float thickness, unsigned int shader, unsigned int vao, unsigned int vbo) {
    const float startX = centerX - radius; // line width
    const float startY = centerY - radius;
    const float endX = centerX + radius;
    const float endY = centerY + radius;

    const unsigned int circleU = glGetUniformLocation(shader, "circle");
    const unsigned int thicknessU = glGetUniformLocation(shader, "thickness");

    glUniform1i(circleU, GL_TRUE);
    glUniform1f(thicknessU, thickness);
    drawRect(startX, startY, endX, endY, vao, vbo);
    glUniform1i(circleU, GL_FALSE);
}

void updateSize(GLFWwindow* window, int* width, int* height, unsigned int shader) {
    glfwGetFramebufferSize(window, width, height);

    glViewport(0, 0, *width, *height);

    if (shader != 0) {
        glm::mat4 projectionMatrix = glm::ortho<float>(0, *width, 0, *height, -1, 1);

        unsigned int matrixID = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &projectionMatrix[0][0]);
    } else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, *width, 0, *height, -1, 1);
    }
}

void drawCircle(float x, float y, float radius, bool filled, float lineWidth, int startDegree, int endDegree) {
    glLineWidth(lineWidth);
    glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    for (int degree = startDegree; degree <= endDegree; degree++) {
        double radians = toRadians(degree);
        glVertex2d(radius * sin(radians) + x, radius * cos(radians) + y);
    }
    glEnd();
}

void drawRect(float startX, float startY, float endX, float endY, unsigned int vao, unsigned int vbo) {
    const float vertices[] = {
        startX, startY, 0, 0,
        startX, endY, 0, 1,
        endX, endY,1, 1,
        endX, startY, 1, 0
    };

    if (vao == 0) glGenVertexArrays(1, &vao);
    if (vbo == 0) glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawLine(float startX, float startY, float endX, float endY, unsigned int vao, unsigned int vbo) {
    const float vertices[] = {
        startX, startY,
        endX, endY
    };

    if (vao == 0) glGenVertexArrays(1, &vao);
    if (vbo == 0) glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setColor(unsigned int shader, glm::vec4 color) {
    if (shader == 0) {
        glColor4f(color.r, color.g, color.b, color.a);
        return;
    }
    
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
}

void setColor(unsigned int shader, unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    setColor(shader, glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f));
}

unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* error = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, error);
        std::cout << "Failed to compile shader! Error: " << error << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}