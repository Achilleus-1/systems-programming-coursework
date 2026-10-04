# Development

Use Linux or WSL with Python 3.12, GCC, Bash, awk and sed. Run `python
tools/check_repository.py` to compile the independent C programs, then `bash -n`
on shell scripts before running them with their documented arguments. GitHub
Actions uses Linux for the POSIX process-control program; MinGW on Windows
does not supply fork/wait semantics. Shell entry points are stored with LF endings.

The process-control demo accepts up to six comma-separated commands and splits
arguments on whitespace, without shell quoting or expansion. Each child receives
only its own bounded command through a separate pipe. Writes use the command's
actual length, and child execution failures produce a nonzero exit status. Linux
CI exercises multiple commands, long argument lists, invalid input and failures.

These exercises manipulate files and spawn processes. Run examples in a scratch
directory with synthetic files. Inspect required arguments and relative paths
in each exercise before execution. A compile/syntax check is not a behavioral
test of every shell command or binary record format.
