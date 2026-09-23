#include "ConfigManager.h"

#include <stdexcept>
#include <utility>

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
    : activeProfile_("Default")
{
    preferences_.profileName = activeProfile_;
}

void ConfigManager::setProvider(
    std::unique_ptr<IConfigProvider> provider
)
{
    if (!provider)
    {
        throw std::invalid_argument(
            "Config provider cannot be null."
        );
    }

    std::lock_guard<std::mutex> lock(mutex_);
    provider_ = std::move(provider);
}

bool ConfigManager::createProfile(
    const std::string& name
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    UserPreferences profile;
    profile.profileName = name;

    return provider_->save(
        name,
        profile
    );
}

bool ConfigManager::saveProfile(
    const std::string& name,
    const UserPreferences& preferences
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    UserPreferences copy = preferences;
    copy.profileName = name;

    if (!provider_->save(name, copy))
    {
        return false;
    }

    if (name == activeProfile_)
    {
        preferences_ = copy;
    }

    return true;
}

bool ConfigManager::saveActiveProfile(
    const UserPreferences& preferences
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    UserPreferences copy = preferences;
    copy.profileName = activeProfile_;

    if (!provider_->save(
            activeProfile_,
            copy
        ))
    {
        return false;
    }

    preferences_ = copy;

    return true;
}

bool ConfigManager::loadProfile(
    const std::string& name
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    const auto loaded =
        provider_->load(name);

    if (!loaded.has_value())
    {
        return false;
    }

    preferences_ = loaded.value();
    activeProfile_ = name;
    preferences_.profileName = name;

    return true;
}

bool ConfigManager::deleteProfile(
    const std::string& name
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    if (name == "Default")
    {
        return false;
    }

    const bool removed =
        provider_->remove(name);

    if (
        removed &&
        activeProfile_ == name
    )
    {
        activeProfile_ = "Default";

        preferences_ =
            UserPreferences{};

        preferences_.profileName =
            "Default";
    }

    return removed;
}

bool ConfigManager::setActiveProfile(
    const std::string& name
)
{
    return loadProfile(name);
}

std::string ConfigManager::getActiveProfileName() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return activeProfile_;
}

UserPreferences ConfigManager::getPreferences() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return preferences_;
}

void ConfigManager::setPreferences(
    const UserPreferences& preferences
)
{
    std::lock_guard<std::mutex> lock(mutex_);
    preferences_ = preferences;
}

std::vector<std::string>
ConfigManager::listProfiles() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    ensureProvider();

    return provider_->listProfiles();
}

void ConfigManager::applyToGameState(
    GameState& state
) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    state.autoPlayEnabled =
        preferences_.autoPlay.enabled;

    state.autoPlayMode =
        preferences_.autoPlay.mode;

    state.force =
        preferences_.autoPlay.force;

    state.actionIntervalMs =
        preferences_.autoPlay.actionIntervalMs;

    state.autoQueueEnabled =
        preferences_.autoQueue.enabled;

    state.queueSpeedMode =
        preferences_.autoQueue.speedMode;

    state.coinsToUse =
        preferences_.autoQueue.coins;

    state.mixedJoin =
        preferences_.autoQueue.mixedJoin;

    state.fixOnSingleTable =
        preferences_.autoQueue.fixOnSingleTable;

    state.selectedTableCoins =
        preferences_.autoQueue.selectedTableCoins;

    state.paused =
        preferences_.paused;
}
void ConfigManager::captureGameState(
    const GameState& state
)
{
    std::lock_guard<std::mutex> lock(mutex_);

    preferences_.autoPlay.enabled =
        state.autoPlayEnabled;

    preferences_.autoPlay.mode =
        state.autoPlayMode;

    preferences_.autoPlay.force =
        state.force;

    preferences_.autoPlay.actionIntervalMs =
        state.actionIntervalMs;

    preferences_.autoQueue.enabled =
        state.autoQueueEnabled;

    preferences_.autoQueue.speedMode =
        state.queueSpeedMode;

    preferences_.autoQueue.coins =
        state.coinsToUse;

    preferences_.autoQueue.mixedJoin =
        state.mixedJoin;

    preferences_.autoQueue.fixOnSingleTable =
        state.fixOnSingleTable;

    preferences_.autoQueue.selectedTableCoins =
        state.selectedTableCoins;

    preferences_.paused =
        state.paused;
}
void ConfigManager::ensureProvider() const
{
    if (!provider_)
    {
        throw std::runtime_error(
            "ConfigManager provider has not been configured."
        );
    }
}
