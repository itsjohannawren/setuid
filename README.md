setuid
======

##### Simple wrapper to set the UID of a called program

Most of the time programs are launched by the user they should run as. Other times root launches them and they don't always know how to drop to a normal user. This fills that gap by running as root, changing to the specified user, and then exec'ing the real program.

Usage
-----

    setuid [-v] [<USER> <COMMAND> [ARGUMENTS]]

Example
-------

    setuid influxdb /opt/influxdb/influxd -config /etc/influxdb/config.toml

Options
-------

* `-v`: Show version information (Git branch, commit short SHA1, repo clean status)
* `<USER>`: Username to run the command as. Not the user's UID.
* `<COMMAND>`: Absolute path to the command you want to run. No `PATH` checking is done.
* `<ARGUMENTS>`: Any arguments you'd like to pass to `<COMMAND>`. Do not add a `--` unless you want it sent to `<COMMAND>`.

Notes
-----

The command specified must be a full path. `setuid` will not scan all of the directories in `PATH` for it.

If you are already running as the target user `setuid` will simply execute the desired program. If you're not that or root the program will error.
