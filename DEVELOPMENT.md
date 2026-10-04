# Development

Use Linux or WSL with Python 3.12, GCC, Bash, awk and sed. Run `python
tools/check_repository.py` to compile the independent C programs, then `bash -n`
on shell scripts before running them with their documented arguments. GitHub
Actions uses Linux for the POSIX process-control program; MinGW on Windows
does not supply fork/wait semantics. Shell entry points are stored with LF endings.

These exercises manipulate files and spawn processes. Run examples in a scratch
directory with synthetic files. Inspect required arguments and relative paths
in each exercise before execution. A compile/syntax check is not a behavioral
test of every shell command or binary record format.
