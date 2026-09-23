#pragma once

#include "UserPreferences.h"

#include <optional>
#include <string>
#include <vector>

class IConfigProvider
{
public:
    virtual ~IConfigProvider() = default;

    virtual bool save(
        const std::string& profileName,
        const UserPreferences& preferences
    ) = 0;

    virtual std::optional<UserPreferences> load(
        const std::string& profileName
    ) = 0;

    virtual bool remove(
        const std::string& profileName
    ) = 0;

    virtual bool exists(
        const std::string& profileName
    ) const = 0;

    virtual std::vector<std::string> listProfiles() const = 0;
};
