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

#define E_FOREGROUND_BLACK "\033[30m"
#define E_BACKGROUND_BLACK "\033[40m"

#define E_FOREGROUND_RED "\033[31m"
#define E_BACKGROUND_RED "\033[41m"

#define E_FOREGROUND_GREEN "\033[32m"
#define E_BACKGROUND_GREEN "\033[42m"

#define E_FOREGROUND_YELLOW "\033[33m"
#define E_BACKGROUND_YELLOW "\033[43m"

#define E_FOREGROUND_BLUE "\033[34m"
#define E_BACKGROUND_BLUE "\033[44m"

#define E_FOREGROUND_MAGENTA "\033[35m"
#define E_BACKGROUND_MAGENTA "\033[45m"

#define E_FOREGROUND_CYAN "\033[36m"
#define E_BACKGROUND_CYAN "\033[46m"

#define E_FOREGROUND_WHITE "\033[37m"
#define E_BACKGROUND_WHITE "\033[47m"

#define E_FOREGROUND_GRAY "\033[38;5;90m"
#define E_BACKGROUND_GRAY "\033[48;5;100m"

#define E_FOREGROUND_BRIGHT_RED "\033[38;5;91m"
#define E_BACKGROUND_BRIGHT_RED "\033[48;5;101m"

#define E_FOREGROUND_BRIGHT_GREEN "\033[38;5;92m"
#define E_BACKGROUND_BRIGHT_GREEN "\033[48;5;102m"

#define E_FOREGROUND_BRIGHT_YELLOW "\033[38;5;93m"
#define E_BACKGROUND_BRIGHT_YELLOW "\033[48;5;103m"

#define E_FOREGROUND_BRIGHT_BLUE "\033[38;5;94m"
#define E_BACKGROUND_BRIGHT_BLUE "\033[48;5;104m"

#define E_FOREGROUND_BRIGHT_MAGENTA "\033[38;5;95m"
#define E_BACKGROUND_BRIGHT_MAGENTA "\033[48;5;105m"

#define E_FOREGROUND_BRIGHT_CYAN "\033[38;5;96m"
#define E_BACKGROUND_BRIGHT_CYAN "\033[48;5;106m"

#define E_FOREGROUND_BRIGHT_WHITE "\033[38;5;97m"
#define E_BACKGROUND_BRIGHT_WHITE "\033[48;5;107m"

// cyan
#define INFO "\033[38;2;17;168;205mINFO: \033[0m"

// red
#define ERR "\033[38;2;255;0;0mERR:  \033[0m"

// yellow
#define WARN "\033[38;2;255;255;0mWARN: \033[0m"