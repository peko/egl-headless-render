
#include <stdio.h>

#include "egl.h"
#include "scene.h"

int main(int argc, char *argv[]) {

    egl_init();
    scene_init();

    egl_init_stream();
    // from now on use your OpenGL context
    char name[32];
    for(int i=0; i<3600; i++) {
        scene_draw(i);
        egl_swap();
        egl_write_stream();
    }
    egl_close_stream();

    // scene_close();
    egl_close();

    return 0;
}

