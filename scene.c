#include <GL/glew.h>
#include <GLES2/gl2.h>

#include <stdlib.h>
#include <stdio.h>

#include "scene.h"
#include "colors.h"

static GLuint program;
static GLint attribute_coord2d;

int init_resources() {

  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
  fprintf(stderr, KRED"INIT SHADER\n"KRST);
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  fprintf(stderr, KGRN"OK\n"KRST);

  const char *vs_source =
    "#version 120\n"  // OpenGL 2.1
    "attribute vec2 coord2d;                  "
    "void main(void) {                        "
    "  gl_Position = vec4(coord2d, 0.0, 1.0); "
    "}";

  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in vertex shader\n");
    return 0;
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs_source =
    "#version 120\n"  // OpenGL 2.1
    "void main(void) {        "
    "  gl_FragColor[0] = 0.0; "
    "  gl_FragColor[1] = 0.0; "
    "  gl_FragColor[2] = 1.0; "
    "}";

  glShaderSource(fs, 1, &fs_source, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in fragment shader\n");
    return 0;
  }

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    return 0;
  }
  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  return 1;
}

// Simple random
float rnd() { return (float)rand()/(float)(RAND_MAX); }

#define INFO(attr) fprintf(stderr,"%*s: %s\n",32, #attr, glGetString(attr))
static void context_info() {
    GLint major, minor, cnt;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    fprintf(stderr, "OpenGL %d.%d\n", major, minor);
    INFO(GL_VENDOR);
    INFO(GL_RENDERER);
    INFO(GL_VERSION);
    INFO(GL_SHADING_LANGUAGE_VERSION);
    // INFO(GL_EXTENSIONS);
}

typedef struct {
    GLfloat x, y, z;
} pnt_t;
uint cnt = 10;
pnt_t* p;
pnt_t* v;
pnt_t* c;

void scene_init() {
    context_info();
    p = malloc(sizeof(pnt_t)*cnt);
    v = malloc(sizeof(pnt_t)*cnt);
    c = malloc(sizeof(pnt_t)*cnt);
    for(uint i=0; i<cnt; i++) {
        float spd = 10.0 + rnd()*30.0;
        v[i] = (pnt_t){(0.5-rnd())/spd, (0.5-rnd())/spd, (0.5-rnd())/spd};
        p[i] = (pnt_t){0.5-rnd(), 0.5-rnd(), 0.5-rnd()};
        c[i] = (pnt_t){rnd(), rnd(), rnd()};
    }
}

void scene_draw(int frame) {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_STRIP);

    for(int i=0; i<cnt; i++) {
        int j, k;
        j = (i!=cnt-1) ? i+1 :     0;
        k = (i!=    0) ? i-1 : cnt-1;

        p[i].x += v[i].x;
        if(p[i].x > 1.0 || p[i].x < -1.0) v[i].x*=-1.0;
        p[i].y += v[i].y;
        if(p[i].y > 1.0 || p[i].y < -1.0) v[i].y*=-1.0;
        p[i].z += v[i].z;
        if(p[i].z > 1.0 || p[i].z < -1.0) v[i].z*=-1;
        glColor4f(0.1, c[i].x, c[i].y, c[i].z);
        glVertex3f(p[i].x, p[i].y, p[i].z);  
    }
    glEnd();
    glFlush();
}

void scene_draw_new(int frame) {
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_vertices[] = {
        0.0,  0.8,
       -0.8, -0.8,
        0.8, -0.8,
    };
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
        attribute_coord2d, // attribute
        2,                 // number of elements per vertex, here (x,y)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        triangle_vertices  // pointer to the C array
        );

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(attribute_coord2d);
    // called by egl
    // glutSwapBuffers();
}

void scene_close() {
    free(p);
    free(v);
    free(c);
    // glDeleteProgram(program);
}
