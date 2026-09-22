#include <jni.h>
#include <pthread.h>
#include <unistd.h>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Menu.h"

#include "engine/MockGameEngine.h"

static MockGameEngine gEngine;
static pthread_once_t gEngineOnce = PTHREAD_ONCE_INIT;

static void* engineLoop(void*) {
    while (true) {
        gEngine.update();
        usleep(16000);
    }

    return nullptr;
}

static void startEngineOnce() {
    gEngine.start();

    pthread_t thread;
    pthread_create(&thread, nullptr, engineLoop, nullptr);
    pthread_detach(thread);

    LOGI("Sandbox engine initialized");
}

static void ensureEngineStarted() {
    pthread_once(&gEngineOnce, startEngineOnce);
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_isGameLibLoaded(
        JNIEnv* env,
        jobject thiz) {

    (void)env;
    (void)thiz;

    ensureEngineStarted();

    // Standalone sandbox:
    // there is no external game library to wait for.
    return JNI_TRUE;
}

JNIEXPORT jobjectArray JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(
        JNIEnv* env,
        jobject thiz) {

    (void)thiz;

    ensureEngineStarted();

    const char* features[] = {

        OBFUSCATE("Category_Auto Play"),

        OBFUSCATE("0_Toggle_Enable Auto Play"),

        OBFUSCATE(
            "1_Spinner_Mode_Disabled,Semi,Full"
        ),

        OBFUSCATE(
            "2_SeekBar_Force_0_100"
        ),

        OBFUSCATE(
            "3_SeekBar_Action Interval (ms)_100_3000"
        ),

        OBFUSCATE("Category_Auto Queue"),

        OBFUSCATE(
            "4_Toggle_Enable Auto Queue"
        ),

        OBFUSCATE("Category_Engine"),

        OBFUSCATE(
            "5_Toggle_Pause Engine"
        ),

        OBFUSCATE(
            "6_Button_Start / Stop Engine"
        ),

        OBFUSCATE("Category_Sandbox"),

        OBFUSCATE(
            "7_Button_Simulate Match Start"
        ),

        OBFUSCATE(
            "8_Button_Simulate Shot"
        ),

        OBFUSCATE(
            "9_Button_Simulate Match End"
        ),

        OBFUSCATE(
            "10_Button_Print Engine State"
        )
    };

    const int total =
        sizeof(features) / sizeof(features[0]);

    jobjectArray result =
        env->NewObjectArray(
            total,
            env->FindClass("java/lang/String"),
            env->NewStringUTF("")
        );

    for (int i = 0; i < total; ++i) {
        env->SetObjectArrayElement(
            result,
            i,
            env->NewStringUTF(features[i])
        );
    }

    return result;
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(
        JNIEnv* env,
        jclass clazz,
        jobject obj,
        jint featNum,
        jstring featName,
        jint value,
        jboolean enabled,
        jstring text) {

    (void)env;
    (void)clazz;
    (void)obj;
    (void)featName;
    (void)text;

    ensureEngineStarted();

    // Negative IDs belong to LGL's own settings.
    if (featNum < 0)
        return;

    switch (featNum) {

        case 0:
            gEngine.toggleAutoPlay(enabled);
            LOGI(
                "Auto Play: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 1:
            if (value >= 0 && value <= 2) {
                gEngine.setAutoPlayMode(
                    static_cast<AutoPlayMode>(
                        value
                    )
                );

                LOGI(
                    "Auto Play mode: %d",
                    value
                );
            }
            break;

        case 2:
            gEngine.setForce(
                static_cast<float>(value)
            );

            LOGI(
                "Force: %d",
                value
            );
            break;

        case 3:
            gEngine.setActionInterval(
                value
            );

            LOGI(
                "Interval: %d ms",
                value
            );
            break;

        case 4:
            gEngine.toggleAutoQueue(
                enabled
            );

            LOGI(
                "Auto Queue: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 5:
            gEngine.pause(enabled);

            LOGI(
                "Engine pause: %s",
                enabled ? "YES" : "NO"
            );
            break;

        case 6:
            if (gEngine.isRunning()) {
                gEngine.stop();
                LOGI("Engine stopped");
            } else {
                gEngine.start();
                LOGI("Engine started");
            }
            break;

        case 7:
            gEngine.simulateMatchStart();
            LOGI("Simulated match started");
            break;

        case 8:
            gEngine.simulateShot();
            LOGI("Simulated shot");
            break;

        case 9:
            gEngine.simulateMatchEnd();
            LOGI("Simulated match ended");
            break;

        case 10: {
            const GameState state =
                gEngine.getGameState();

            LOGI(
                "STATE | running=%d autoplay=%d "
                "mode=%d force=%.1f interval=%d "
                "queue=%d paused=%d "
                "player=%d opponent=%d",
                gEngine.isRunning(),
                state.autoPlayEnabled,
                static_cast<int>(
                    state.autoPlayMode
                ),
                state.force,
                state.actionIntervalMs,
                state.autoQueueEnabled,
                state.paused,
                state.simulatedScorePlayer,
                state.simulatedScoreOpponent
            );

            break;
        }
    }
}

}
