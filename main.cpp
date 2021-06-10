#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <GL/gl.h>
#include<algorithm>
#include<vector>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
using namespace std;

const int WIDTH=800, HEIGHT=600;
const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                   "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     //"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "color = vec4(1.0f,1.0f,8*6*1.0f/255, 1.0f);\n"

                                     "}\n\0";
struct Point
{
    float x;
    float y;
    Point(float _x, float _y):x(_x), y(_y){}
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
    AETNode(const NETNode& t):x(t.x_min), y_max(t.y_max)
    {
        if(isinf(t.k))
            dx = 0.0f;
        else
            dx = 1/t.k;
    }
    bool operator < (const AETNode& another_node) const
    {
        return x < another_node.x || (x == another_node.x && 1.0/dx < 1.0/another_node.dx);
    }
};

int ATEcmp(const AETNode& a, const AETNode& b)
{
    return a.x < b.x || (a.x == b.x && 1.0/a.dx < 1.0/b.dx);
}
vector<float> print_pic(const vector<Edge>& edge_set)
{
    vector<float> fill_vector;
    int y_min = numeric_limits<int>::max();
    int y_max = numeric_limits<int>::min();


    for(const auto & i : edge_set) {
        y_min = min(y_min, (int)i.start.y);
        y_max = max(y_max, (int)i.end.y);
    }
    vector<vector<NETNode>> NET(y_max - y_min + 1);
    for(const auto & i : edge_set) {
        NETNode new_Node(i);
        NET[i.start.y - y_min].push_back(new_Node);
    }
    vector<AETNode> AET;
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
            AET.push_back(AETNode(NET[i - y_min][j]));
        sort(AET.begin(), AET.end(), ATEcmp);
        if(AET.size() % 2 == 1)
        {
            cout << "why";
        }

        for(int j = 0; j < AET.size(); j=j+2)
        {
//            if(AET.size() > 2)
//                cout << "sjadlk";
            for(int k = (int)AET[j].x +1; k<= (int )AET[j+1].x; k++)
            {
                float px = k * 1.0f / WIDTH;
                float py = i * 1.0f / HEIGHT;
                fill_vector.push_back(px);
                fill_vector.push_back(py);
                fill_vector.push_back(0.0f);
            }
        }
        if(AET.size() % 2 == 1)
        {
            cout << "why";
        }

    }
    return fill_vector;



}
vector<Edge> create_edge(const vector<Point>& point_set)
{
    vector<Edge> edge_set;
    for(int i = 0; i < point_set.size(); i++)
    {
        int next_index = i+1;
        if(i == point_set.size() - 1)
            next_index = 0;
        if(point_set[i].y == point_set[next_index].y)
            continue;
        edge_set.push_back(Edge(point_set[i], point_set[next_index]));
    }
    return edge_set;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    vector<Point> point_set;
   vector<Edge> edge_set = {Edge(Point(0,0), Point(0,300)), Edge(Point(300,300), Point(300, 0)),
                            Edge(Point(100,100), Point(100,250)), Edge(Point(250,250), Point(250, 100))};

    //vector<Edge> edge_set = {Edge(Point(100,100), Point(100,250)), Edge(Point(100,250), Point(250,250)), Edge(Point(250,250), Point(250, 100))};
    //vector<Edge> edge_set = {Edge(Point(0,0), Point(0,300)), Edge(Point(0,300), Point(300,300)), Edge(Point(300,300), Point(300, 0))};
    //    cout << "点的个数:" << endl;
//    int point_num;
//    cin >> point_num;
//    cout <<"请输入点："  << endl;
//    for(int i = 0; i < point_num; i++)
//    {
//        float x, y;
//        cin >> x>> y;
//        point_set.push_back(Point(x, y));
//    }
//    vector<Edge> edge_set = create_edge(point_set);
    vector<float> fill = print_pic(edge_set);
    GLfloat* vertices = fill.data();
    int size = fill.size();


    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,size*sizeof(vertices)/2, vertices, GL_STATIC_DRAW);

    GLint success;
    GLchar infoLog[512];
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);

    //glPointSize(5);




    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        glClearColor(0 * 1.0f/255, 96 * 1.0f/255, 100 * 1.0f / 255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, size/2);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}
