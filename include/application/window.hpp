#pragma once
#include <string_view>

enum class WindowType {
    Default = 0,
    Fullscreen,
    Borderless,
};

class SurfaceOptions {
public:
    std::string_view window_name = "phase";
    int window_width = 1024;
    int window_height = 768;
    int window_x = 0;
    int window_y = 0;

    WindowType window_type = WindowType::Default;

    SurfaceOptions() = default;

    SurfaceOptions& width(int width) {
        window_width = width;
        return *this;
    }

    SurfaceOptions& height(int height) {
        window_height = height;
        return *this;
    }

    SurfaceOptions& x(int x) {
        window_x = x;
        return *this;
    }

    SurfaceOptions& y(int y) {
        window_y = y;
        return *this;
    }

    SurfaceOptions& type(WindowType type) {
        window_type = type;
        return *this;
    }

    SurfaceOptions& title(std::string_view title) {
        window_name = title;
        return *this;
    }

    ~SurfaceOptions() = default;
};

struct SurfaceId {
    unsigned int id;

    explicit SurfaceId(const unsigned int id_) : id(id_) {}

    struct hash {
        std::size_t operator()(const SurfaceId& inst) const noexcept {
            return std::hash<decltype(inst.id)>()(inst.id);
        }
    };

    bool operator==(const SurfaceId& other) const { return other.id == id; }
};
