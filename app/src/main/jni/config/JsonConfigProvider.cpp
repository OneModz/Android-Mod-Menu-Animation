#include "JsonConfigProvider.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <utility>

#include "../third_party/nlohmann/json.hpp"

using json = nlohmann::json;

namespace
{

json serializePreferences(
    const UserPreferences& prefs
)
{
    return {
        {
            "profileName",
            prefs.profileName
        },
        {
            "autoPlay",
            {
                {
                    "enabled",
                    prefs.autoPlay.enabled
                },
                {
                    "mode",
                    static_cast<int>(
                        prefs.autoPlay.mode
                    )
                },
                {
                    "force",
                    prefs.autoPlay.force
                },
                {
                    "actionIntervalMs",
                    prefs.autoPlay.actionIntervalMs
                }
            }
        },
        {
            "autoQueue",
            {
                {
                    "enabled",
                    prefs.autoQueue.enabled
                },
                {
                    "speedMode",
                    static_cast<int>(
                        prefs.autoQueue.speedMode
                    )
                },
                {
                    "coins",
                    prefs.autoQueue.coins
                },
                {
                    "mixedJoin",
                    prefs.autoQueue.mixedJoin
                },
                {
                    "fixOnSingleTable",
                    prefs.autoQueue.fixOnSingleTable
                },
                {
                    "selectedTableCoins",
                    prefs.autoQueue.selectedTableCoins
                }
            }
        },
        {
            "lines",
            {
                {
                    "showLines",
                    prefs.lines.showLines
                },
                {
                    "keepLinesAfterShot",
                    prefs.lines.keepLinesAfterShot
                },
                {
                    "style",
                    static_cast<int>(
                        prefs.lines.style
                    )
                },
                {
                    "width",
                    prefs.lines.width
                },
                {
                    "opacity",
                    prefs.lines.opacity
                }
            }
        },

        {
            "appearance",
            {
                {
                    "menuWidth",
                    prefs.appearance.menuWidth
                },
                {
                    "menuHeight",
                    prefs.appearance.menuHeight
                },
                {
                    "opacity",
                    prefs.appearance.opacity
                },
                {
                    "scale",
                    prefs.appearance.scale
                },
                {
                    "theme",
                    static_cast<int>(
                        prefs.appearance.theme
                    )
                }
            }
        },
        {
            "paused",
            prefs.paused
        }
    };
}

UserPreferences deserializePreferences(
    const json& data
)
{
    UserPreferences prefs;

    prefs.profileName =
        data.value(
            "profileName",
            "Default"
        );

    if (data.contains("autoPlay"))
    {
        const auto& autoPlay =
            data.at("autoPlay");

        prefs.autoPlay.enabled =
            autoPlay.value(
                "enabled",
                false
            );

        prefs.autoPlay.mode =
            static_cast<AutoPlayMode>(
                autoPlay.value(
                    "mode",
                    0
                )
            );

        prefs.autoPlay.force =
            autoPlay.value(
                "force",
                50.0f
            );

        prefs.autoPlay.actionIntervalMs =
            autoPlay.value(
                "actionIntervalMs",
                1000
            );
    }

    if (data.contains("autoQueue"))
    {
        const auto& autoQueue =
            data.at("autoQueue");

        prefs.autoQueue.enabled =
            autoQueue.value(
                "enabled",
                false
            );

        prefs.autoQueue.speedMode =
            static_cast<QueueSpeedMode>(
                autoQueue.value(
                    "speedMode",
                    1
                )
            );

        prefs.autoQueue.coins =
            autoQueue.value(
                "coins",
                100
            );

        prefs.autoQueue.mixedJoin =
            autoQueue.value(
                "mixedJoin",
                false
            );

        prefs.autoQueue.fixOnSingleTable =
            autoQueue.value(
                "fixOnSingleTable",
                false
            );

        prefs.autoQueue.selectedTableCoins =
            autoQueue.value(
                "selectedTableCoins",
                100
            );
    }

    if (data.contains("lines"))
    {
        const auto& lines =
            data.at("lines");

        prefs.lines.showLines =
            lines.value(
                "showLines",
                false
            );

        prefs.lines.keepLinesAfterShot =
            lines.value(
                "keepLinesAfterShot",
                false
            );

        prefs.lines.style =
            static_cast<LineStyle>(
                lines.value(
                    "style",
                    0
                )
            );

        prefs.lines.width =
            lines.value(
                "width",
                2.0f
            );

        prefs.lines.opacity =
            lines.value(
                "opacity",
                1.0f
            );
    }

    if (data.contains("appearance"))
    {
        const auto& appearance =
            data.at("appearance");

        prefs.appearance.menuWidth =
            appearance.value(
                "menuWidth",
                290
            );

        prefs.appearance.menuHeight =
            appearance.value(
                "menuHeight",
                210
            );

        prefs.appearance.opacity =
            appearance.value(
                "opacity",
                0.90f
            );

        prefs.appearance.scale =
            appearance.value(
                "scale",
                1.0f
            );

        prefs.appearance.theme =
            static_cast<AppTheme>(
                appearance.value(
                    "theme",
                    0
                )
            );
    }

    prefs.paused =
        data.value(
            "paused",
            false
        );

    return prefs;
}

}

JsonConfigProvider::JsonConfigProvider(
    std::filesystem::path configDirectory
)
    : configDirectory_(
        std::move(configDirectory)
    )
{
    std::error_code error;

    std::filesystem::create_directories(
        configDirectory_,
        error
    );
}

bool JsonConfigProvider::save(
    const std::string& profileName,
    const UserPreferences& preferences
)
{
    try
    {
        std::error_code error;

        std::filesystem::create_directories(
            configDirectory_,
            error
        );

        if (error)
        {
            return false;
        }

        const auto path =
            profilePath(profileName);

        std::ofstream file(
            path,
            std::ios::out |
            std::ios::trunc
        );

        if (!file.is_open())
        {
            return false;
        }

        UserPreferences copy =
            preferences;

        copy.profileName =
            profileName;

        file
            << serializePreferences(copy)
                   .dump(4);

        file.flush();

        return file.good();
    }
    catch (...)
    {
        return false;
    }
}

std::optional<UserPreferences>
JsonConfigProvider::load(
    const std::string& profileName
)
{
    try
    {
        const auto path =
            profilePath(profileName);

        std::ifstream file(path);

        if (!file.is_open())
        {
            return std::nullopt;
        }

        json data;

        file >> data;

        UserPreferences prefs =
            deserializePreferences(data);

        prefs.profileName =
            profileName;

        return prefs;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

bool JsonConfigProvider::remove(
    const std::string& profileName
)
{
    try
    {
        std::error_code error;

        const bool removed =
            std::filesystem::remove(
                profilePath(profileName),
                error
            );

        return removed && !error;
    }
    catch (...)
    {
        return false;
    }
}

bool JsonConfigProvider::exists(
    const std::string& profileName
) const
{
    std::error_code error;

    const bool result =
        std::filesystem::exists(
            profilePath(profileName),
            error
        );

    return result && !error;
}

std::vector<std::string>
JsonConfigProvider::listProfiles() const
{
    std::vector<std::string> profiles;

    std::error_code error;

    if (
        !std::filesystem::exists(
            configDirectory_,
            error
        ) ||
        error
    )
    {
        return profiles;
    }

    for (
        const auto& entry :
        std::filesystem::directory_iterator(
            configDirectory_
        )
    )
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        const auto& path =
            entry.path();

        if (path.extension() == ".json")
        {
            profiles.push_back(
                path.stem().string()
            );
        }
    }

    std::sort(
        profiles.begin(),
        profiles.end()
    );

    return profiles;
}

std::filesystem::path
JsonConfigProvider::profilePath(
    const std::string& profileName
) const
{
    return configDirectory_
        / (
            sanitizeProfileName(
                profileName
            )
            + ".json"
        );
}

std::string
JsonConfigProvider::sanitizeProfileName(
    const std::string& profileName
)
{
    std::string result;

    result.reserve(
        profileName.size()
    );

    for (const char character : profileName)
    {
        const auto value =
            static_cast<unsigned char>(
                character
            );

        if (
            std::isalnum(value) ||
            character == '-' ||
            character == '_'
        )
        {
            result.push_back(
                character
            );
        }
        else
        {
            result.push_back('_');
        }
    }

    if (result.empty())
    {
        return "Default";
    }

    return result;
}
