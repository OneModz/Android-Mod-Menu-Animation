#include <jni.h>
#include <pthread.h>
#include <unistd.h>

#include <exception>
#include <memory>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Menu.h"

#include "engine/MockGameEngine.h"

#include "config/ConfigManager.h"
#include "config/JsonConfigProvider.h"

namespace
{

constexpr const char* CONFIG_DIRECTORY =
    "/data/data/uk.lgl/files/configs";

class ConfigBootstrap
{
public:
    ConfigBootstrap() noexcept
    {
        try
        {
            auto& config =
                ConfigManager::getInstance();

            auto provider =
                std::make_unique<
                    JsonConfigProvider
                >(
                    CONFIG_DIRECTORY
                );

            config.setProvider(
                std::move(provider)
            );

            if (
                !config.loadProfile(
                    "Default"
                )
            )
            {
                config.createProfile(
                    "Default"
                );

                config.loadProfile(
                    "Default"
                );
            }

            const std::string profile =
                config.getActiveProfileName();

            LOGI(
                "Config initialized | profile=%s",
                profile.c_str()
            );
        }
        catch (const std::exception& error)
        {
            LOGE(
                "Config initialization failed: %s",
                error.what()
            );
        }
        catch (...)
        {
            LOGE(
                "Config initialization failed: "
                "unknown error"
            );
        }
    }
};

static ConfigBootstrap gConfigBootstrap;
static MockGameEngine gEngine;

static int gSelectedProfile = 0;

static const char* selectedProfileName()
{
    switch (gSelectedProfile)
    {
        case 1:
            return "Training";

        case 2:
            return "Custom";

        case 0:
        default:
            return "Default";
    }
}

}
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

        OBFUSCATE(
            "14_Spinner_Queue Speed_Slow,Global,Fast"
        ),

        OBFUSCATE(
            "15_SeekBar_Coins to Use_0_100000"
        ),

        OBFUSCATE(
            "16_Toggle_Mixed Join"
        ),

        OBFUSCATE(
            "17_Toggle_Fix on Single Table"
        ),

        OBFUSCATE(
            "18_Spinner_Table Selection_25,100,500,1.2k,2.5k,3k,5k,10k,50.5k,101k,510k,2.55M,4.15M"
        ),

        OBFUSCATE("Category_Lines / Mira"),

        OBFUSCATE(
            "19_Toggle_Show Lines"
        ),

        OBFUSCATE(
            "20_Toggle_Keep Lines After Shot"
        ),

        OBFUSCATE(
            "21_Spinner_Line Style_Solid,Dashed,Dotted"
        ),

        OBFUSCATE(
            "22_SeekBar_Line Width_1_10"
        ),

        OBFUSCATE(
            "23_SeekBar_Line Opacity_0_100"
        ),

        OBFUSCATE("Category_Player Information"),

        OBFUSCATE(
            "RichTextView_Player Level: <font color='#4C8DFF'>30</font>"
        ),

        OBFUSCATE(
            "RichTextView_Player Cash: <font color='#41C300'>320</font>"
        ),

        OBFUSCATE(
            "RichTextView_Player Coins: <font color='#FFD54F'>9151275</font>"
        ),

        OBFUSCATE("Category_Appearance / Overlay"),

        OBFUSCATE(
            "24_SeekBar_Menu Width_200_600"
        ),

        OBFUSCATE(
            "25_SeekBar_Menu Height_150_700"
        ),

        OBFUSCATE(
            "26_SeekBar_Overlay Opacity_20_100"
        ),

        OBFUSCATE(
            "27_SeekBar_Overlay Scale_50_200"
        ),

        OBFUSCATE("Category_Engine"),

        OBFUSCATE(
            "5_Toggle_Pause Engine"
        ),

        OBFUSCATE(
            "6_Button_Start / Stop Engine"
        ),

        OBFUSCATE("Category_Profiles"),

        OBFUSCATE(
            "11_Spinner_Profile_Default,Training,Custom"
        ),

        OBFUSCATE(
            "12_Button_Save Profile"
        ),

        OBFUSCATE(
            "13_Button_Load Profile"
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


JNIEXPORT jstring JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getEngineStateJson(
        JNIEnv* env,
        jobject thiz)
{
    (void)thiz;

    ensureEngineStarted();

    const GameState state =
        gEngine.getGameState();

    char buffer[512];

    snprintf(
        buffer,
        sizeof(buffer),
        "{\"autoPlay\":%s,"
        "\"mode\":%d,"
        "\"force\":%.1f,"
        "\"interval\":%d,"
        "\"autoQueue\":%s,"
        "\"paused\":%s}",
        state.autoPlayEnabled
            ? "true"
            : "false",
        static_cast<int>(
            state.autoPlayMode
        ),
        state.force,
        state.actionIntervalMs,
        state.autoQueueEnabled
            ? "true"
            : "false",
        state.paused
            ? "true"
            : "false"
    );

    return env->NewStringUTF(buffer);
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

        case 14:
            if (value >= 0 && value <= 2)
            {
                gEngine.setQueueSpeedMode(
                    static_cast<QueueSpeedMode>(
                        value
                    )
                );

                LOGI(
                    "Queue speed: %d",
                    value
                );
            }
            break;

        case 15:
            gEngine.setCoinsToUse(
                value
            );

            LOGI(
                "Coins to use: %d",
                value
            );
            break;

        case 16:
            gEngine.setMixedJoin(
                enabled
            );

            LOGI(
                "Mixed Join: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 17:
            gEngine.setFixOnSingleTable(
                enabled
            );

            LOGI(
                "Fix on single table: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 18: {
            static const int tableValues[] = {
                25,
                100,
                500,
                1200,
                2500,
                3000,
                5000,
                10000,
                50500,
                101000,
                510000,
                2550000,
                4150000
            };

            constexpr int tableCount =
                sizeof(tableValues) /
                sizeof(tableValues[0]);

            if (
                value >= 0 &&
                value < tableCount
            )
            {
                gEngine.setSelectedTableCoins(
                    tableValues[value]
                );

                LOGI(
                    "Selected table: %d",
                    tableValues[value]
                );
            }

            break;
        }

        case 19:
            gEngine.setShowLines(
                enabled
            );

            LOGI(
                "Show Lines: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 20:
            gEngine.setKeepLinesAfterShot(
                enabled
            );

            LOGI(
                "Keep Lines After Shot: %s",
                enabled ? "ON" : "OFF"
            );
            break;

        case 21:
            if (value >= 0 && value <= 2)
            {
                gEngine.setLineStyle(
                    static_cast<LineStyle>(
                        value
                    )
                );

                LOGI(
                    "Line Style: %d",
                    value
                );
            }
            break;

        case 22:
            gEngine.setLineWidth(
                static_cast<float>(
                    value
                )
            );

            LOGI(
                "Line Width: %d",
                value
            );
            break;

        case 23:
            gEngine.setLineOpacity(
                static_cast<float>(
                    value
                ) / 100.0f
            );

            LOGI(
                "Line Opacity: %d%%",
                value
            );
            break;

        case 24:
            gEngine.setMenuWidth(
                value
            );

            LOGI(
                "Menu Width: %d",
                value
            );
            break;

        case 25:
            gEngine.setMenuHeight(
                value
            );

            LOGI(
                "Menu Height: %d",
                value
            );
            break;

        case 26:
            gEngine.setOverlayOpacity(
                static_cast<float>(value)
                / 100.0f
            );

            LOGI(
                "Overlay Opacity: %d%%",
                value
            );
            break;

        case 27:
            gEngine.setOverlayScale(
                static_cast<float>(value)
                / 100.0f
            );

            LOGI(
                "Overlay Scale: %d%%",
                value
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

        case 11:
            if (value >= 0 && value <= 2)
            {
                gSelectedProfile = value;

                LOGI(
                    "Selected profile: %s",
                    selectedProfileName()
                );
            }
            break;

        case 12: {
            auto& config =
                ConfigManager::getInstance();

            config.captureGameState(
                gEngine.getGameState()
            );

            UserPreferences preferences =
                config.getPreferences();

            const char* profile =
                selectedProfileName();

            preferences.profileName =
                profile;

            if (
                config.saveProfile(
                    profile,
                    preferences
                )
            )
            {
                LOGI(
                    "Profile saved: %s",
                    profile
                );
            }
            else
            {
                LOGE(
                    "Failed to save profile: %s",
                    profile
                );
            }

            break;
        }

        case 13: {
            auto& config =
                ConfigManager::getInstance();

            const char* profile =
                selectedProfileName();

            if (!config.loadProfile(profile))
            {
                LOGE(
                    "Profile not found: %s",
                    profile
                );

                break;
            }

            const UserPreferences preferences =
                config.getPreferences();

            gEngine.toggleAutoPlay(
                preferences.autoPlay.enabled
            );

            gEngine.setAutoPlayMode(
                preferences.autoPlay.mode
            );

            gEngine.setForce(
                preferences.autoPlay.force
            );

            gEngine.setActionInterval(
                preferences.autoPlay.actionIntervalMs
            );

            gEngine.toggleAutoQueue(
                preferences.autoQueue.enabled
            );

            gEngine.setQueueSpeedMode(
                preferences.autoQueue.speedMode
            );

            gEngine.setCoinsToUse(
                preferences.autoQueue.coins
            );

            gEngine.setMixedJoin(
                preferences.autoQueue.mixedJoin
            );

            gEngine.setFixOnSingleTable(
                preferences.autoQueue.fixOnSingleTable
            );

            gEngine.setSelectedTableCoins(
                preferences.autoQueue.selectedTableCoins
            );

            gEngine.setShowLines(
                preferences.lines.showLines
            );

            gEngine.setKeepLinesAfterShot(
                preferences.lines.keepLinesAfterShot
            );

            gEngine.setLineStyle(
                preferences.lines.style
            );

            gEngine.setLineWidth(
                preferences.lines.width
            );

            gEngine.setLineOpacity(
                preferences.lines.opacity
            );

            gEngine.setMenuWidth(
                preferences.appearance.menuWidth
            );

            gEngine.setMenuHeight(
                preferences.appearance.menuHeight
            );

            gEngine.setOverlayOpacity(
                preferences.appearance.opacity
            );

            gEngine.setOverlayScale(
                preferences.appearance.scale
            );

            gEngine.pause(
                preferences.paused
            );

            LOGI(
                "Profile loaded: %s",
                profile
            );

            break;
        }
    }
}

}
