#pragma once
#include <string>

struct WindowConfiguration {
    int width = 800;
    int height = 600;
    char* title = "T";
};
struct Configuration {
    WindowConfiguration window;
    int frameLimit = 60;
};
