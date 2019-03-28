package com.theta360.pluginapplication.app.ndk;

import com.theta360.pluginapplication.app.GLApplication;

public class NDKApplication extends GLApplication {
    /**
     * NDK側のGLApplicationポインタ
     */
    long GLApplication_ptr;


    public NDKApplication() {
    }

    @Override
    public native void initialize();

    @Override
    public native void async();

    @Override
    public native void resized();

    @Override
    public native void rendering();

    @Override
    public native void destroy();

    @Override
    public native boolean hasAsync();
}
