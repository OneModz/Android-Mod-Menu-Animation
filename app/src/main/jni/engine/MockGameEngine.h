#pragma once

#include "IGameEngine.h"

#include <chrono>
#include <cstdint>
#include <mutex>

class MockGameEngine final : public IGameEngine
{
public:
    MockGameEngine();

    void update() override;

    // =====================================================
    // AUTO PLAY
    // =====================================================

    void toggleAutoPlay(
        bool enabled
    ) override;

    void setAutoPlayMode(
        AutoPlayMode mode
    ) override;

    void setForce(
        float force
    ) override;

    float getForce() const override;

    void setActionInterval(
        int milliseconds
    ) override;

    int getActionInterval()
        const override;

    // =====================================================
    // AUTO QUEUE
    // =====================================================

    void toggleAutoQueue(
        bool enabled
    ) override;

    void setQueueSpeedMode(
        QueueSpeedMode mode
    ) override;

    QueueSpeedMode getQueueSpeedMode()
        const override;

    void setCoinsToUse(
        int coins
    ) override;

    int getCoinsToUse()
        const override;

    void setMixedJoin(
        bool enabled
    ) override;

    bool isMixedJoinEnabled()
        const override;

    void setFixOnSingleTable(
        bool enabled
    ) override;

    bool isFixOnSingleTableEnabled()
        const override;

    void setSelectedTableCoins(
        int coins
    ) override;

    int getSelectedTableCoins()
        const override;

    // =====================================================
    // LINES / MIRA
    // =====================================================

    void setShowLines(
        bool enabled
    ) override;

    bool getShowLines()
        const override;

    void setKeepLinesAfterShot(
        bool enabled
    ) override;

    bool getKeepLinesAfterShot()
        const override;

    void setLineStyle(
        LineStyle style
    ) override;

    LineStyle getLineStyle()
        const override;

    void setLineWidth(
        float width
    ) override;

    float getLineWidth()
        const override;

    void setLineOpacity(
        float opacity
    ) override;

    float getLineOpacity()
        const override;

    // =====================================================
    // APPEARANCE
    // =====================================================

    void setMenuWidth(
        int width
    ) override;

    int getMenuWidth()
        const override;

    void setMenuHeight(
        int height
    ) override;

    int getMenuHeight()
        const override;

    void setOverlayOpacity(
        float opacity
    ) override;

    float getOverlayOpacity()
        const override;

    void setOverlayScale(
        float scale
    ) override;

    float getOverlayScale()
        const override;

    // =====================================================
    // ENGINE
    // =====================================================

    void start() override;
    void stop() override;

    void pause(
        bool paused
    ) override;

    bool isRunning()
        const override;

    GameState getGameState()
        const override;

    // =====================================================
    // FSM OBSERVABILITY
    // =====================================================

    const char* getAutomationStateText()
        const;

    std::uint64_t getAutomationCycle()
        const;

    // =====================================================
    // SANDBOX
    // =====================================================

    void simulateMatchStart() override;
    void simulateShot() override;
    void simulateMatchEnd() override;

private:

    // =====================================================
    // AUTO PLAY FSM
    // =====================================================

    enum class AutomationState
    {
        STATE_IDLE = 0,
        STATE_DECIDING,
        STATE_EXECUTING,
        STATE_COOLDOWN
    };

    enum class SimulatedActionType
    {
        None = 0,
        Shot
    };

    struct PendingAction
    {
        SimulatedActionType type =
            SimulatedActionType::None;

        AutoPlayMode mode =
            AutoPlayMode::Disabled;

        float selectedForce =
            0.0f;

        bool expectedScore =
            false;

        bool valid =
            false;
    };

    using Clock =
        std::chrono::steady_clock;

    using TimePoint =
        Clock::time_point;

    // =====================================================
    // FSM
    // =====================================================

    void processAutoPlayLocked(
        TimePoint now
    );

    void decideAutoPlayActionLocked();

    void executePendingActionLocked();

    void transitionToLocked(
        AutomationState next,
        TimePoint now
    );

    void resetAutomationLocked(
        TimePoint now,
        const char* reason
    );

    // =====================================================
    // CONFIG
    // =====================================================

    void persistStateSnapshot()
        const;

    // =====================================================
    // LOGGING
    // =====================================================

    void log(
        const char* message
    ) const;

    void logTransition(
        AutomationState from,
        AutomationState to
    ) const;

    static const char*
    automationStateName(
        AutomationState state
    );

    static const char*
    autoPlayModeName(
        AutoPlayMode mode
    );

    // =====================================================
    // STATE
    // =====================================================

    mutable std::mutex stateMutex_;

    GameState state_;

    bool running_ =
        false;

    AutomationState automationState_ =
        AutomationState::STATE_IDLE;

    PendingAction pendingAction_;

    TimePoint stateEnteredAt_;

    std::uint64_t automationCycle_ =
        0;
};
