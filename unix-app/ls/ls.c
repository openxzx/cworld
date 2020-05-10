#include "apue.h"

/* System include file, including opendir and readdir */
#include <dirent.h>

int main(int argc, char *argv[])
{
        DIR *dp;
        struct dirent *dirp;

        if (argc != 2) {
                printf("\033[0;31mUsage: ls directory_name.\033[0m\n");
                exit(1);
        }

        if ((dp = opendir(argv[1])) == NULL) {
                printf("\033[0;31mCan't open %s\033[0m\n", argv[1]);
                exit(1);
        }

        while ((dirp = readdir(dp)) != NULL)
                printf("\033[0;32m%s   \033[0m", dirp->d_name);
        printf("\n");

        closedir(dp);

        exit(0);
}
