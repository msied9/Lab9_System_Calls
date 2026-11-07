# Lab 9: System Calls


CPE-2600 Systems Programming, Fall 2026
Name: Mia Siedentopf

## Description
Four small C programs that use POSIX system calls directly.
## Build
- Build everything: `make`
- Remove built files: `make clean`

## Programs
1. `info`: prints system information.
 - Time of day in nanoseconds
- Network name
- OS name, release, version and hardware type
- Number of CPUs
- Total and free physical memory in bytes
2. `pinfo [pid]`: prints the priority and scheduling method of a process.
- With no argument it reports on itself.
- An invalid or nonexistent pid prints an error.
3. `pmod`: lowers its own priority by 10 with `nice`, sleeps for
   1,837,272,638 nanoseconds with `nanosleep`, then says goodbye.
4. `finfo <file>`: prints file information.
 - File type
  - Permissions
  - Owner user id
- Size in bytes
- Last modification time

## Usage Examples
./info
./pinfo
./pinfo 1
./pmod
./finfo Makefile
