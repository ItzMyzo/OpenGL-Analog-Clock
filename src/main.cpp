#include <cstring>
#include <sstream>

#include "Renderer.h"

const std::string vertexShader =
    "#version 330 core\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec2 inTexCoord;\n"
    "uniform mat4 projection;\n"
    "out vec2 texCoord;\n"
    "void main() {\n"
    "   gl_Position = projection * position;\n"
    "   texCoord = inTexCoord;\n"
    "}\n";

const std::string fragmentShader =
    "#version 330 core\n"
    "in vec2 texCoord;\n"
    "out vec4 fragColor;\n"
    "uniform vec4 color;\n"
    "uniform float thickness;\n"
    "uniform bool circle;\n"
    "void main() {\n"
    "   if (circle) {\n"
    "       vec2 uv = (texCoord - 0.5) * 2.0;\n" // center uv
    "\n"
    "       float distance = length(uv);\n" // calculate distance to edge
    "       float invDistance = 1.0 - distance;\n" // invert the distance
    "       const float fade = 0.01;\n"
    "\n"
    "       float alpha = invDistance;\n"
    "       alpha = smoothstep(0.0, fade, alpha);\n" // if we are inside the circle set alpha to 1
    "       alpha *= smoothstep(thickness + fade, thickness, invDistance);\n" // inner outline
    "\n"
    "       fragColor = vec4(color.rgb, color.a * alpha);\n"
    "       return;\n"
    "   }\n"
    "   fragColor = color;\n"
    "}\n";

int main(const int argc, const char** argv) {
    bool useOldApi = false;

    if (argc >= 2 && strcmp(argv[1], "-noshader") == 0) {
        useOldApi = true;
    }

    struct tm* time = localTime();
    printf("Current Date and Time: %i/%i/%i %i:%i:%i:%li\n", time->tm_mday, time->tm_mon, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, currentTime<milliseconds>() % 1000);

    int width = 1280;
    int height = 720;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "Error while setting up GLFW" << std::endl;
        return -1;
    }

    printf("GLFW Version: %s\n", glfwGetVersionString());

    // vsync
    glfwSwapInterval(1);

    // Anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL Clock", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    const GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Error while setting up GLEW: " << glewGetErrorString(glewStatus) << std::endl;
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // background
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);

    // alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // generate vao, vbo and create Shader
    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    const unsigned int shader = !useOldApi ? createShader(vertexShader, fragmentShader) : 0;
    glUseProgram(shader);

    double fps = 0;
    int frames = 0;
    double lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        frames++;
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;

        if (delta >= 1.0) {
            fps = frames / delta;
            frames = 0;
            lastTime = currentTime;
        }

        std::stringstream ss;
        ss << "OpenGL Clock W: " << width << " H: " << height << " @" << fps << " fps";
        glfwSetWindowTitle(window, ss.str().c_str());

        updateSize(window, &width, &height, shader);
        
        /* Clear previous frames */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */
        renderClock(width, height, 5.0f, 5.0f, shader, vao, vbo);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        while (GLenum error = glGetError() != GL_NO_ERROR) {
            printf("OpenGL Error: %u\n", error);
        }
    }

    glUseProgram(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
}