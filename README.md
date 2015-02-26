setuid
======

##### Simple wrapper to set the UID of a called program

Most of the time programs are launched by the user they should run as. Other times root launches them and they don't always know how to drop to a normal user. This fills that gap by running as root, changing to the specified user, and then exec'ing the real program.

Usage
-----

    setuid <USER> <COMMAND> [ARGUMENTS]

Example
-------

    setuid influxdb /opt/influxdb/influxd -config /etc/influxdb/config.toml

Notes
-----

The command specified must be a full path. `setuid` will not scan all of the directories in `PATH` for it.
