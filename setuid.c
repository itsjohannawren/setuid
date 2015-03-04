#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

const char *user;
struct passwd *pw;

int main (int argc, char **argv, char **envp) {
	if (argc < 3) {
		fprintf (stderr, "Usage:\n  %s <USER> <COMMAND> [ARGS]\n", argv [0]);
		return (1);
	}
	
	user = *++argv;
	argv++;
	pw = getpwnam (user);
	if (! pw) {
		fprintf (stderr, "Error: Unknown user: %s\n", user);
		return (1);
	}

	if ((geteuid () != 0) && (geteuid () != pw->pw_uid)) {
		fprintf (stderr, "Error: Not running as root or the target user\n");
		return (1);
	}

	if (geteuid () == 0) {
		if (setgid (pw->pw_gid) == -1) {
			fprintf (stderr, "Error: Failed to set GID to %i: %s\n", (int) pw->pw_gid, strerror (errno));
			return (1);
		}
		if (setuid (pw->pw_uid) == -1) {
			fprintf (stderr, "Error: Failed to set UID to %i: %s\n", (int) pw->pw_uid, strerror (errno));
			return (1);
		}
	}

	execve (*argv, argv, envp);
	fprintf (stderr, "Error: Failed to run %s: %s\n", *argv, strerror (errno));
	return (1);
}
