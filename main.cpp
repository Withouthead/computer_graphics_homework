#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<algorithm>
#include<vector>
#include <list>
using namespace std;
struct Point
{
    float x;
    float y;
    bool operator < (const Point& another_point) const
    {
        return y < another_point.y || (y == another_point.y && x < another_point.x);
    }
    bool operator > (const Point& another_point) const
    {
        return y > another_point.y || (y == another_point.y && x > another_point.x);
    }

    bool operator == (const Point& another_point) const
    {
        return y == another_point.y && x == another_point.x;
    }

};
struct Edge
{
    Point start;
    Point end;
    float k;
    Edge(const Point& _start, const Point& _end):start(_start), end(_end)
    {
        if(start > end)
            swap(start, end);
        k = (start.y - end.y) / (start.x - end.x);
    }
};
struct NETNode
{
    float y_max;
    float x_min;
    float k;

    NETNode(const Edge& edge)
    {
        y_max = edge.end.y;
        x_min = edge.start.x;
        k = edge.k;
    }
    bool operator < (const NETNode& another_edge) const
    {
        return x_min == another_edge.x_min && k < another_edge.k;
    }
};
struct AETNode
{
    float x;
    float dx;
    float y_max;
    AETNode(float _x, float _dx, float _y_max):x(_x),dx(_dx), y_max(_y_max){}
    AETNode(const NETNode& t):x(t.x_min), dx(1.0/t.k), y_max(t.y_max){}
    bool operator < (const AETNode& another_node) const
    {
        return x < another_node.x || (x == another_node.x && 1.0/dx < 1.0/another_node.dx);
    }
};

void print_pic(const vector<Edge>& edge_set)
{
    int y_min = edge_set[0].start.y;
    int y_max = edge_set.back().end.y;
    vector<vector<NETNode>> NET(y_max - y_min + 1);
    for(const auto & i : edge_set)
        NET[i.start.y].push_back(NETNode(i));
    list<AETNode> AET;
    for(int i = y_min; i <= y_max; i++)
    {
        auto ite = AET.begin();
        while(ite != AET.end())
        {
            ite->x += ite->dx;
            if(ite->y_max == i)
                ite = AET.erase(ite);
            else
                ite ++;
        }

        for(int j = 0; j < NET[i - y_min].size(); j++)
        {
            AET.push_back(AETNode(NET[i - y_min][j]));
        }
        sort(AET.begin(), AET.end());

    }


}


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    return 0;
}
