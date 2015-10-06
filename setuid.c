#include <errno.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef GIT_SHA1
#define GIT_SHA1 "(unknown)"
#endif

#ifndef GIT_BRANCH
#define GIT_BRANCH "(unknown)"
#endif

#ifndef GIT_UNCLEAN
#define GIT_UNCLEAN -1
#endif

int main (int argc, char **argv, char **envp) {
	const char *user;
	struct passwd *pw;
	struct stat st;
	char procPath[PATH_MAX + 1], ourPath[PATH_MAX + 1];
	size_t ourPathLength;

	// Check arguments
	if ((argc == 2) && (strncmp (argv [1], "-v", 2) == 0)) {
		// Version requested
		fprintf (stderr, "setuid: %s@%s ", GIT_BRANCH, GIT_SHA1);
		if (GIT_UNCLEAN == -1) {
			fprintf (stderr, "unknown state\n");
		} else if (GIT_UNCLEAN == 0) {
			fprintf (stderr, "clean\n");
		} else {
			fprintf (stderr, "%i file%s unclean\n", GIT_UNCLEAN, (GIT_UNCLEAN == 1 ? "" : "s"));
		}
		return (0);
	} else if (argc < 3) {
		// Missing a few things
		fprintf (stderr, "Usage:\n  %s [-v] [<USER> <COMMAND> [ARGS]]\n", argv [0]);
		return (1);
	}

	// Store the user
	user = *++argv;
	// Advance argv to the next argument
	argv++;
	// Retrieve the user
	pw = getpwnam (user);
	if (! pw) {
		// Well, that didn't go as planned
		fprintf (stderr, "Error: Unknown user: %s\n", user);
		return (1);
	}

	// Check for the setuid bit
	// Empty the string for our proc path
	memset (procPath, 0, PATH_MAX + 1);
	// Create a string for our proc path
	sprintf (procPath, "/proc/%u/exe", getpid ());
	// Empty the string for our real path
	memset (ourPath, 0, PATH_MAX + 1);
	// Read our real path
	ourPathLength = readlink (procPath, ourPath, PATH_MAX);
	if (ourPathLength < 0) {
		// Well hells bells
		fprintf (stderr, "Security: Not running! Failed to read our own path\n");
		return (1);
	}
	// Stat our real path to get our mode
	if (stat (ourPath, &st)) {
		// Cute
		fprintf (stderr, "Security: Not running! Failed to stat ourself\n");
		return (1);
	} else if (st.st_mode & 04000) {
		// What we don't want
		fprintf (stderr, "Security: Not running! Our setuid bit is set\n");
		return (1);
	}

	// Ensure the call is root or the target user
	if ((geteuid () != 0) && (geteuid () != pw->pw_uid)) {
		// Ehhhhh, NOPE
		fprintf (stderr, "Security: Not running! You are not root or the target user\n");
		return (1);
	}

	// Only have to do this if they're root
	if (geteuid () == 0) {
		// Set our group ID
		if (setgid (pw->pw_gid) == -1) {
			// Ok, that's not right
			fprintf (stderr, "Error: Failed to set GID to %i: %s\n", (int) pw->pw_gid, strerror (errno));
			return (1);
		}
		// Set our user ID
		if (setuid (pw->pw_uid) == -1) {
			// And again with the failure thing
			fprintf (stderr, "Error: Failed to set UID to %i: %s\n", (int) pw->pw_uid, strerror (errno));
			return (1);
		}
	}

	// Become what the user wants
	execve (*argv, argv, envp);

	// Haha, we couldn't do it
	fprintf (stderr, "Error: Failed to run %s: %s\n", *argv, strerror (errno));
	return (1);
}
