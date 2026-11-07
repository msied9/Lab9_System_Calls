/*
 * Name:       Mia Siedentopf
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    121
 * File:       info.c
 *
 * Part 1: Information maintenance system calls.
 * Prints information about the running system:
 *   - time of day in nanoseconds   (clock_gettime)
 *   - network name                 (gethostname)
 *   - OS name, release, version, hardware type (uname)
 *   - number of CPUs               (get_nprocs)
 *   - total / free physical memory in bytes (sysconf, getpagesize)
 * Any string value that comes back empty is NOT printed.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

#define NANOS_PER_SEC 1000000000LL
#define HOST_LEN 256

/* Print "label: value" only if value is not an empty string. */
static void print_if_set(const char *label, const char *value)
{
    if (value != NULL && value[0] != '\0')
    {
        printf("%s: %s\n", label, value);
    }
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    /* Time of day: clock_gettime fills a timespec (seconds + nanoseconds).
     * CLOCK_REALTIME is the wall-clock time since the Unix epoch.
     * Combine both fields into one total number of nanoseconds. */
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
    {
        long long nanos = (long long)ts.tv_sec * NANOS_PER_SEC + ts.tv_nsec;
        printf("Current time (ns since epoch): %lld\n", nanos);
    }
    else
    {
        perror("clock_gettime");
    }

    /* Network name of this machine. */
    char host[HOST_LEN];
    if (gethostname(host, sizeof(host)) == 0)
    {
        host[sizeof(host) - 1] = '\0';   /* guarantee termination */
        print_if_set("Network name", host);
    }
    else
    {
        perror("gethostname");
    }

    /* uname fills a struct with OS name, release, version, hardware type. */
    struct utsname u;
    if (uname(&u) == 0)
    {
        print_if_set("OS name", u.sysname);
        print_if_set("OS release", u.release);
        print_if_set("OS version", u.version);
        print_if_set("Hardware type", u.machine);
    }
    else
    {
        perror("uname");
    }

    /* Number of processors currently available. */
    printf("Number of CPUs: %d\n", get_nprocs());

    /* Memory: sysconf reports PAGES, so multiply by the page size
     * (getpagesize) to get bytes. Use long long to avoid overflow. */
    long long page = getpagesize();
    long long total_pages = sysconf(_SC_PHYS_PAGES);
    long long free_pages = sysconf(_SC_AVPHYS_PAGES);

    if (total_pages >= 0)
    {
        printf("Total physical memory (bytes): %lld\n", total_pages * page);
    }
    if (free_pages >= 0)
    {
        printf("Free memory (bytes): %lld\n", free_pages * page);
    }

    return 0;
}