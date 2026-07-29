#pragma once
#include <string_view>

enum class WindowType {
    Default = 0,
    Fullscreen,
    Borderless,
};

class WindowOptions {
public:
    std::string_view window_name = "phase";
    int window_width = 1024;
    int window_height = 768;
    int window_x = 0;
    int window_y = 0;

    WindowType window_type = WindowType::Default;

    WindowOptions() = default;

    WindowOptions &width(int width) {
        window_width = width;
        return *this;
    }

    WindowOptions &height(int height) {
        window_height = height;
        return *this;
    }

    WindowOptions &x(int x) {
        window_x = x;
        return *this;
    }

    WindowOptions &y(int y) {
        window_y = y;
        return *this;
    }

    WindowOptions &type(WindowType type) {
        window_type = type;
        return *this;
    }


    WindowOptions &title(std::string_view title) {
        window_name = title;
        return *this;
    }


    ~WindowOptions() = default;
};


struct WindowId {
    unsigned int id;

    explicit WindowId(const unsigned int id_): id(id_) {}

    struct hash {
        std::size_t operator()(const WindowId &inst) const noexcept {
            return std::hash<decltype(inst.id)>()(inst.id);
        }
    };

    bool operator==(const WindowId& other) const {
        return other.id == id;
    }
};