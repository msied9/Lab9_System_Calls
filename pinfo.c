/*
 * Name:       Mia Siedentopf
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    121
 * File:       pinfo.c
 *
 * Part 2a: Process information.
 * Usage: pinfo [pid]
 * Prints the priority and scheduling method (as text) of the given process.
 * With no argument, reports on the current process.
 *
 * Related calls researched: getpid (own process id), getpriority (nice
 * value), sched_getscheduler (scheduling policy), getcpu (CPU running the
 * caller), getrusage (resource usage such as CPU time), getrlimit
 * (resource limits such as max open files).
 */

/* Needed BEFORE any header so SCHED_BATCH and SCHED_IDLE are defined. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>

/* Convert a scheduling policy number to readable text. */
static const char *policy_name(int policy)
{
    switch (policy)
    {
        case SCHED_OTHER: return "SCHED_OTHER (normal time-sharing)";
        case SCHED_FIFO:  return "SCHED_FIFO (real-time, first in first out)";
        case SCHED_RR:    return "SCHED_RR (real-time, round robin)";
        case SCHED_BATCH: return "SCHED_BATCH (batch)";
        case SCHED_IDLE:  return "SCHED_IDLE (very low priority)";
        default:          return "unknown";
    }
}

int main(int argc, char* argv[])
{
    pid_t pid;

    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [pid]\n", argv[0]);
        return 1;
    }
    else if (argc == 2)
    {
        /* strtol lets us detect text that is not a number. */
        char *end;
        errno = 0;
        long value = strtol(argv[1], &end, 10);
        if (errno != 0 || *end != '\0' || end == argv[1] || value <= 0)
        {
            fprintf(stderr, "Error: '%s' is not a valid process id\n",
                    argv[1]);
            return 1;
        }
        pid = (pid_t)value;
    }
    else
    {
        pid = getpid();   /* no argument: use the current process */
    }

    /* getpriority can legitimately return -1, so clear errno first and
     * check it afterward to tell a real error from a real value. */
    errno = 0;
    int priority = getpriority(PRIO_PROCESS, pid);
    if (priority == -1 && errno != 0)
    {
        fprintf(stderr, "Error: cannot get priority of process %d: %s\n",
                (int)pid, errno == ESRCH ? "no such process"
                                         : "unable to read priority");
        return 1;
    }

    int policy = sched_getscheduler(pid);
    if (policy == -1)
    {
        fprintf(stderr, "Error: cannot get scheduler of process %d: %s\n",
                (int)pid, errno == ESRCH ? "no such process"
                                         : "unable to read scheduler");
        return 1;
    }

    printf("Process id:        %d\n", (int)pid);
    printf("Priority (nice):   %d\n", priority);
    printf("Scheduling method: %s\n", policy_name(policy));

    return 0;
}