#pragma once
#include "application/window.hpp"

#include <string_view>

// This abstract class defines the window-agnostic surface API, only handles creation and data/event
// functionality, while letting the actual windows handles stored in the application / renderer.
class DrawingSurface {
public:
    virtual void init() = 0;
    virtual void destroy(WindowHandle handle) = 0;
    virtual void swap_buffers(WindowHandle handle) = 0;
    virtual void poll_events(WindowHandle handle) = 0;
    virtual bool should_close(WindowHandle handle) = 0;

    virtual WindowHandle create_surface(const WindowOptions& options) = 0;

    virtual ~DrawingSurface() = default;
};
