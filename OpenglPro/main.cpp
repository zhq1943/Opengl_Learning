#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



float vertices[]={
	0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	0.0f,0.0f,0.0f
};

float vertices1[]={
	-0.5f,0.5f,0.0f,
	0.0f,0.0f,0.0f,
	-0.5f,-0.5f,0.0f
};

unsigned int indices[]={
	0,1,3,
	1,2,3
};

char* vertex_shader = "#version 330 core \n\
					  layout (location = 0) in vec3 aPos; \n\
					  void main() \n\
					  {\
					     gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0f); \
                      }";
char* fragment_shader = "#version 330 core \n\
					  out vec4 FragColor;\n \
					  void main() \n\
					  {\
					      FragColor=vec4(1.0f,0.5,0.2f,1.0f); \
					  }";

char* fragment_shader2 = "#version 330 core \n\
						out vec4 FragColor;\n \
						void main() \n\
						{\
						FragColor=vec4(1.0f,1.0,0.0f,1.0f); \
						}";

unsigned int Drawstyle = GL_TRIANGLES;
unsigned int keystate = 0;

void framebuffer_size_callback(GLFWwindow* wd,int width,int height)
{
	glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}else if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS && keystate == 0)
	{
		if (Drawstyle == GL_TRIANGLES)
		{
			Drawstyle = GL_LINE_LOOP;
		}else
		{
			Drawstyle = GL_TRIANGLES;
		}

		keystate = 1;
	}else if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_RELEASE && keystate == 1)
	{
		keystate = 0;
	}
}

bool shaderError(unsigned int id,unsigned int state)
{
	int succ;
	char infolog[512];
	glGetShaderiv(id,state,&succ);
	if (!succ)
	{
		glGetShaderInfoLog(id,512,NULL,infolog);
		std::cout <<infolog<<std::endl;
		return false;
	}

	return true;
}

int main(int _Argc,char** _argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpengl",NULL,NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"Failed to initialize GLAD"<<std::endl;
		return -1;
	}

	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);



	//unsigned int ebo;
	//glGenBuffers(1,&ebo);

	unsigned int VAO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	unsigned int Vbo;
	glGenBuffers(1,&Vbo);
	glBindBuffer(GL_ARRAY_BUFFER,Vbo);

	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	unsigned int VAO1;
	glGenVertexArrays(1,&VAO1);
	glBindVertexArray(VAO1);

	unsigned int Vbo1;
	glGenBuffers(1,&Vbo1);
	glBindBuffer(GL_ARRAY_BUFFER,Vbo1);

	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader,1,(&vertex_shader),NULL);
	glCompileShader(vertexShader);

	if (shaderError(vertexShader,GL_COMPILE_STATUS) == false)
	{
		return -1;
	}

    unsigned int fragmentshader;
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader,1,&fragment_shader,NULL);
	glCompileShader(fragmentshader);

	if (shaderError(fragmentshader,GL_COMPILE_STATUS) == false)
	{
		return -1;
	}

	unsigned int fragmentshader2;
	fragmentshader2 = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader2,1,&fragment_shader2,NULL);
	glCompileShader(fragmentshader2);

	if (shaderError(fragmentshader2,GL_COMPILE_STATUS) == false)
	{
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentshader);
	glLinkProgram(shaderProgram);

	if (shaderError(shaderProgram,GL_LINK_STATUS) == false)
	{
		return -1;
	}

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2,vertexShader);
	glAttachShader(shaderProgram2,fragmentshader2);
	glLinkProgram(shaderProgram2);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		//glDrawElements(Drawstyle,6,GL_UNSIGNED_INT,0);
		glDrawArrays(GL_TRIANGLES,0,3);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES,0,3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentshader);
	glfwTerminate();
	return 0;
}