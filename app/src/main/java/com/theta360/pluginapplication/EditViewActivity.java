package com.theta360.pluginapplication;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.SurfaceView;

import com.theta360.pluginapplication.app.GLApplication;
import com.theta360.pluginapplication.app.RenderingThread;
import com.theta360.pluginapplication.app.ndk.NDKApplication;

/**
 * Created by user on 2018/04/23.
 */

public class EditViewActivity extends Activity {
    private RenderingThread renderThread;

    static final String INTENT_KEY_FILE_URL= "FILE_URL";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        String url = intent.getStringExtra(INTENT_KEY_FILE_URL);

        GLApplication app = new NDKApplication();
        app.platform.context = this;
        app.url =url;
        renderThread = new RenderingThread(app);

        SurfaceView view = new SurfaceView(this);
        setContentView(view);
        renderThread.initialize(view);

        // 描画スレッドを開始する
        renderThread.start();
    }

    @Override
    protected void onPause() {
        super.onPause();
        renderThread.onPause();
        if (isFinishing()) {
            renderThread.onDestroy();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        renderThread.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // pause状態のままActivityが廃棄されることに備え、こちらでも廃棄を行う
        renderThread.onDestroy();
    }
}
