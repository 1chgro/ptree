#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <libgen.h>

#define MAX_PATH 4096

void human_readable_size(off_t size, char *buf, size_t bufsize)
{
    double kb = (double)size / 1024.0;
    if (kb < 1.0) {
        snprintf(buf, bufsize, "%lld B", size);
    } else {
        snprintf(buf, bufsize, "%.1f KB", kb);
    }
}


void print_tree(const char *path, const char *name, const char *prefix, int is_last)
{
    char fullpath[MAX_PATH];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, name);

    struct stat st;
    if (lstat(fullpath, &st) == -1)
    {
        fprintf(stderr, "Error reading %s: %s\n", fullpath, strerror(errno));
        return;
    }

    printf("%s%s ", prefix, is_last ? "└──" : "├──");

    // Print folder or file name with size info for files
    if (S_ISDIR(st.st_mode))
        printf("%s\n", name);
    else
    {
        char sizebuf[20];
        human_readable_size(st.st_size, sizebuf, sizeof(sizebuf));
        printf("%s (%s)\n", name, sizebuf);
    }

    // If directory, recurse inside it
    if (S_ISDIR(st.st_mode))
    {
        DIR *dir = opendir(fullpath);
        if (!dir) {
            fprintf(stderr, "Cannot open directory %s: %s\n", fullpath, strerror(errno));
            return;
        }

        struct dirent **namelist;
        int n = scandir(fullpath, &namelist, NULL, alphasort);
        if (n < 0)
        {
            fprintf(stderr, "scandir error on %s: %s\n", fullpath, strerror(errno));
            closedir(dir);
            return;
        }

        int count = 0;
        for (int i = 0; i < n; i++)
        {
            if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                count++;
        }

        int printed = 0;
        for (int i = 0; i < n; i++)
        {
            if (strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0)
            {
                free(namelist[i]);
                continue;
            }
            printed++;

            char newprefix[1024];
            snprintf(newprefix, sizeof(newprefix), "%s%s", prefix, (is_last ? "    " : "│   "));
            print_tree(fullpath, namelist[i]->d_name, newprefix, printed == count);

            free(namelist[i]);
        }
        free(namelist);
        closedir(dir);
    }
}

const char *my_basename(const char *path)
{
    const char *base = strrchr(path, '/');
    return base ? base + 1 : path;
}

int main(int argc, char **argv) 
{
    char cwd[PATH_MAX];
    const char *root_path;

    if (argc > 1) {
        root_path = argv[1];
    } else {
        if (!getcwd(cwd, sizeof(cwd))) {
            perror("getcwd");
            return 1;
        }
        root_path = cwd;
    }

    char path_copy[PATH_MAX];
    strncpy(path_copy, root_path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';


    char *root_name = basename(path_copy);

    printf("%s/\n", root_name);

    DIR *dir = opendir(root_path);
    if (!dir)
    {
        fprintf(stderr, "Cannot open directory %s: %s\n", root_path, strerror(errno));
        return 1;
    }

    struct dirent **namelist;
    int n = scandir(root_path, &namelist, NULL, alphasort);
    if (n < 0)
    {
        fprintf(stderr, "scandir error on %s: %s\n", root_path, strerror(errno));
        closedir(dir);
        return 1;
    }

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
            count++;
    }

    int printed = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0)
        {
            free(namelist[i]);
            continue;
        }
        printed++;
        print_tree(root_path, namelist[i]->d_name, "", printed == count);
        free(namelist[i]);
    }

    free(namelist);
    closedir(dir);
    return 0;
}
