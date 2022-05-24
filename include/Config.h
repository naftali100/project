#pragma once
#ifndef CONFIG_H
#define CONFIG_H

// ==============================
// settings
#define WIN_SIZE_X 800
#define WIN_SIZE_Y 600
// #define WIN_SIZE_X 1920
// #define WIN_SIZE_Y 1080

#define FPS 60

#if defined(_WIN32) || defined(__CYGWIN__)
// windows
#define FONT_PATH "C:/Windows/Fonts/Arial.ttf"
#elif defined(__linux__)
// Linux
#define FONT_PATH "/usr/share/fonts/noto/NotoSansMono-Regular.ttf"
#endif

#endif