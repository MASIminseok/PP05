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
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;// 16진수에서 8개는 칠하고 8개는 칠하지않고
            checkImage[i][j][0] = (GLubyte)c;   //rgba 값을 받아줌
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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // packing 안쓰고 막쓸꺼다

    glGenTextures(1, &texName);//1번 텍스쳐에 저장할거다 총8개까지
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat 그림을 반복해서 넣겠다 wrap 이 영역을 벗어났을때 이렇게 할것이다
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, //mag_filter 깨졌을때 어떻게함 안써도됨 대신 그림 화질구지
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
        checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        checkImage);
}

//비트맵 헤더를 한 묶음으로 다시 구조체로 묶었어요. 함수처리를 편하게 하려구요.
typedef struct tagBITMAPHEADER {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD hRGB[256];
}BITMAPHEADER;

BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, const char* filename)
{
    FILE* fp = fopen(filename, "rb");	//파일을 이진읽기모드로 열기
    if (fp == NULL)
    {
        printf("파일로딩에 실패했습니다.\n");	//fopen에 실패하면 NULL값을 리턴
        return NULL;
    }
    else
    {//비트맵 표준
        fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//비트맵파일헤더 읽기
        fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//비트맵인포헤더 읽기
        fread(&bitmapHeader->hRGB, sizeof(RGBQUAD), 256, fp);	//색상팔렛트 읽기

        int imgSizeTemp = bitmapHeader->bi.biWidth * bitmapHeader->bi.biHeight;	//이미지 사이즈 계산
        *imgSize = imgSizeTemp;	// 이미지 사이즈를 상위 변수에 할당

        BYTE* image = (BYTE*)malloc(sizeof(BYTE) * imgSizeTemp);	//이미지크기만큼 메모리할당
        fread(image, sizeof(BYTE), imgSizeTemp, fp);//이미지 크기만큼 파일에서 읽어오기
        fclose(fp);
        return image;
    }
}

void ChangeRGB(RGBQUAD* originalhRGB, RGBQUAD* outputhRGB, int maniplRed, int maniplBlue, int maniplGreen)
{
    //클리핑 처리후 outputhRGB에 그 값을 할당
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
    FILE* fp = fopen(filename, "wb");//파일을 이진쓰기모드로 열기
    
    fwrite(&outputHeader.bf, sizeof(BITMAPFILEHEADER), 1, fp);//비트맵파일헤더쓰기
    fwrite(&outputHeader.bi, sizeof(BITMAPINFOHEADER), 1, fp);//비트맵인포헤더쓰기
    fwrite(&outputHeader.hRGB, sizeof(RGBQUAD), 256, fp);//색상팔렛트 쓰기
    fwrite(output, sizeof(BYTE), imgSize, fp);//이미지인덱스정보 쓰기
    fclose(fp);
}



int LoadBitmap()
{
    BITMAPHEADER originalHeader;	//비트맵의 헤더부분을 파일에서 읽어 저장할 구조체
    BITMAPHEADER outputHeader;	//변형을 가한 헤더부분을 저장할 구조체
    int imgSize;			//이미지의 크기를 저장할 변수
    BYTE* image = LoadBitmapFile(&originalHeader, &imgSize, "lena_gray.bmp"); //비트맵파일을 읽어 화소정보를 저장
    if (image == NULL) return 0;        //파일 읽기에 실패하면 프로그램 종료
    BYTE* output = (BYTE*)malloc(sizeof(BYTE) * imgSize);	//결과값을 저장할 포인터 선언 및 메모리 할당
    outputHeader = originalHeader;				//헤더정보를 출력헤더정보에 할당

    /*요기에다가 영상을 처리할 코드가 들어가면 됩니다.*/
       //여기부터 밝기를 50만큼 증가시킨 파일 생성
    ChangeRGB(originalHeader.hRGB, outputHeader.hRGB, 50, 50, 50);
    WriteBitmapFile(outputHeader, output, imgSize, "output1.bmp");

    //화소정보 복사
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

    BITMAPHEADER originalHeader;	//비트맵의 헤더부분을 파일에서 읽어 저장할 구조체
    
    int imgSize;			//이미지의 크기를 저장할 변수
    BYTE* image = LoadBitmapFile(&originalHeader, &imgSize, "lena_gray.bmp"); //비트맵파일을 읽어 화소정보를 저장
    if (image == NULL) return 0;        //파일 읽기에 실패하면 프로그램 종료

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