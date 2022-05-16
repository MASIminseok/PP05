
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
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL); /*�����츦 ����ڴ�.*/
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); /*�޼��� �ڵ鷯�ۼ�*/

    while (!glfwWindowShouldClose(window)) /*������ â�� �ٲ��ִ�*/
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;


        glClearColor(0.3f, 0.3f, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

      /*glBegin(GL_POINT); //���� ������� �׸��°� �� �ϳ��̴�!(�� �������� GL_POINTS�� �����)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glEnd(); */

      /*glBegin(GL_LINES);  //�������
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();*/

        /*glBegin(GL_TRIANGLES);  //�ﰢ�������
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();*/

        /*glBegin(GL_LINES);  //�������
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd(); 
        glFlush(); //�������� ���°� ���� �ڵ尡 �� �����ϰ� �� ���� */
        
        
        glBegin(GL_TRIANGLES);  /*�÷��̾���*/
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.1f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(0.1f, 0.1f);
        glVertex2f(0.0f, 0.1f);
        glEnd();
        
        

        glBegin(GL_TRIANGLES);  /*�������*/
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.7f, 0.0f);
        glVertex2f(0.8f, 0.1f);
        glVertex2f(0.9f, 0.0f);
        glEnd();

        glFlush();

        glfwSwapBuffers(window);
        glfwPollEvents();/*�޼����� ���ֳ� Ȯ��*/
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}