#define _CRT_SECURE_NO_WARNINGS

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


#pragma comment(lib, "OpenGL32")


/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;


void makeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;// 16�������� 8���� ĥ�ϰ� 8���� ĥ�����ʰ�
            checkImage[i][j][0] = (GLubyte)c;   //rgba ���� �޾���
            checkImage[i][j][1] = (GLubyte)c;   //      "
            checkImage[i][j][2] = (GLubyte)c;   //      "
            checkImage[i][j][3] = (GLubyte)255; //      "
        }
    }
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glShadeModel(GL_FLAT);  //GL_SMOOTH
    //glEnable(GL_DEPTH_TEST);

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // packing �Ⱦ��� ��������

    glGenTextures(1, &texName);//1�� �ؽ��Ŀ� �����ҰŴ� ��8������
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat �׸��� �ݺ��ؼ� �ְڴ� wrap �� ������ ������� �̷��� �Ұ��̴�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, //mag_filter �������� ����� �Ƚᵵ�� ��� �׸� ȭ������
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
        checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        checkImage);
}

//��Ʈ�� ����� �� �������� �ٽ� ����ü�� �������. �Լ�ó���� ���ϰ� �Ϸ�����.
typedef struct tagBITMAPHEADER {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD hRGB[256];
}BITMAPHEADER;

BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, const char* filename)
{
    FILE* fp = fopen(filename, "rb");	//������ �����б���� ����
    if (fp == NULL)
    {
        printf("���Ϸε��� �����߽��ϴ�.\n");	//fopen�� �����ϸ� NULL���� ����
        return NULL;
    }
    else
    {//��Ʈ�� ǥ��
        fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//��Ʈ��������� �б�
        fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//��Ʈ��������� �б�
        fread(&bitmapHeader->hRGB, sizeof(RGBQUAD), 256, fp);	//�����ȷ�Ʈ �б�

        int imgSizeTemp = bitmapHeader->bi.biWidth * bitmapHeader->bi.biHeight;	//�̹��� ������ ���
        *imgSize = imgSizeTemp;	// �̹��� ����� ���� ������ �Ҵ�

        BYTE* image = (BYTE*)malloc(sizeof(BYTE) * imgSizeTemp);	//�̹���ũ�⸸ŭ �޸��Ҵ�
        fread(image, sizeof(BYTE), imgSizeTemp, fp);//�̹��� ũ�⸸ŭ ���Ͽ��� �о����
        fclose(fp);
        return image;
    }
}

void ChangeRGB(RGBQUAD* originalhRGB, RGBQUAD* outputhRGB, int maniplRed, int maniplBlue, int maniplGreen)
{
    //Ŭ���� ó���� outputhRGB�� �� ���� �Ҵ�
    for (int i = 0; i < 256; i++)
    {
        if (originalhRGB[i].rgbRed + maniplRed > 255)
            outputhRGB[i].rgbRed = 255;
        else if (originalhRGB[i].rgbRed + maniplRed < 0)
            outputhRGB[i].rgbRed = 0;
        else
            outputhRGB[i].rgbRed = originalhRGB[i].rgbRed + maniplRed;

        if (originalhRGB[i].rgbBlue + maniplBlue > 255)
            outputhRGB[i].rgbBlue = 255;
        else if (originalhRGB[i].rgbBlue + maniplBlue < 0)
            outputhRGB[i].rgbBlue = 0;
        else
            outputhRGB[i].rgbBlue = originalhRGB[i].rgbBlue + maniplBlue;

        if (originalhRGB[i].rgbGreen + maniplGreen > 255)
            outputhRGB[i].rgbGreen = 255;
        else if (originalhRGB[i].rgbGreen + maniplGreen < 0)
            outputhRGB[i].rgbGreen = 0;
        else
            outputhRGB[i].rgbGreen = originalhRGB[i].rgbGreen + maniplGreen;
    }

}


void WriteBitmapFile(BITMAPHEADER outputHeader, BYTE* output, int imgSize, const char* filename)
{
    FILE* fp = fopen(filename, "wb");//������ ����������� ����
    
    fwrite(&outputHeader.bf, sizeof(BITMAPFILEHEADER), 1, fp);//��Ʈ�������������
    fwrite(&outputHeader.bi, sizeof(BITMAPINFOHEADER), 1, fp);//��Ʈ�������������
    fwrite(&outputHeader.hRGB, sizeof(RGBQUAD), 256, fp);//�����ȷ�Ʈ ����
    fwrite(output, sizeof(BYTE), imgSize, fp);//�̹����ε������� ����
    fclose(fp);
}



int LoadBitmap()
{
    BITMAPHEADER originalHeader;	//��Ʈ���� ����κ��� ���Ͽ��� �о� ������ ����ü
    BITMAPHEADER outputHeader;	//������ ���� ����κ��� ������ ����ü
    int imgSize;			//�̹����� ũ�⸦ ������ ����
    BYTE* image = LoadBitmapFile(&originalHeader, &imgSize, "lena_gray.bmp"); //��Ʈ�������� �о� ȭ�������� ����
    if (image == NULL) return 0;        //���� �б⿡ �����ϸ� ���α׷� ����
    BYTE* output = (BYTE*)malloc(sizeof(BYTE) * imgSize);	//������� ������ ������ ���� �� �޸� �Ҵ�
    outputHeader = originalHeader;				//��������� ������������ �Ҵ�

    /*��⿡�ٰ� ������ ó���� �ڵ尡 ���� �˴ϴ�.*/
       //������� ��⸦ 50��ŭ ������Ų ���� ����
    ChangeRGB(originalHeader.hRGB, outputHeader.hRGB, 50, 50, 50);
    WriteBitmapFile(outputHeader, output, imgSize, "output1.bmp");

    //ȭ������ ����
    for (int i = 0; i < imgSize; i++)
    {
        output[i] = image[i];
    }

    free(image);
    free(output);

    return 0;
}



static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}


int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BITMAPHEADER originalHeader;	//��Ʈ���� ����κ��� ���Ͽ��� �о� ������ ����ü
    
    int imgSize;			//�̹����� ũ�⸦ ������ ����
    BYTE* image = LoadBitmapFile(&originalHeader, &imgSize, "lena_gray.bmp"); //��Ʈ�������� �о� ȭ�������� ����
    if (image == NULL) return 0;        //���� �б⿡ �����ϸ� ���α׷� ����

    init();

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glClearColor(.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

     


        //p->Render();
        //e->Render();


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texName);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

        glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
        glEnd();
        glFlush();
        glDisable(GL_TEXTURE_2D);


        /*
        glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);

        glVertex2f(1.0f, -1.0f);

        glVertex2f(-1.0f, -1.0f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);

        glVertex2f(1.0f, -1.0f);

        glVertex2f(-1.0f, -1.0f);
        glEnd();
        */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
  
    exit(EXIT_SUCCESS);
}