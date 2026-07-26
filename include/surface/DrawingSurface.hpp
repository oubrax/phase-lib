#pragma once
#include <string_view>

class DrawingSurface {
public:
    virtual void init()         = 0;
    virtual void destroy()      = 0;
    virtual void swap_buffers() = 0;
    virtual void poll_events()   = 0;
    virtual bool should_close()  = 0;

    virtual void create_surface(
        std::string_view name,
        int width,
        int height) = 0;

    virtual ~DrawingSurface() = default;
};