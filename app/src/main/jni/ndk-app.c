//
// Created by USER on 2018/04/04.
//


#include "support_ndk.h"
#include "openGL/open-gl.h"

#include <jni.h>
#include <GLES2/gl2.h>
#include <stdlib.h>
#include <time.h>


static jclass class_NDKApplication = NULL;


static jclass class_GLApplication = NULL;

/**
 * NDK側のGLApplicationポインタ
 */

static jfieldID field_GLApplication_ptr = NULL;

/**
 * View（レンダリングターゲット）の幅
 */
static jfieldID field_surface_width = NULL;

/**
 * View（レンダリングターゲット）の高さ
 */
static jfieldID field_surface_height = NULL;

/**
 * アプリ実行フラグ
 */
static jfieldID field_flags = NULL;

/**
 * プラットフォームごとに保持している拡張データ
 * サンプルアプリから扱う必要はない。
 */
static jfieldID field_platform = NULL;



static jfieldID field_url = NULL;


/**
 * アプリを強制終了させるメソッド
 */
static jmethodID method_abortWithMessage = NULL;

/**
 * NDKの情報をSDKにコピーする
 */
static void NDKApplication_syncNDK2SDK(JNIEnv *env, jobject _this, GLApplication *app) {
    (*env)->SetIntField(env, _this, field_surface_width, app->surface_width);
    (*env)->SetIntField(env, _this, field_surface_height, app->surface_height);
    (*env)->SetIntField(env, _this, field_flags, app->flags);
}


//extern "C"
JNIEXPORT void JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_initialize(JNIEnv *env, jobject instance){

    // 必要なリフレクションを得る
    if (!class_NDKApplication) {
        class_NDKApplication = (jclass) (*env)->NewGlobalRef(env, (*env)->FindClass(env, NDKApplication_CLASS_SIGNATURE));
        class_GLApplication = (*env)->NewGlobalRef(env, (*env)->GetSuperclass(env, class_NDKApplication));

        field_surface_width = ndk_loadIntField(env, class_GLApplication, "surface_width");
        field_surface_height = ndk_loadIntField(env, class_GLApplication, "surface_height");
        field_flags = ndk_loadIntField(env, class_GLApplication, "flags");
        field_platform = ndk_loadClassField(env, class_GLApplication,
                                            "L" AndroidPlatformData_CLASS_SIGNATURE ";",
                                            "platform");
        field_url = ndk_loadStringField(env, class_GLApplication, "url");
        field_GLApplication_ptr = ndk_loadLongField(env, class_NDKApplication, "GLApplication_ptr");

        method_abortWithMessage = ndk_loadMethod(env, class_GLApplication, "abortWithMessage",
                                                 "(L" GLApplication_CLASS_SIGNATURE ";Ljava/lang/String;)V",
                                                 true);
    }

    // アプリ構造体を作成する
    GLApplication *app = (GLApplication *) calloc(1, sizeof(GLApplication));

    {
        // platform固有データを設定する
        {
            NDKPlatform *platform = (NDKPlatform *) malloc(sizeof(NDKPlatform));
            platform->jGLApplication = (*env)->NewGlobalRef(env, instance);
            platform->jPlatform = (*env)->NewGlobalRef(env, (*env)->GetObjectField(env, instance,
                                                                                   field_platform));
            app->platform = (void *) platform;
        }

        app->url = (*env)->GetStringUTFChars(env, (*env)->GetObjectField(env, instance, field_url), 0);

        // 関数ポインタを設定する
        OpenGLLifeCycle *openGLLifeCycle=get_func();
        app->initialize = openGLLifeCycle->initialize;
        app->resized = openGLLifeCycle->resized;
        app->rendering = openGLLifeCycle->rendering;
        app->destroy = openGLLifeCycle->destroy;

    }

    // 構造体を保存する
    (*env)->SetLongField(env, instance, field_GLApplication_ptr, (jlong)app);

    // サンプル関数に処理を行わせる
    app->initialize(app);


}

//extern "C"
JNIEXPORT void JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_resized(JNIEnv *env, jobject instance) {


    GLApplication *app = (GLApplication*) (*env)->GetLongField(env, instance, field_GLApplication_ptr);
    assert(app != NULL);

    // サーフェイスの幅と高さを同期する
    {
        app->surface_width = (*env)->GetIntField(env, instance, field_surface_width);
        app->surface_height = (*env)->GetIntField(env, instance, field_surface_height);
    }

    // サンプル関数に処理を行わせる
    (*app->resized)(app);

    // 変数情報をNDK -> SDKにコピーする
    NDKApplication_syncNDK2SDK(env, instance, app);

}

//extern "C"
JNIEXPORT void JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_rendering(JNIEnv *env, jobject instance) {

    GLApplication *app = (GLApplication*) (*env)->GetLongField(env, instance, field_GLApplication_ptr);
    assert(app != NULL);
    // サンプル関数に処理を行わせる
    (*app->rendering)(app);

    // 変数情報をNDK -> SDKにコピーする
    NDKApplication_syncNDK2SDK(env, instance, app);

}

//extern "C"
JNIEXPORT void JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_destroy(JNIEnv *env, jobject instance) {

    GLApplication *app = (GLApplication*) (*env)->GetLongField(env, instance, field_GLApplication_ptr);
    assert(app != NULL);
    (*app->destroy)(app);

    // 参照削除
    {
        NDKPlatform *platform = (NDKPlatform*) app->platform;
        (*env)->DeleteGlobalRef(env, platform->jGLApplication);
        (*env)->DeleteGlobalRef(env, platform->jPlatform);

    }
    // GLApplicationを廃棄する
    free(app->platform);
    free(app);

}

//extern "C"
JNIEXPORT void JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_async(JNIEnv *env, jobject instance) {

    GLApplication *app = (GLApplication*) (*env)->GetLongField(env, instance, field_GLApplication_ptr);
    assert(app != NULL);
    // サンプル関数に処理を行わせる
    if (app->async) {
        (*app->async)(app);
    }
//    (*app->rendering)(app);

}

//extern "C"
JNIEXPORT jboolean JNICALL
Java_com_theta360_pluginapplication_app_ndk_NDKApplication_hasAsync(JNIEnv *env, jobject instance) {

    GLApplication *app = (GLApplication*) (*env)->GetLongField(env, instance, field_GLApplication_ptr);
    assert(app != NULL);

    return app->async != NULL;

}


