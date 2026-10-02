/*
 * PS5 RetroArch - the libc functions the title's own code and its libc++ link
 * by their standard names, where the console has none that works: gmtime_r
 * and utimensat. The implementations are the platform layer's (my payload SDK
 * fork, include/ps5platform/libc.h); these give them the standard names inside
 * the title. A core imports them through tools/core-imports.py instead.
 *
 * Also: ASCII ctype. Under __SCE__ the FreeBSD ctype.h macros are disabled and
 * the title imports isgraph/isspace/... from libSceLibcInternal. Those imports
 * are empty stubs in the public SDK; on Kyty they return 0. RetroArch's
 * config_file parser uses isgraph to read keys, so every retroarch.cfg line
 * failed to parse (0 entries) and video_driver/menu_driver never left the
 * compiled defaults. These ASCII answers match the C locale and do not need
 * the rune table the stubs never provide.
 *
 * Copyright (C) 2026 Mihawk
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <ps5platform/libc.h>

/* PPSSPP's FFmpeg (libavutil's time formatting) calls the reentrant form; the
 * console's libc has only gmtime. */
struct tm *gmtime_r(const time_t *time, struct tm *result)
{
    return ps5_gmtime_r(time, result);
}

/* libc++'s std::filesystem::last_write_time(path, time) sets a file's times
 * with it, and Dolphin's file utilities link that setter. */
int utimensat(int directory, const char *path, const struct timespec times[2], int flags)
{
    return ps5_utimensat(directory, path, times, flags);
}

/* --- ASCII ctype (C locale). See file comment above. --- */

int isgraph(int c)
{
    unsigned char u = (unsigned char)c;
    return u > 0x20 && u < 0x7f;
}

int isprint(int c)
{
    unsigned char u = (unsigned char)c;
    return u >= 0x20 && u < 0x7f;
}

int isspace(int c)
{
    unsigned char u = (unsigned char)c;
    return u == ' ' || (u >= '\t' && u <= '\r');
}

int isdigit(int c)
{
    unsigned char u = (unsigned char)c;
    return u >= '0' && u <= '9';
}

int isalpha(int c)
{
    unsigned char u = (unsigned char)c;
    return (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z');
}

int isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}

int islower(int c)
{
    unsigned char u = (unsigned char)c;
    return u >= 'a' && u <= 'z';
}

int isupper(int c)
{
    unsigned char u = (unsigned char)c;
    return u >= 'A' && u <= 'Z';
}

int iscntrl(int c)
{
    unsigned char u = (unsigned char)c;
    return u < 0x20 || u == 0x7f;
}

int ispunct(int c)
{
    return isgraph(c) && !isalnum(c);
}

int isxdigit(int c)
{
    unsigned char u = (unsigned char)c;
    return isdigit(c) || (u >= 'A' && u <= 'F') || (u >= 'a' && u <= 'f');
}

int isblank(int c)
{
    unsigned char u = (unsigned char)c;
    return u == ' ' || u == '\t';
}
