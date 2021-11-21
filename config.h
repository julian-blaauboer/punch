#ifndef CONFIG_H
#define CONFIG_H

/*
 * Configuration file for punch
 *
 * This header file is not only included in punch.c, but also parsed by the
 * shell scripts under tools/. It is therefore required that the macros only
 * consist of a single string literal. Doing it this way centralizes all
 * configuration to just this file.
 */

/*
 * NAME is the name of the executable, man page and database directory.
 */
#define NAME "punch"

/*
 * VERSION is the semver version of the executable.
 */
#define VERSION "0.1.0"

/*
 * PREFIX is the prefix applied to BIN_DIR and MAN_DIR (but not DB_DIR),
 * normally this is either "/usr" for packaged executables or "/usr/local"
 * for manually installed programs.
 */
#define PREFIX "/usr"

/*
 * BIN_DIR is the directory where binaries are installed, this is almost always
 * "/bin".
 */
#define BIN_DIR "/bin"

/*
 * MAN_DIR is the directory where man(1) expects the directories for man pages.
 * Usually this is "/share/man".
 */
#define MAN_DIR "/share/man"

/*
 * DB_DIR is the directory under which the database directory should exist.
 * This is usually "/var/lib" for Linux and "/var/db" for *BSD.
 */
#define DB_DIR "/var/lib"

#endif
