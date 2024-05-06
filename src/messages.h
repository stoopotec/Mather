#pragma once

#define TOSTRING(x) #x

#define E_RESET      "\e[0m"
#define E_BOLD       "\e[1m"
// #define E_DIM        "\e[2m"
#define E_ITALIC     "\e[3m"
#define E_UNDERLINE  "\e[4m"
#define E_BLINK      "\e[5m"
// #define E_RAPID_BLINK      "\e[6m"
#define E_INVERT     "\e[7m"
#define E_HIDE       "\e[8m"
#define E_CROSSED    "\e[9m"
#define E_ITALIC     "\e[3m"

    // cyan
    #define INFO "\e[38;2;17;168;205mINFO: \e[0m"

    // red
    #define ERR  "\e[38;2;255;0;0mERR:  \e[0m"

    // yellow
    #define WARN "\e[38;2;255;255;0mWARN: \e[0m"