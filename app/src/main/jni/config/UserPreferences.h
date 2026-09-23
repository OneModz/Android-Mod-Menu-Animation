#pragma once

#include "../engine/GameState.h"

#include <string>

enum class AppTheme
{
    Dark = 0,
    Light,
    Custom
};

enum class AppLanguage
{
    English = 0,
    Portuguese,
    Spanish
};

struct AutoPlayPreferences
{
    bool enabled = false;

    AutoPlayMode mode =
        AutoPlayMode::Disabled;

    float force = 50.0f;

    int actionIntervalMs = 1000;
};

struct AutoQueuePreferences
{
    bool enabled = false;

    QueueSpeedMode speedMode =
        QueueSpeedMode::Global;

    int coins = 100;

    bool mixedJoin = false;

    bool fixOnSingleTable = false;

    int selectedTableCoins = 100;
};

struct LinesPreferences
{
    bool showLines = false;

    bool keepLinesAfterShot = false;

    LineStyle style =
        LineStyle::Solid;

    float width = 2.0f;

    float opacity = 1.0f;
};

struct AppearancePreferences
{
    int menuWidth = 290;

    int menuHeight = 210;

    float opacity = 0.90f;

    float scale = 1.0f;

    AppTheme theme =
        AppTheme::Dark;
};

struct UserPreferences
{
    std::string profileName = "Default";

    AutoPlayPreferences autoPlay;

    AutoQueuePreferences autoQueue;

    LinesPreferences lines;

    AppearancePreferences appearance;

    AppLanguage language =
        AppLanguage::Portuguese;

    bool paused = false;
};
