/*
 * Copyright (C) 2021 Julian Blaauboer
 * SPDX-License-Identifier: MIT
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <pwd.h>

#include "config.h"

static const char default_dir[] = DB_DIR "/" NAME; 

static void version(void) {
	fprintf(stderr, NAME " " VERSION "\n");
	fprintf(stderr, "Copyright (C) 2021 Julian Blaauboer\n");
}

static void usage(const char *name) {
	fprintf(stderr, NAME " - a simple punch clock utility\n");
	fprintf(stderr, "Usage: %s [OPTION]... { in | out } [tags]\n\n", name);
	fprintf(stderr, "  -f  --file=file        append to a different file (instead of \"%s/<user>\")\n", default_dir);
	fprintf(stderr, "  -c  --comment=comment  set the comment field\n");
	fprintf(stderr, "  -V  --version          display the version information and exit\n");
	fprintf(stderr, "  -h  --help             display this help message and exit\n");
}

static void drop_privileges(void) {
	gid_t gid = getgid();
	uid_t uid = getuid();
	
	if (setregid(gid, gid) == -1) {
		perror("setregid");
		exit(EXIT_FAILURE);
	}

	if (setreuid(uid, uid) == -1) {
		perror("setreuid");
		exit(EXIT_FAILURE);
	}
}

static char *default_path(void) {
	struct passwd *pw = getpwuid(getuid());
	char *path;

	if (!pw) {
		perror("getpwuid");
		exit(EXIT_FAILURE);
	}

	path = malloc(sizeof(default_dir) + strlen(pw->pw_name) + 1);
	sprintf(path, "%s/%s", default_dir, pw->pw_name);

	return path;
}

static int validate(const char *tags) {
	char c;

	while ((c = *tags++)) {
		if (c == '\t' || c == '\n') {
			return 0;
		}
	}
	
	return 1;
}

int main(int argc, char **argv) {
	char *name = NAME, *path = NULL, *tags = "", *comment = "";
	FILE *file;

	if (argv[0]) {
		name = argv[0];
	}

	for (;;) {
		static struct option options[] = {
			{   "file", required_argument, NULL, 'f'},
			{"comment", required_argument, NULL, 'c'},
			{"version",       no_argument, NULL, 'V'},
			{   "help",       no_argument, NULL, 'h'},
			{ NULL, 0, NULL, 0},
		};

		switch (getopt_long(argc, argv, "f:c:Vh", options, NULL)) {
			case -1:
				break;
			case 'f':
				path = optarg;
				continue;
			case 'c':
				comment = optarg;
				continue;
			case 'V':
				version();
				exit(EXIT_SUCCESS);
			case 'h':
				usage(name);
				exit(EXIT_SUCCESS);
			default:
				usage(name);
				exit(EXIT_FAILURE);
		}

		break;
	}

	if (optind + 1 > argc) {
		fprintf(stderr, "%s: too few arguments\n", name);
		usage(name);
		exit(EXIT_FAILURE);
	}

	if (optind + 1 < argc) {
		tags = argv[optind + 1];
	}

	if (optind + 2 < argc) {
		fprintf(stderr, "%s: too many arguments\n", name);
		usage(name);
		exit(EXIT_FAILURE);
	}

	if (!(!strcmp(argv[optind], "in") || !strcmp(argv[optind], "out"))) {
		fprintf(stderr, "%s: invalid action \"%s\"\n", name, argv[optind]);
		usage(name);
		exit(EXIT_FAILURE);
	}

	if (!validate(tags)) {
		fprintf(stderr, "%s: tags contain tabs or newlines\n", name);
		usage(name);
		exit(EXIT_FAILURE);
	}

	if (!validate(comment)) {
		fprintf(stderr, "%s: comment contains tabs or newlines\n", name);
		usage(name);
		exit(EXIT_FAILURE);
	}

	if (path) {
		drop_privileges();
	} else {
		path = default_path();
	}

	/* Binary mode is used because we want newlines to stay newlines. */
	if (!(file = fopen(path, "ab"))) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	/* There is no format specifier for time_t, so we use long instead. */
	fprintf(file, "%ld\t%s\t%s\t%s\n", (long)time(NULL), argv[optind], tags, comment);
	fclose(file);

	exit(EXIT_SUCCESS);
}
