#pragma once

#define TOSTRING(x) #x

#define E_RESET      "\033[0m"
#define E_BOLD       "\033[1m"
// #define E_DIM        "\033[2m"
#define E_ITALIC     "\033[3m"
#define E_UNDERLINE  "\033[4m"
#define E_BLINK      "\033[5m"
// #define E_RAPID_BLINK      "\033[6m"
#define E_INVERT     "\033[7m"
#define E_HIDE       "\033[8m"
#define E_CROSSED    "\033[9m"
#define E_ITALIC     "\033[3m"


#define E_FOREGROUND_RGB(r, g, b) "\033[38;2;"#r";"#g";"#b"m"
#define E_BACKGROUND_RGB(r, g, b) "\033[48;2;"#r";"#g";"#b"m"

    // cyan
    #define INFO "\033[38;2;17;168;205mINFO: \033[0m"

    // red
    #define ERR  "\033[38;2;255;0;0mERR:  \033[0m"

    // yellow
    #define WARN "\033[38;2;255;255;0mWARN: \033[0m"