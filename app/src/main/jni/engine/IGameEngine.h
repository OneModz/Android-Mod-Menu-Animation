#pragma once

#include "GameState.h"

class IGameEngine
{
public:
    virtual ~IGameEngine() = default;

    virtual void update() = 0;

    // Auto Play
    virtual void toggleAutoPlay(
        bool enabled
    ) = 0;

    virtual void setAutoPlayMode(
        AutoPlayMode mode
    ) = 0;

    virtual void setForce(
        float force
    ) = 0;

    virtual float getForce() const = 0;

    virtual void setActionInterval(
        int milliseconds
    ) = 0;

    virtual int getActionInterval() const = 0;

    // Auto Queue
    virtual void toggleAutoQueue(
        bool enabled
    ) = 0;

    virtual void setQueueSpeedMode(
        QueueSpeedMode mode
    ) = 0;

    virtual QueueSpeedMode getQueueSpeedMode()
        const = 0;

    virtual void setCoinsToUse(
        int coins
    ) = 0;

    virtual int getCoinsToUse() const = 0;

    virtual void setMixedJoin(
        bool enabled
    ) = 0;

    virtual bool isMixedJoinEnabled()
        const = 0;

    virtual void setFixOnSingleTable(
        bool enabled
    ) = 0;

    virtual bool isFixOnSingleTableEnabled()
        const = 0;

    virtual void setSelectedTableCoins(
        int coins
    ) = 0;

    virtual int getSelectedTableCoins()
        const = 0;

    // Lines / Mira
    virtual void setShowLines(
        bool enabled
    ) = 0;

    virtual bool getShowLines()
        const = 0;

    virtual void setKeepLinesAfterShot(
        bool enabled
    ) = 0;

    virtual bool getKeepLinesAfterShot()
        const = 0;

    virtual void setLineStyle(
        LineStyle style
    ) = 0;

    virtual LineStyle getLineStyle()
        const = 0;

    virtual void setLineWidth(
        float width
    ) = 0;

    virtual float getLineWidth()
        const = 0;

    virtual void setLineOpacity(
        float opacity
    ) = 0;

    virtual float getLineOpacity()
        const = 0;

    // Appearance / Overlay
    virtual void setMenuWidth(
        int width
    ) = 0;

    virtual int getMenuWidth()
        const = 0;

    virtual void setMenuHeight(
        int height
    ) = 0;

    virtual int getMenuHeight()
        const = 0;

    virtual void setOverlayOpacity(
        float opacity
    ) = 0;

    virtual float getOverlayOpacity()
        const = 0;

    virtual void setOverlayScale(
        float scale
    ) = 0;

    virtual float getOverlayScale()
        const = 0;

    // Engine
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void pause(
        bool paused
    ) = 0;

    virtual bool isRunning() const = 0;

    virtual GameState getGameState() const = 0;

    // Sandbox
    virtual void simulateMatchStart() = 0;
    virtual void simulateShot() = 0;
    virtual void simulateMatchEnd() = 0;
};
