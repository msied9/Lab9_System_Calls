/*
 * Name:       Mia Siedentopf
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    121
 * File:       pmod.c
 *
 * Part 2b: Altering a process.
 * Lowers this process's priority by 10, sleeps for 1,837,272,638 ns,
 * then prints a goodbye message.
 *
 * nice(n) ADDS n to the nice value. A higher nice value means a LOWER
 * priority, so nice(10) reduces the priority (nice(-10) would raise it).
 * (man nice = shell command, man 2 nice = the system call used here.)
 * nanosleep suspends the process for the requested time.
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    /* nice can legitimately return -1, so check errno instead. */
    errno = 0;
    int new_nice = nice(10);
    if (new_nice == -1 && errno != 0)
    {
        perror("nice");
        return 1;
    }
    printf("New nice value: %d (priority reduced)\n", new_nice);

    /* 1 second + 837,272,638 ns = 1,837,272,638 ns total.
     * tv_nsec must stay below 1,000,000,000. */
    struct timespec req = { .tv_sec = 30, .tv_nsec = 837272638L };
    struct timespec rem;

    /* If a signal interrupts the sleep, nanosleep stores the time left
     * in rem; keep sleeping for the remainder. */
    while (nanosleep(&req, &rem) == -1 && errno == EINTR)
    {
        req = rem;
    }

    printf("Goodbye!\n");
    return 0;
}