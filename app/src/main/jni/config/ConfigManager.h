#pragma once

#include "IConfigProvider.h"

#include <memory>
#include <mutex>
#include <string>
#include <vector>

class ConfigManager final
{
public:
    static ConfigManager& getInstance();

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    void setProvider(
        std::unique_ptr<IConfigProvider> provider
    );

    bool createProfile(
        const std::string& name
    );

    bool saveProfile(
        const std::string& name,
        const UserPreferences& preferences
    );

    bool saveActiveProfile(
        const UserPreferences& preferences
    );

    bool loadProfile(
        const std::string& name
    );

    bool deleteProfile(
        const std::string& name
    );

    bool setActiveProfile(
        const std::string& name
    );

    std::string getActiveProfileName() const;

    UserPreferences getPreferences() const;

    void setPreferences(
        const UserPreferences& preferences
    );

    std::vector<std::string> listProfiles() const;

    void applyToGameState(
        GameState& state
    ) const;

    void captureGameState(
        const GameState& state
    );

private:
    ConfigManager();

    void ensureProvider() const;

    mutable std::mutex mutex_;

    std::unique_ptr<IConfigProvider> provider_;

    UserPreferences preferences_;

    std::string activeProfile_;
};
