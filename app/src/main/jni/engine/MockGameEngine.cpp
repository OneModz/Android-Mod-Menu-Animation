#include "MockGameEngine.h"

#include "../config/ConfigManager.h"

#include <algorithm>
#include <android/log.h>

#define LOG_TAG "MockEngine"

#define MOCK_LOG(...) \
    __android_log_print( \
        ANDROID_LOG_INFO, \
        LOG_TAG, \
        __VA_ARGS__ \
    )

MockGameEngine::MockGameEngine()
{
    ConfigManager::getInstance()
        .applyToGameState(state_);

    lastAction_ =
        std::chrono::steady_clock::now();

    log("MockGameEngine initialized.");
}

void MockGameEngine::update()
{
    if (!running_)
        return;

    if (state_.paused)
        return;

    if (!state_.autoPlayEnabled)
        return;

    if (
        state_.matchState !=
        MatchState::Playing
    )
        return;

    const auto now =
        std::chrono::steady_clock::now();

    const auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(now - lastAction_);

    if (
        elapsed.count() >=
        state_.actionIntervalMs
    )
    {
        processAutoPlay();

        lastAction_ = now;
    }
}

void MockGameEngine::toggleAutoPlay(
    bool enabled
)
{
    state_.autoPlayEnabled = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "AutoPlay: %s",
        enabled ? "ON" : "OFF"
    );
}

void MockGameEngine::setAutoPlayMode(
    AutoPlayMode mode
)
{
    state_.autoPlayMode = mode;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "AutoPlay mode: %d",
        static_cast<int>(mode)
    );
}

void MockGameEngine::setForce(
    float force
)
{
    state_.force =
        std::clamp(
            force,
            0.0f,
            100.0f
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Force: %.1f",
        state_.force
    );
}

float MockGameEngine::getForce() const
{
    return state_.force;
}

void MockGameEngine::setActionInterval(
    int milliseconds
)
{
    state_.actionIntervalMs =
        std::clamp(
            milliseconds,
            100,
            5000
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Action interval: %d ms",
        state_.actionIntervalMs
    );
}

int MockGameEngine::getActionInterval()
    const
{
    return state_.actionIntervalMs;
}

void MockGameEngine::toggleAutoQueue(
    bool enabled
)
{
    state_.autoQueueEnabled = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "AutoQueue: %s",
        enabled ? "ON" : "OFF"
    );
}


void MockGameEngine::setQueueSpeedMode(
    QueueSpeedMode mode
)
{
    state_.queueSpeedMode = mode;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Queue speed mode: %d",
        static_cast<int>(mode)
    );
}

QueueSpeedMode MockGameEngine::getQueueSpeedMode()
    const
{
    return state_.queueSpeedMode;
}

void MockGameEngine::setCoinsToUse(
    int coins
)
{
    state_.coinsToUse =
        std::clamp(
            coins,
            0,
            1000000
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Coins to use: %d",
        state_.coinsToUse
    );
}

int MockGameEngine::getCoinsToUse() const
{
    return state_.coinsToUse;
}


void MockGameEngine::setMixedJoin(
    bool enabled
)
{
    state_.mixedJoin = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Mixed Join: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::isMixedJoinEnabled()
    const
{
    return state_.mixedJoin;
}

void MockGameEngine::setFixOnSingleTable(
    bool enabled
)
{
    state_.fixOnSingleTable = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Fix on single table: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::isFixOnSingleTableEnabled()
    const
{
    return state_.fixOnSingleTable;
}

void MockGameEngine::setSelectedTableCoins(
    int coins
)
{
    state_.selectedTableCoins =
        std::clamp(
            coins,
            0,
            10000000
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Selected table coins: %d",
        state_.selectedTableCoins
    );
}

int MockGameEngine::getSelectedTableCoins()
    const
{
    return state_.selectedTableCoins;
}


void MockGameEngine::setShowLines(
    bool enabled
)
{
    state_.showLines = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Show Lines: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::getShowLines()
    const
{
    return state_.showLines;
}

void MockGameEngine::setKeepLinesAfterShot(
    bool enabled
)
{
    state_.keepLinesAfterShot = enabled;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Keep Lines After Shot: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::getKeepLinesAfterShot()
    const
{
    return state_.keepLinesAfterShot;
}

void MockGameEngine::setLineStyle(
    LineStyle style
)
{
    state_.lineStyle = style;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Line Style: %d",
        static_cast<int>(style)
    );
}

LineStyle MockGameEngine::getLineStyle()
    const
{
    return state_.lineStyle;
}

void MockGameEngine::setLineWidth(
    float width
)
{
    state_.lineWidth =
        std::clamp(
            width,
            1.0f,
            10.0f
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Line Width: %.1f",
        state_.lineWidth
    );
}

float MockGameEngine::getLineWidth()
    const
{
    return state_.lineWidth;
}

void MockGameEngine::setLineOpacity(
    float opacity
)
{
    state_.lineOpacity =
        std::clamp(
            opacity,
            0.0f,
            1.0f
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Line Opacity: %.2f",
        state_.lineOpacity
    );
}

float MockGameEngine::getLineOpacity()
    const
{
    return state_.lineOpacity;
}


void MockGameEngine::setMenuWidth(
    int width
)
{
    state_.menuWidth =
        std::clamp(
            width,
            200,
            600
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Menu Width: %d",
        state_.menuWidth
    );
}

int MockGameEngine::getMenuWidth()
    const
{
    return state_.menuWidth;
}

void MockGameEngine::setMenuHeight(
    int height
)
{
    state_.menuHeight =
        std::clamp(
            height,
            150,
            700
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Menu Height: %d",
        state_.menuHeight
    );
}

int MockGameEngine::getMenuHeight()
    const
{
    return state_.menuHeight;
}

void MockGameEngine::setOverlayOpacity(
    float opacity
)
{
    state_.overlayOpacity =
        std::clamp(
            opacity,
            0.20f,
            1.0f
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Overlay Opacity: %.2f",
        state_.overlayOpacity
    );
}

float MockGameEngine::getOverlayOpacity()
    const
{
    return state_.overlayOpacity;
}

void MockGameEngine::setOverlayScale(
    float scale
)
{
    state_.overlayScale =
        std::clamp(
            scale,
            0.50f,
            2.0f
        );

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Overlay Scale: %.2f",
        state_.overlayScale
    );
}

float MockGameEngine::getOverlayScale()
    const
{
    return state_.overlayScale;
}

void MockGameEngine::start()
{
    if (running_)
        return;

    running_ = true;

    lastAction_ =
        std::chrono::steady_clock::now();

    log("Engine started.");
}

void MockGameEngine::stop()
{
    if (!running_)
        return;

    running_ = false;

    log("Engine stopped.");
}

void MockGameEngine::pause(
    bool paused
)
{
    state_.paused = paused;

    ConfigManager::getInstance()
        .captureGameState(state_);

    MOCK_LOG(
        "Pause: %s",
        paused ? "YES" : "NO"
    );
}

bool MockGameEngine::isRunning() const
{
    return running_;
}

GameState MockGameEngine::getGameState()
    const
{
    return state_;
}

void MockGameEngine::simulateMatchStart()
{
    state_.matchState =
        MatchState::Playing;

    state_.simulatedScorePlayer = 0;
    state_.simulatedScoreOpponent = 0;

    log("Simulated match started.");
}

void MockGameEngine::simulateShot()
{
    if (
        state_.matchState !=
        MatchState::Playing
    )
    {
        log(
            "Shot ignored: "
            "no active simulated match."
        );

        return;
    }

    MOCK_LOG(
        "Simulated shot | force=%.1f",
        state_.force
    );

    ++state_.simulatedScorePlayer;
}

void MockGameEngine::simulateMatchEnd()
{
    if (
        state_.matchState !=
        MatchState::Playing
    )
        return;

    state_.matchState =
        MatchState::Finished;

    log("Simulated match ended.");
}

void MockGameEngine::processAutoPlay()
{
    switch (state_.autoPlayMode)
    {
        case AutoPlayMode::Disabled:
            return;

        case AutoPlayMode::Semi:
            log(
                "Semi Auto: "
                "evaluating simulated shot."
            );
            break;

        case AutoPlayMode::Full:
            log(
                "Full Auto: "
                "performing simulated action."
            );

            simulateShot();
            break;
    }
}

void MockGameEngine::log(
    const char* message
) const
{
    MOCK_LOG("%s", message);
}
