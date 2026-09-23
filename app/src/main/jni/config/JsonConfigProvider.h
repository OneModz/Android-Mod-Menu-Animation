#pragma once

#include "IConfigProvider.h"

#include <filesystem>

class JsonConfigProvider final
    : public IConfigProvider
{
public:
    explicit JsonConfigProvider(
        std::filesystem::path configDirectory
    );

    bool save(
        const std::string& profileName,
        const UserPreferences& preferences
    ) override;

    std::optional<UserPreferences> load(
        const std::string& profileName
    ) override;

    bool remove(
        const std::string& profileName
    ) override;

    bool exists(
        const std::string& profileName
    ) const override;

    std::vector<std::string> listProfiles() const override;

private:
    std::filesystem::path profilePath(
        const std::string& profileName
    ) const;

    static std::string sanitizeProfileName(
        const std::string& profileName
    );

    std::filesystem::path configDirectory_;
};
