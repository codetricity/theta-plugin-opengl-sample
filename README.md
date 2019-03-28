# OpenGL plug-in Sample for RICOH THETA
This sample plug-in does image processing via OpenGL.<br>
This project is based on [ricohapi/theta-plugin-sdk](https://github.com/ricohapi/theta-plugin-sdk/tree/v1.0.0).


## Set up
* Download this project using git clone.

```
$ git clone https://github.com/ricohapi/theta-plugin-opengl-sample
```

* Import this project by Android Studio.
* Build the plug-in and install to the THETA.
* Set up app permissions manually after the installation.<br>
Enable the permissions of "Storage".<br>

## Usage
* To run plug-in mode, press and hold the mode button.
* Press the mode button several times until the still mode lamp is turned.
* Press the shutter button and capture.
* The processed image is displayed by Vysor.


## Development Environment
### Camera
* RICOH THETA V Firmware ver.2.41.1 and above

### SDK/Library
* RICOH THETA Plug-in SDK ver.1.0.1
* openGL

### Development Software
* Android Studio ver.3.0.1
* gradle ver.3.1.4
* NDK ver. 16.0.4442984


## License

```
Copyright 2018 Ricoh Company, Ltd.
Third party copyrights are property of their respective owners.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```