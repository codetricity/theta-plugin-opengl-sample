/**
 * Copyright 2018 Ricoh Company, Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.theta360.pluginapplication;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;

import com.theta360.pluginapplication.task.TakePictureTask;
import com.theta360.pluginapplication.task.TakePictureTask.Callback;
import com.theta360.pluginapplication.util.ES20;
import com.theta360.pluginlibrary.activity.PluginActivity;
import com.theta360.pluginlibrary.callback.KeyCallback;
import com.theta360.pluginlibrary.receiver.KeyReceiver;
import com.theta360.pluginlibrary.values.LedColor;
import com.theta360.pluginlibrary.values.LedTarget;

import java.io.File;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.theta360.pluginapplication.EditViewActivity.INTENT_KEY_FILE_URL;


public class MainActivity extends PluginActivity {

    private static final String DCIM = Environment.getExternalStoragePublicDirectory(
            Environment.DIRECTORY_DCIM).getPath();


    // Used to load the 'openGL-library' library on application startup.
    static {
        System.loadLibrary("openGL-library");
        ES20.initializeNative();
    }

    private TakePictureTask.Callback mTakePictureTaskCallback = new Callback() {
        @Override
        public void onTakePicture(String fileUrl) {
            Matcher matcher = Pattern.compile("/\\d{3}RICOH.*").matcher(fileUrl);
            if (matcher.find()) {
                String filePath = DCIM + matcher.group();
                File file = new File(filePath);
                Intent intent = new Intent(MainActivity.this, EditViewActivity.class);  //インテントの作成
                intent.putExtra(INTENT_KEY_FILE_URL, file.getAbsolutePath());
                startActivity(intent);
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Set a callback when a button operation event is acquired.
        setKeyCallback(new KeyCallback() {
            @Override
            public void onKeyDown(int keyCode, KeyEvent event) {
                if (keyCode == KeyReceiver.KEYCODE_CAMERA) {
                    new TakePictureTask(mTakePictureTaskCallback).execute();
                }
            }

            @Override
            public void onKeyUp(int keyCode, KeyEvent event) {
            }

            @Override
            public void onKeyLongPress(int keyCode, KeyEvent event) {
            }

        });

    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
