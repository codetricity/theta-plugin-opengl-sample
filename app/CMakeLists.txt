# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


find_package(GLUT)
find_package(OpenGL)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(src/main/jni/support src/main/jni src/main/jni/openGL)

add_library( # Sets the name of the library.
             openGL-library

             # Sets the library as a shared library.
              SHARED

             # Provides a relative path to your source file(s).
             src/main/jni/ndk-app.c
             src/main/jni/ES20_impl.c
             src/main/jni/support_ndk.c
             src/main/jni/RawData_impl.c
             src/main/jni/RawPixelImage_impl.c
             src/main/jni/openGL/open-gl.c
             src/main/jni/support/support.c
             src/main/jni/support/support_gl.c
             src/main/jni/support/support_gl_CompressedTexture_KtxImage.c
             src/main/jni/support/support_gl_CompressedTexture_PkmImage.c
             src/main/jni/support/support_gl_CompressedTexture_PvrtcImage.c
             src/main/jni/support/support_gl_Shader.c
             src/main/jni/support/support_gl_Sprite.c
             src/main/jni/support/support_gl_Texture.c
             src/main/jni/support/support_gl_Texture_RawPixelImage.c
             src/main/jni/support/support_gl_Vector.c
             src/main/jni/support/support_RawData.c
             )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# Mac依存
# link_directories(/usr/local/lib)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )



# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       openGL-library

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} android EGL GLESv2)





