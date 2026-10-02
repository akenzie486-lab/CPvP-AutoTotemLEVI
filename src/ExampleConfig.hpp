
#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <pl/Config.hpp>

namespace fullcppmod {

enum class DisplayMode {
    Compact,
    Detailed,
    Debug
};

struct ExampleConfig {
    int version = 1;
    bool showOverlay = true;
    int opacity = 80;
    double scale = 1.0;
    DisplayMode mode = DisplayMode::Compact;
    std::string accentColor = "#4AE0A0";
};

inline constexpr int kMinOpacity = 0;
inline constexpr int kMaxOpacity = 100;
inline constexpr double kMinScale = 0.5;
inline constexpr double kMaxScale = 2.0;

inline constexpr std::string_view kModeMenuOptions =
    "Compact,Detailed,Debug";

}

namespace pl::config {

template <>
struct Schema<fullcppmod::ExampleConfig> {
    static constexpr std::string_view title =
        "CPvP AutoTotem";

    static constexpr std::string_view description =
        "CPvP AutoTotem configuration.";

    static constexpr FieldSchema field(std::string_view name) {
        if (name == "version") {
            return {"Version", "Config version",
                    std::nullopt, std::nullopt, true};
        }

        if (name == "showOverlay") {
            return {"Show Overlay", "Overlay visibility",
                    std::nullopt, std::nullopt, false};
        }

        if (name == "opacity") {
            return {"Opacity", "Overlay opacity",
                    fullcppmod::kMinOpacity,
                    fullcppmod::kMaxOpacity, false};
        }

        if (name == "scale") {
            return {"Scale", "Overlay scale",
                    fullcppmod::kMinScale,
                    fullcppmod::kMaxScale, false};
        }

        if (name == "mode") {
            return {"Display Mode", "Display mode",
                    std::nullopt, std::nullopt, false};
        }

        if (name == "accentColor") {
            return {"Accent Color", "Overlay color",
                    std::nullopt, std::nullopt, false};
        }

        return {};
    }
};

}
