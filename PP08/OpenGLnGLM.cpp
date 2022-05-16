
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>


#pragma comment(lib,"opengl32")

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL); /*윈도우를 만들겠다.*/
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); /*메세지 핸들러작성*/

    while (!glfwWindowShouldClose(window)) /*윈도우 창을 바꿔주는*/
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;


        glClearColor(0.3f, 0.3f, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

      /*glBegin(GL_POINT); //내가 엔드까지 그리는건 점 하나이다!(점 여러개는 GL_POINTS를 쓰면됨)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glEnd(); */

      /*glBegin(GL_LINES);  //선만들기
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();*/

        /*glBegin(GL_TRIANGLES);  //삼각형만들기
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();*/

        /*glBegin(GL_LINES);  //선만들기
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd(); 
        glFlush(); //마지막에 쓰는게 좋음 코드가 더 안전하게 됨 ㅇㅇ */
        
        
        glBegin(GL_TRIANGLES);  /*플레이어만들기*/
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.1f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(0.1f, 0.1f);
        glVertex2f(0.0f, 0.1f);
        glEnd();
        
        

        glBegin(GL_TRIANGLES);  /*적만들기*/
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.7f, 0.0f);
        glVertex2f(0.8f, 0.1f);
        glVertex2f(0.9f, 0.0f);
        glEnd();

        glFlush();

        glfwSwapBuffers(window);
        glfwPollEvents();/*메세지가 들어가있나 확인*/
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}