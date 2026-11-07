/*
 * Name:       Mia Siedentopf
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    121
 * File:       finfo.c
 *
 * Part 3: File management system calls.
 * Usage: finfo <filename>
 * Prints the file type, permissions, owner id, size in bytes, and time of
 * last modification.
 *
 * stat() is used because we have a file NAME (fstat needs an open file
 * descriptor) and we want information about the file itself, following
 * any symbolic link (lstat would describe the link).
 */

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TIME_LEN 64

/* Return a readable name for the file type in a st_mode value. */
static const char *file_type(mode_t mode)
{
    if (S_ISREG(mode))  return "regular file";
    if (S_ISDIR(mode))  return "directory";
    if (S_ISLNK(mode))  return "symbolic link";
    if (S_ISCHR(mode))  return "character device";
    if (S_ISBLK(mode))  return "block device";
    if (S_ISFIFO(mode)) return "FIFO (named pipe)";
    if (S_ISSOCK(mode)) return "socket";
    return "unknown";
}

/* Build a string like "rwxr-xr--" from the permission bits. */
static void permission_string(mode_t mode, char out[10])
{
    out[0] = (mode & S_IRUSR) ? 'r' : '-';
    out[1] = (mode & S_IWUSR) ? 'w' : '-';
    out[2] = (mode & S_IXUSR) ? 'x' : '-';
    out[3] = (mode & S_IRGRP) ? 'r' : '-';
    out[4] = (mode & S_IWGRP) ? 'w' : '-';
    out[5] = (mode & S_IXGRP) ? 'x' : '-';
    out[6] = (mode & S_IROTH) ? 'r' : '-';
    out[7] = (mode & S_IWOTH) ? 'w' : '-';
    out[8] = (mode & S_IXOTH) ? 'x' : '-';
    out[9] = '\0';
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (stat(argv[1], &st) == -1)
    {
        perror(argv[1]);
        return 1;
    }

    char perms[10];
    permission_string(st.st_mode, perms);

    /* Format the modification time as a readable date and time. */
    char when[TIME_LEN];
    struct tm *tm_info = localtime(&st.st_mtime);
    if (tm_info == NULL ||
        strftime(when, sizeof(when), "%Y-%m-%d %H:%M:%S", tm_info) == 0)
    {
        fprintf(stderr, "Error: cannot format modification time\n");
        return 1;
    }

    printf("File:              %s\n", argv[1]);
    printf("Type:              %s\n", file_type(st.st_mode));
    printf("Permissions:       %s (%03o)\n", perms,
           (unsigned)(st.st_mode & 0777));
    printf("Owner (uid):       %u\n", (unsigned)st.st_uid);
    printf("Size (bytes):      %lld\n", (long long)st.st_size);
    printf("Last modified:     %s\n", when);

    return 0;
}