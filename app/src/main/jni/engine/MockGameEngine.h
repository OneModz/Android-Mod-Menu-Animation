#pragma once

#include "IGameEngine.h"

#include <chrono>

class MockGameEngine final : public IGameEngine
{
public:
    MockGameEngine();

    void update() override;

    // Auto Play
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

    int getActionInterval() const override;

    // Auto Queue
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

    int getCoinsToUse() const override;

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

    // Lines / Mira
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

    // Appearance / Overlay
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

    // Engine
    void start() override;
    void stop() override;

    void pause(
        bool paused
    ) override;

    bool isRunning() const override;

    GameState getGameState() const override;

    // Sandbox
    void simulateMatchStart() override;
    void simulateShot() override;
    void simulateMatchEnd() override;

private:
    void log(
        const char* message
    ) const;

    void processAutoPlay();

    GameState state_;

    bool running_ = false;

    std::chrono::steady_clock::time_point
        lastAction_;
};
