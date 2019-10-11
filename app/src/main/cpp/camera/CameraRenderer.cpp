//
// Created by templechen on 2019-10-11.
//

#include <base/native_log.h>
#include <GLES2/gl2.h>
#include "CameraRenderer.h"

CameraRenderer::CameraRenderer() {

}

CameraRenderer::~CameraRenderer() {

}

void CameraRenderer::cameraViewCreated(ANativeWindow *nativeWindow) {
    ALOGD("cameraViewCreated");
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    windowSurface = new window_surface(nativeWindow, eglCore);

    windowSurface->makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    baseFilter = new CameraBaseFilter();
}

void CameraRenderer::cameraViewChanged(int width, int height) {
    ALOGD("cameraViewChanged");
    windowSurface->makeCurrent();
    glViewport(0, 0, width, height);
    baseFilter->setNativeWindowSize(width, height);
}

void CameraRenderer::cameraViewDestroyed() {
    ALOGD("cameraViewDestroyed");
    if (baseFilter != nullptr) {
        baseFilter->release();
        delete baseFilter;
        baseFilter = nullptr;
    }
    if (windowSurface != nullptr) {
        windowSurface->release(true);
        delete windowSurface;
        windowSurface = nullptr;
    }
    if (eglCore != nullptr) {
//        eglCore->release();
        delete eglCore;
        eglCore = nullptr;
    }
}

void CameraRenderer::cameraViewDoFrame() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    baseFilter->doFrame();

    windowSurface->swapBuffer();
}
