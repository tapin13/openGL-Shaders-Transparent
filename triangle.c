/* 
 * File:   triangle.c
 * Author: tapin13
 *
 * Created on April 30, 2017, 11:02 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "shader_utils.h"

GLuint program;
GLint attribute_coord2d;

int init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    
    GLuint vs, fs;
    if((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER)) == 0) {
        return 0;
    }
    if((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) {
        return 0;
    }
    
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if(!compile_ok) {
        fprintf(stderr, "Error in fragment shader\n");
        return 0;
    }
    
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if(!link_ok) {
        fprintf(stderr, "Error in linking program\n");
        return 0;
    }
    
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if(attribute_coord2d == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }
    
    return 1;    
}

void onDisplay() {
    //Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_verteces[] = {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8
    };
    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, triangle_verteces);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);
    
    glutSwapBuffers();    
}

void free_resources() {
    glDeleteProgram(program);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Triangle");
    
    // probable this part can be skip
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        return EXIT_FAILURE;
    }
    
    if(init_resources() == 1) {
        glutDisplayFunc(onDisplay);
        glutMainLoop();
    }
    
    free_resources();    
    return (EXIT_SUCCESS);
}

