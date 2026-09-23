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

// =========================================================
// CONSTRUCTOR
// =========================================================

MockGameEngine::MockGameEngine()
{
    ConfigManager::getInstance()
        .applyToGameState(state_);

    stateEnteredAt_ =
        Clock::now();

    log(
        "MockGameEngine initialized "
        "with AutoPlay FSM."
    );
}

// =========================================================
// UPDATE / NON-BLOCKING TICK
// =========================================================

void MockGameEngine::update()
{
    const auto now =
        Clock::now();

    std::lock_guard<std::mutex>
        lock(stateMutex_);

    if (!running_)
    {
        return;
    }

    if (state_.paused)
    {
        resetAutomationLocked(
            now,
            "Engine paused"
        );

        return;
    }

    if (!state_.autoPlayEnabled)
    {
        resetAutomationLocked(
            now,
            "AutoPlay disabled"
        );

        return;
    }

    if (
        state_.autoPlayMode ==
        AutoPlayMode::Disabled
    )
    {
        resetAutomationLocked(
            now,
            "AutoPlay mode disabled"
        );

        return;
    }

    if (
        state_.matchState !=
        MatchState::Playing
    )
    {
        resetAutomationLocked(
            now,
            "No active simulated match"
        );

        return;
    }

    processAutoPlayLocked(now);
}

// =========================================================
// FSM
// =========================================================

void MockGameEngine::processAutoPlayLocked(
    TimePoint now
)
{
    switch (automationState_)
    {
        case AutomationState::STATE_IDLE:
        {
            transitionToLocked(
                AutomationState::
                    STATE_DECIDING,
                now
            );

            break;
        }

        case AutomationState::
            STATE_DECIDING:
        {
            decideAutoPlayActionLocked();

            transitionToLocked(
                AutomationState::
                    STATE_EXECUTING,
                now
            );

            break;
        }

        case AutomationState::
            STATE_EXECUTING:
        {
            executePendingActionLocked();

            transitionToLocked(
                AutomationState::
                    STATE_COOLDOWN,
                now
            );

            break;
        }

        case AutomationState::
            STATE_COOLDOWN:
        {
            const auto elapsed =
                std::chrono::
                    duration_cast<
                        std::chrono::
                            milliseconds
                    >(
                        now -
                        stateEnteredAt_
                    );

            const int cooldownMs =
                std::clamp(
                    state_.actionIntervalMs,
                    100,
                    5000
                );

            if (
                elapsed.count() >=
                cooldownMs
            )
            {
                transitionToLocked(
                    AutomationState::
                        STATE_DECIDING,
                    now
                );
            }

            break;
        }
    }
}

// =========================================================
// SIMULATED AI DECISION
// =========================================================

void MockGameEngine::
decideAutoPlayActionLocked()
{
    pendingAction_ =
        PendingAction{};

    const float configuredForce =
        std::clamp(
            state_.force,
            0.0f,
            100.0f
        );

    pendingAction_.mode =
        state_.autoPlayMode;

    pendingAction_.type =
        SimulatedActionType::Shot;

    switch (state_.autoPlayMode)
    {
        case AutoPlayMode::Disabled:
        {
            pendingAction_.valid =
                false;

            return;
        }

        case AutoPlayMode::Semi:
        {
            /*
             * Semi Auto uses a reduced simulated
             * force so its behavior differs from
             * Full Auto.
             */
            pendingAction_.selectedForce =
                std::clamp(
                    configuredForce *
                        0.85f,
                    0.0f,
                    100.0f
                );

            pendingAction_.expectedScore =
                pendingAction_
                    .selectedForce >=
                55.0f;

            break;
        }

        case AutoPlayMode::Full:
        {
            pendingAction_.selectedForce =
                configuredForce;

            pendingAction_.expectedScore =
                pendingAction_
                    .selectedForce >=
                40.0f;

            break;
        }
    }

    pendingAction_.valid =
        true;

    MOCK_LOG(
        "AI decision | cycle=%llu "
        "mode=%s "
        "configuredForce=%.1f "
        "selectedForce=%.1f "
        "prediction=%s",
        static_cast<
            unsigned long long
        >(automationCycle_ + 1),
        autoPlayModeName(
            pendingAction_.mode
        ),
        configuredForce,
        pendingAction_.selectedForce,
        pendingAction_.expectedScore
            ? "SCORE"
            : "MISS"
    );
}

// =========================================================
// SIMULATED ACTION EXECUTION
// =========================================================

void MockGameEngine::
executePendingActionLocked()
{
    if (!pendingAction_.valid)
    {
        log(
            "FSM execution skipped: "
            "no pending action."
        );

        return;
    }

    switch (pendingAction_.type)
    {
        case SimulatedActionType::None:
        {
            log(
                "FSM execution: "
                "no action."
            );

            break;
        }

        case SimulatedActionType::Shot:
        {
            /*
             * Sandbox only:
             *
             * no external process,
             * memory or application is touched.
             *
             * The result only changes GameState.
             */

            if (
                pendingAction_
                    .expectedScore
            )
            {
                ++state_
                    .simulatedScorePlayer;
            }

            ++automationCycle_;

            MOCK_LOG(
                "AI execute | cycle=%llu "
                "action=SHOT "
                "force=%.1f "
                "result=%s "
                "score=%d-%d",
                static_cast<
                    unsigned long long
                >(automationCycle_),
                pendingAction_
                    .selectedForce,
                pendingAction_
                    .expectedScore
                    ? "SCORE"
                    : "MISS",
                state_
                    .simulatedScorePlayer,
                state_
                    .simulatedScoreOpponent
            );

            break;
        }
    }

    pendingAction_ =
        PendingAction{};
}

// =========================================================
// FSM TRANSITIONS
// =========================================================

void MockGameEngine::transitionToLocked(
    AutomationState next,
    TimePoint now
)
{
    if (
        automationState_ ==
        next
    )
    {
        return;
    }

    const AutomationState previous =
        automationState_;

    automationState_ =
        next;

    stateEnteredAt_ =
        now;

    logTransition(
        previous,
        next
    );
}

void MockGameEngine::resetAutomationLocked(
    TimePoint now,
    const char* reason
)
{
    const bool hadActiveState =
        automationState_ !=
            AutomationState::
                STATE_IDLE
        ||
        pendingAction_.valid;

    pendingAction_ =
        PendingAction{};

    if (
        automationState_ !=
        AutomationState::
            STATE_IDLE
    )
    {
        transitionToLocked(
            AutomationState::
                STATE_IDLE,
            now
        );
    }

    if (
        hadActiveState &&
        reason != nullptr
    )
    {
        MOCK_LOG(
            "FSM reset | reason=%s",
            reason
        );
    }
}

// =========================================================
// AUTO PLAY
// =========================================================

void MockGameEngine::toggleAutoPlay(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.autoPlayEnabled =
            enabled;

        if (!enabled)
        {
            resetAutomationLocked(
                Clock::now(),
                "AutoPlay manually disabled"
            );
        }
    }

    persistStateSnapshot();

    MOCK_LOG(
        "AutoPlay: %s",
        enabled ? "ON" : "OFF"
    );
}

void MockGameEngine::setAutoPlayMode(
    AutoPlayMode mode
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        const bool changed =
            state_.autoPlayMode != mode;

        state_.autoPlayMode =
            mode;

        if (changed)
        {
            resetAutomationLocked(
                Clock::now(),
                "AutoPlay mode changed"
            );
        }
    }

    persistStateSnapshot();

    MOCK_LOG(
        "AutoPlay mode: %s",
        autoPlayModeName(mode)
    );
}

void MockGameEngine::setForce(
    float force
)
{
    float applied = 0.0f;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.force =
            std::clamp(
                force,
                0.0f,
                100.0f
            );

        applied =
            state_.force;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Force: %.1f",
        applied
    );
}

float MockGameEngine::getForce()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.force;
}

void MockGameEngine::setActionInterval(
    int milliseconds
)
{
    int applied = 0;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.actionIntervalMs =
            std::clamp(
                milliseconds,
                100,
                5000
            );

        applied =
            state_.actionIntervalMs;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Action interval: %d ms",
        applied
    );
}

int MockGameEngine::getActionInterval()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.actionIntervalMs;
}

// =========================================================
// AUTO QUEUE
// =========================================================

void MockGameEngine::toggleAutoQueue(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.autoQueueEnabled =
            enabled;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "AutoQueue: %s",
        enabled ? "ON" : "OFF"
    );
}

void MockGameEngine::setQueueSpeedMode(
    QueueSpeedMode mode
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.queueSpeedMode =
            mode;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Queue speed mode: %d",
        static_cast<int>(mode)
    );
}

QueueSpeedMode
MockGameEngine::getQueueSpeedMode()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.queueSpeedMode;
}

void MockGameEngine::setCoinsToUse(
    int coins
)
{
    int applied = 0;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.coinsToUse =
            std::clamp(
                coins,
                0,
                1000000
            );

        applied =
            state_.coinsToUse;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Coins to use: %d",
        applied
    );
}

int MockGameEngine::getCoinsToUse()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.coinsToUse;
}

void MockGameEngine::setMixedJoin(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.mixedJoin =
            enabled;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Mixed Join: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::
isMixedJoinEnabled() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.mixedJoin;
}

void MockGameEngine::
setFixOnSingleTable(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.fixOnSingleTable =
            enabled;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Fix on single table: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::
isFixOnSingleTableEnabled() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.fixOnSingleTable;
}

void MockGameEngine::
setSelectedTableCoins(
    int coins
)
{
    int applied = 0;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.selectedTableCoins =
            std::clamp(
                coins,
                0,
                10000000
            );

        applied =
            state_.selectedTableCoins;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Selected table coins: %d",
        applied
    );
}

int MockGameEngine::
getSelectedTableCoins() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_
        .selectedTableCoins;
}

// =========================================================
// LINES / MIRA
// =========================================================

void MockGameEngine::setShowLines(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.showLines =
            enabled;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Show Lines: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::getShowLines()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.showLines;
}

void MockGameEngine::
setKeepLinesAfterShot(
    bool enabled
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.keepLinesAfterShot =
            enabled;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Keep Lines After Shot: %s",
        enabled ? "ON" : "OFF"
    );
}

bool MockGameEngine::
getKeepLinesAfterShot() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_
        .keepLinesAfterShot;
}

void MockGameEngine::setLineStyle(
    LineStyle style
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.lineStyle =
            style;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Line Style: %d",
        static_cast<int>(style)
    );
}

LineStyle MockGameEngine::
getLineStyle() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.lineStyle;
}

void MockGameEngine::setLineWidth(
    float width
)
{
    float applied = 0.0f;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.lineWidth =
            std::clamp(
                width,
                1.0f,
                10.0f
            );

        applied =
            state_.lineWidth;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Line Width: %.1f",
        applied
    );
}

float MockGameEngine::getLineWidth()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.lineWidth;
}

void MockGameEngine::setLineOpacity(
    float opacity
)
{
    float applied = 0.0f;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.lineOpacity =
            std::clamp(
                opacity,
                0.0f,
                1.0f
            );

        applied =
            state_.lineOpacity;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Line Opacity: %.2f",
        applied
    );
}

float MockGameEngine::
getLineOpacity() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.lineOpacity;
}

// =========================================================
// APPEARANCE / OVERLAY
// =========================================================

void MockGameEngine::setMenuWidth(
    int width
)
{
    int applied = 0;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.menuWidth =
            std::clamp(
                width,
                200,
                600
            );

        applied =
            state_.menuWidth;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Menu Width: %d",
        applied
    );
}

int MockGameEngine::getMenuWidth()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.menuWidth;
}

void MockGameEngine::setMenuHeight(
    int height
)
{
    int applied = 0;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.menuHeight =
            std::clamp(
                height,
                150,
                700
            );

        applied =
            state_.menuHeight;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Menu Height: %d",
        applied
    );
}

int MockGameEngine::getMenuHeight()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.menuHeight;
}

void MockGameEngine::
setOverlayOpacity(
    float opacity
)
{
    float applied = 0.0f;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.overlayOpacity =
            std::clamp(
                opacity,
                0.20f,
                1.0f
            );

        applied =
            state_.overlayOpacity;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Overlay Opacity: %.2f",
        applied
    );
}

float MockGameEngine::
getOverlayOpacity() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.overlayOpacity;
}

void MockGameEngine::setOverlayScale(
    float scale
)
{
    float applied = 0.0f;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.overlayScale =
            std::clamp(
                scale,
                0.50f,
                2.0f
            );

        applied =
            state_.overlayScale;
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Overlay Scale: %.2f",
        applied
    );
}

float MockGameEngine::
getOverlayScale() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_.overlayScale;
}

// =========================================================
// ENGINE
// =========================================================

void MockGameEngine::start()
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        if (running_)
        {
            return;
        }

        running_ =
            true;

        resetAutomationLocked(
            Clock::now(),
            nullptr
        );
    }

    log("Engine started.");
}

void MockGameEngine::stop()
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        if (!running_)
        {
            return;
        }

        running_ =
            false;

        resetAutomationLocked(
            Clock::now(),
            "Engine stopped"
        );
    }

    log("Engine stopped.");
}

void MockGameEngine::pause(
    bool paused
)
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.paused =
            paused;

        if (paused)
        {
            resetAutomationLocked(
                Clock::now(),
                "Engine paused"
            );
        }
    }

    persistStateSnapshot();

    MOCK_LOG(
        "Pause: %s",
        paused ? "YES" : "NO"
    );
}

bool MockGameEngine::isRunning()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return running_;
}

GameState MockGameEngine::
getGameState() const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return state_;
}

// =========================================================
// FSM OBSERVABILITY
// =========================================================

const char*
MockGameEngine::getAutomationStateText()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return automationStateName(
        automationState_
    );
}

std::uint64_t
MockGameEngine::getAutomationCycle()
    const
{
    std::lock_guard<std::mutex>
        lock(stateMutex_);

    return automationCycle_;
}

// =========================================================
// SANDBOX MATCH
// =========================================================

void MockGameEngine::
simulateMatchStart()
{
    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        state_.matchState =
            MatchState::Playing;

        state_.simulatedScorePlayer =
            0;

        state_.simulatedScoreOpponent =
            0;

        automationCycle_ =
            0;

        resetAutomationLocked(
            Clock::now(),
            nullptr
        );
    }

    log(
        "Simulated match started."
    );
}

void MockGameEngine::simulateShot()
{
    float force = 0.0f;
    int playerScore = 0;
    int opponentScore = 0;
    bool validMatch = false;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        if (
            state_.matchState ==
            MatchState::Playing
        )
        {
            validMatch =
                true;

            force =
                state_.force;

            ++state_
                .simulatedScorePlayer;

            playerScore =
                state_
                    .simulatedScorePlayer;

            opponentScore =
                state_
                    .simulatedScoreOpponent;
        }
    }

    if (!validMatch)
    {
        log(
            "Shot ignored: "
            "no active simulated match."
        );

        return;
    }

    MOCK_LOG(
        "Manual simulated shot | "
        "force=%.1f | score=%d-%d",
        force,
        playerScore,
        opponentScore
    );
}

void MockGameEngine::
simulateMatchEnd()
{
    bool ended = false;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        if (
            state_.matchState ==
            MatchState::Playing
        )
        {
            state_.matchState =
                MatchState::Finished;

            resetAutomationLocked(
                Clock::now(),
                "Simulated match ended"
            );

            ended =
                true;
        }
    }

    if (ended)
    {
        log(
            "Simulated match ended."
        );
    }
}

// =========================================================
// CONFIG SNAPSHOT
// =========================================================

void MockGameEngine::
persistStateSnapshot() const
{
    GameState snapshot;

    {
        std::lock_guard<std::mutex>
            lock(stateMutex_);

        snapshot =
            state_;
    }

    ConfigManager::getInstance()
        .captureGameState(
            snapshot
        );
}

// =========================================================
// LOGGING
// =========================================================

void MockGameEngine::log(
    const char* message
) const
{
    MOCK_LOG(
        "%s",
        message
    );
}

void MockGameEngine::logTransition(
    AutomationState from,
    AutomationState to
) const
{
    MOCK_LOG(
        "FSM transition | %s -> %s",
        automationStateName(from),
        automationStateName(to)
    );
}

const char*
MockGameEngine::automationStateName(
    AutomationState state
)
{
    switch (state)
    {
        case AutomationState::
            STATE_IDLE:
        {
            return "IDLE";
        }

        case AutomationState::
            STATE_DECIDING:
        {
            return "DECIDING";
        }

        case AutomationState::
            STATE_EXECUTING:
        {
            return "EXECUTING";
        }

        case AutomationState::
            STATE_COOLDOWN:
        {
            return "COOLDOWN";
        }
    }

    return "UNKNOWN";
}

const char*
MockGameEngine::autoPlayModeName(
    AutoPlayMode mode
)
{
    switch (mode)
    {
        case AutoPlayMode::Disabled:
        {
            return "Disabled";
        }

        case AutoPlayMode::Semi:
        {
            return "Semi";
        }

        case AutoPlayMode::Full:
        {
            return "Full";
        }
    }

    return "Unknown";
}
