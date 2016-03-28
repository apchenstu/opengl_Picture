#include<glew.h>
#include<glut.h>
#include<stdio.h>
#include<glaux.h>
#include"lodepng.h"
#include<glext.h>

unsigned int width = 500;
unsigned int height = 500;
unsigned char* TextureImage;
GLuint Texture,Texture_result;
GLuint g_framebuffer, g_depthbuffer;	

float QuadVBO[8];
float QuadUV[8];
float QuadColor[12];
GLuint uiVBO[3];
GLuint uiVAO;


bool LoadGLTextures()
{
	bool status = FALSE;
	unsigned char *dst;
	

	if (!lodepng_decode32_file(&TextureImage, &width, &height, "../1.png")){
		status = true;

		dst = (unsigned char *)malloc(sizeof(char)*width*height * 4);
		dst[0] = 0;

			glGenTextures(1, &Texture);
			glBindTexture(GL_TEXTURE_2D, Texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage);

			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);
			float t = dst[0]; printf("%f ", t);
			unsigned error = lodepng_encode32_file("dst.png", dst, width, height);


	}

	if (TextureImage)
		free(TextureImage);

	return status;
}



void InitVBO()
{
	float * data;
	data = (float *)malloc(sizeof(float)*width*height*4);

	QuadVBO[0] = -1; QuadVBO[1] = 1;
	QuadVBO[2] = -1; QuadVBO[3] = -1;
	QuadVBO[4] = 1; QuadVBO[5] = -1;
	QuadVBO[6] = 1; QuadVBO[7] = 1;

	QuadColor[0] = 1;

	QuadUV[0] = 0; QuadUV[1] = 0;
	QuadUV[0] = 0; QuadUV[1] = 1;
	QuadUV[0] = 1; QuadUV[1] = 1;
	QuadUV[0] = 1; QuadUV[1] = 0;

	glGenBuffers(3, uiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(float), QuadVBO,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(float), QuadColor, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(float), QuadUV, GL_STATIC_DRAW);

	glGenVertexArrays(1,&uiVAO);
	glBindVertexArray(uiVAO);


	glGenFramebuffersEXT(1, &g_framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_EXT,Texture,0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	//glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, data);


	////分配zbuffer给FBO 使用 
 //    glGenRenderbuffersEXT(1,&g_depthbuffer);
 //    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,g_depthbuffer);
 //    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24,width,height);
 //    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,g_depthbuffer);
 //    GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

	// switch (status){
	// case GL_FRAMEBUFFER_COMPLETE_EXT:
	//	 //    MessageBox(NULL,"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_OK|MB_ICONEXCLAMATION);
	//	 break;

	// case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	//	 //   MessageBox(NULL,"GL_FRAMEBUFFER_UNSUPPORTED_EXT!","ERROR",MB_OK|MB_ICONEXCLAMATION);
	//	 exit(0);
	//	 break;
	// }
}

bool InitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);

	if (!LoadGLTextures())
		return false;
	return true;
}

void SaveVBO()
{
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);
}

void RenderScene()
{

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer);
	glPushAttrib(GL_VIEWPORT_BIT);

	glViewport(0,0,width,height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_SMOOTH);
	glLoadIdentity();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);


	 glPopAttrib();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_SMOOTH);
	glLoadIdentity();

	glBegin(GL_QUADS);
	{
		//glVertex2f(-1, 1); glColor3f(0.0, 0.0, 1.0);
		//glVertex2f(-1, -1); glColor3f(1.0, 0.0, 1.0);
		//glVertex2f(1, -1); glColor3f(0.0, 1.0, 1.0);
		//glVertex2f(1, 1); glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0,0); glVertex2f(-1, 1);
		glTexCoord2d(0, 1); glVertex2f(-1, -1);
		glTexCoord2d(1, 1); glVertex2f(1, -1);
		glTexCoord2d(1, 0); glVertex2f(1, 1);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glFlush();

	glutSwapBuffers();
}

void RCSet()
{
	glClearColor(0.0,1.0,0.0,1.0);
	gluOrtho2D(-1.0,1.0,-1.0,1.0);
}

void init(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow("cap");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	InitGL();
	InitVBO();
//	SaveVBO();
	
}

int main(int argc, char *argv[])
{

	init(argc, argv);

	glutDisplayFunc(RenderScene);
	RCSet();
	glutMainLoop();
	return 0;
}
