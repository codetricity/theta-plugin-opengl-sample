//
// Created by USER on 2018/04/24.
//

#ifndef EDITPIC_OPEN_GL_3D_H
#define EDITPIC_OPEN_GL_3D_H

#include <support.h>

typedef struct OpenGLLifeCycle {

    /**
     * 初期化関数
     */
    GLApplication_initialize initialize;

    /**
     * サーフェイスサイズ変更関数
     */
    GLApplication_surface_changed resized;

    /**
     * レンダリング関数
     */
    GLApplication_rendering rendering;

    /**
     * 解放関数
     */
    GLApplication_destroy destroy;

} OpenGLLifeCycle;

extern OpenGLLifeCycle* get_func();

#endif //EDITPIC_OPEN_GL_3D_H
