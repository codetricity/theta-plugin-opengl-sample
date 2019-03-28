//
// Created by USER on 2018/04/12.
//

#include "open-gl.h"

#include <support.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1415927

const short __DV = 40;
const short __DU = 80;

const short vertices_num = (__DV+1) * (__DU+1);
const short indices_num = __DV * __DU * 6;

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // UV情報属性
    GLint attr_uv;

    /**
     * world look projection
     */
    GLint unif_wlp;

    // 球に貼り付けるテクスチャロケーション
    GLint unif_texture;

    // 球に貼り付けるテクスチャ
    Texture *texture;

    // 回転角
    GLfloat rotate;


    // 頂点バッファ
    GLuint vertices_buffer;

    // インデックスバッファ
    GLuint indices_buffer;


} Extension_VertexstructAlignment;

// 頂点構造体を定義
typedef struct SphereVertex {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} SphereVertex;

// Texture構造体を定義
typedef struct SphereTexture {
    GLfloat u;
    GLfloat v;
} SphereTexture;

SphereVertex vertices[vertices_num];
SphereTexture textureVertices[vertices_num];
short sphereIndices[indices_num];

void sphere_calc();
void calcSphereIndices();
void makeVertexArray();

/**
 * アプリの初期化を行う
 */
void initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_VertexstructAlignment*) malloc(sizeof(Extension_VertexstructAlignment));
    // サンプルアプリ用データを取り出す
    Extension_VertexstructAlignment *extension = (Extension_VertexstructAlignment*) app->extension;



    // 頂点シェーダーを用意する
    {
        {
            // カメラ用の行列を計算に加える
            const GLchar *vertex_shader_source =
                    // 位置属性
                    "attribute mediump vec4 attr_pos;"
                            "attribute mediump vec2 attr_uv;"
                            // varying
                            "varying mediump vec2 vary_uv;"
                            // uniform
                            "uniform mediump mat4 unif_wlp;"
                            "void main() {"
                            "   gl_Position = unif_wlp * attr_pos;"
                            "   vary_uv = attr_uv;"
                            "}";

            const GLchar *fragment_shader_source =
                    //
                    "varying mediump vec2 vary_uv;"
                            "uniform sampler2D unif_texture;"
                            "void main() {"
                            "   gl_FragColor = texture2D(unif_texture, vary_uv);"
                            "}";

            // コンパイルとリンクを行う
            extension->shader_program = Shader_createProgramFromSource(vertex_shader_source,
                                                                       fragment_shader_source);
        }

        // attributeを取り出す
        {
            extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
            assert(extension->attr_pos >= 0);

            extension->attr_uv = glGetAttribLocation(extension->shader_program, "attr_uv");
            assert(extension->attr_uv >= 0);
        }

        // uniform変数のlocationを取得する
        {
            extension->unif_texture = glGetUniformLocation(extension->shader_program,
                                                           "unif_texture");
            assert(extension->unif_texture >= 0);

            // 変換行列を取得
            extension->unif_wlp = glGetUniformLocation(extension->shader_program, "unif_wlp");
            assert(extension->unif_wlp >= 0);

        }
    }

    {
        extension->texture = Texture_load(app, (app -> url), TEXTURE_RAW_RGBA8);
        assert(extension->texture);
    }



    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    extension->rotate = 0;
}

/**
 * レンダリングエリアが変更された
 */
void resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_VertexstructAlignment *extension = (Extension_VertexstructAlignment *) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // カメラを初期化する
    {
        // カメラをセットアップする
        const vec3 camera_pos = vec3_create(0, 0.9, 0); // カメラ位置
        const vec3 camera_look = vec3_create(0, 0, 0); // カメラ注視
        const vec3 camera_up = vec3_create(0, 0, 1); // カメラ上ベクトル

        const GLfloat prj_near = 0.1f;
        const GLfloat prj_far = 20.0f;
        const GLfloat prj_fovY = 80.0f;
        const GLfloat prj_aspect = (GLfloat) (app->surface_width) / (GLfloat) (app->surface_height);

        const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);
        const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
        const mat4 world = mat4_rotate(vec3_createNormalized(1, 1, 0), extension->rotate);

        // 行列を転送する
        mat4 wlp = mat4_multiply(projection, lookAt);
        wlp = mat4_multiply(wlp, world);
        glUniformMatrix4fv(extension->unif_wlp, 1, GL_FALSE, (GLfloat *) wlp.m);

        extension->rotate += 0;
    }

    {
        makeVertexArray();

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glUniform1i(extension->unif_texture, 0);

        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVertex), (GLvoid*) vertices);
        // UVを転送する
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(SphereTexture), (GLvoid*) textureVertices);
        glDrawElements(GL_TRIANGLES, indices_num, GL_UNSIGNED_SHORT, sphereIndices);

        assert(glGetError() == GL_NO_ERROR);
    }
    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);


}

/**
 * アプリのデータ削除を行う
 */
void destroy( GLApplication *app) {

    // サンプルアプリ用データを取り出す
    Extension_VertexstructAlignment *extension = (Extension_VertexstructAlignment*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // テクスチャ解放
    Texture_free(extension->texture);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}



OpenGLLifeCycle * get_func() {
    OpenGLLifeCycle *p = (OpenGLLifeCycle *) malloc(sizeof(OpenGLLifeCycle));
    p->initialize = initialize;
    p->resized = resized;
    p->rendering = rendering;
    p->destroy = destroy;
    return p;
}

void makeVertexArray(){
    sphere_calc();
    calcSphereIndices();
}

void sphere_calc(){

    const float radius = 1.0;
    for (int j = 0; j <= __DV; j++) {
        const float v = (float) j / (float) __DV; // 水平分割を行う高さの割合
        const float y = cos(PI * v); // 水平分割の高さ座標
        const float r = sin(PI * v); // 球の半径
        for (int i = 0; i <= __DU; i++) {
            const float u = (float) i / (float) __DU; // 円周分割の割合
            const float up = 2.0 * PI * u - (PI / 2); // 円周上の位置

            const GLfloat k = (GLfloat) (u * 1.0);

            vertices[j * (__DU+1) + i].x = (GLfloat) ((cos(up) * r) * radius);
            vertices[j * (__DU+1) + i].y = (GLfloat) (y * radius);
            vertices[j * (__DU+1) + i].z = (GLfloat) ((sin(up) * r) * radius);


            textureVertices[j * (__DU+1) + i].u = k;
            textureVertices[j * (__DU+1) + i].v = (GLfloat) v;

        }

    }
}

void calcSphereIndices() {
    int ii = 0;
    for (short i = 0; i < __DV; i++) {
        short b = (short) (i * (__DU + 1));
        for (short j = 0; j < __DU; j++) {
            short p = (short) (b + j);
            short q = (short) (b + j + 1);
            short r = (short) (p + __DU + 1);
            short s = (short) (q + __DU + 1);
            sphereIndices[ii++] = p;
            sphereIndices[ii++] = q;
            sphereIndices[ii++] = r;
            sphereIndices[ii++] = r;
            sphereIndices[ii++] = q;
            sphereIndices[ii++] = s;
        }
    }
}
