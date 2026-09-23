#pragma once

enum class AutoPlayMode
{
    Disabled = 0,
    Semi,
    Full
};

enum class QueueSpeedMode
{
    Slow = 0,
    Global,
    Fast
};

enum class LineStyle
{
    Solid = 0,
    Dashed,
    Dotted
};

enum class MatchState
{
    Idle = 0,
    Searching,
    Playing,
    Finished
};

struct GameState
{
    // Auto Play
    bool autoPlayEnabled = false;

    AutoPlayMode autoPlayMode =
        AutoPlayMode::Disabled;

    float force = 50.0f;

    int actionIntervalMs = 1000;

    // Auto Queue
    bool autoQueueEnabled = false;

    QueueSpeedMode queueSpeedMode =
        QueueSpeedMode::Global;

    int coinsToUse = 100;

    bool mixedJoin = false;

    bool fixOnSingleTable = false;

    int selectedTableCoins = 100;

    // Lines / Mira
    bool showLines = false;

    bool keepLinesAfterShot = false;

    LineStyle lineStyle =
        LineStyle::Solid;

    float lineWidth = 2.0f;

    float lineOpacity = 1.0f;

    // Appearance / Overlay
    int menuWidth = 290;

    int menuHeight = 210;

    float overlayOpacity = 0.90f;

    float overlayScale = 1.0f;

    // Player Information - Sandbox
    int playerLevel = 30;

    int playerCash = 320;

    long long playerCoins = 9151275;

    // Match / Engine
    MatchState matchState =
        MatchState::Idle;

    bool paused = false;

    // Sandbox
    int simulatedScorePlayer = 0;
    int simulatedScoreOpponent = 0;
};
