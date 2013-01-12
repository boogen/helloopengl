#include "include/GL/glew.h"
#include "include/GL/glfw.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <string>


GLuint program;

char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}


void checkCompilationStatus(GLuint s) {
    GLint status = 0;

    glGetShaderiv(s, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        int infologLength = 0;
        int charsWritten  = 0;

        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &infologLength);

        if (infologLength > 0)
        {
            GLchar* infoLog = (GLchar *)malloc(infologLength);
            if (infoLog == NULL)
            {
                printf( "ERROR: Could not allocate InfoLog buffer");
                exit(1);
            }
            glGetShaderInfoLog(s, infologLength, &charsWritten, infoLog);
            printf( "Shader InfoLog:\n%s", infoLog );
            free(infoLog);
        }
    }

}


void setShaders() {
    GLuint v, f;
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
    checkCompilationStatus(v);
	glCompileShader(f);
    checkCompilationStatus(f);

	program = glCreateProgram();
	glAttachShader(program,v);
	glAttachShader(program,f);

    GLuint error;
	glLinkProgram(program);
	glUseProgram(program);
}


int main(int argc, char* argv[]) {

    glfwInit();

    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
    glViewport(0, 0, 800, 600);
    glfwSetWindowTitle("VBO");

    glewExperimental = GL_TRUE;
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(result) << std::endl;
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    std::cout << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    setShaders();

    GLfloat vertices[] = {
        0.0f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f
    };

    GLfloat colors[] = {
        1.0f, 0.f, 0.f,
        0.f, 1.0f, 0.f,
        0.f, 0.f, 1.0f
    };


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexbuffer;
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint pos = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint colorbuffer;
    
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint col = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(col);
    glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    while (glfwGetWindowParam(GLFW_OPENED)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers();        
        glfwSleep(0.001);

   }
    
}
