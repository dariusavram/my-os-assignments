#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void list_directory(char *dir_path, int recursive, int min_size, char *name_ends_with) {

    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(dir_path);

    if (dir == NULL) {
        fprintf(stderr, "ERROR\ninvalid directory path\n");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (entry->d_type == DT_DIR) {
                if (recursive) {
                    char path[1024];
                    snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
                    list_directory(path, 1, min_size, name_ends_with);
                }
                if (name_ends_with == NULL || name_ends_with[0] == '\0' || strstr(entry->d_name, name_ends_with) != NULL) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                }
            } else if (entry->d_type == DT_REG) {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
                if (stat(full_path, &st) == -1) {
                    perror("stat");
                    continue;
                }
                if (st.st_size > min_size && (name_ends_with == NULL || name_ends_with[0] == '\0' || (strlen(entry->d_name) >= strlen(name_ends_with) && strcmp(entry->d_name + strlen(entry->d_name) - strlen(name_ends_with), name_ends_with) == 0))) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                }
            } else {
                if (name_ends_with != NULL && strstr(entry->d_name, name_ends_with) != NULL) {
                    printf("%s/%s\n", dir_path, entry->d_name);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {

    char *dir_path = NULL;

    int recursive = 0;

    int min_size = 0;

    char *name_ends_with = NULL;

    if (argc >= 2) {

        if (strcmp(argv[1], "variant") == 0) {

            printf("48169\n");

            return 0;

        }

        for (int i = 1; i < argc; i++) {

            if (strncmp(argv[i], "path=", 5) == 0) {

                dir_path = argv[i] + 5;

            } else if (strcmp(argv[i], "recursive") == 0) {

                recursive = 1;

            } else if (strncmp(argv[i], "size_greater=", 13) == 0) {

                min_size = atoi(argv[i] + 13);

            } else if (strncmp(argv[i], "name_ends_with=", 15) == 0) {

                name_ends_with = argv[i] + 15;

            }

        }

        if (dir_path != NULL) {

            list_directory(dir_path, recursive, min_size, name_ends_with);

        } else {

            fprintf(stderr, "ERROR\npath argument is missing\n");

            exit(EXIT_FAILURE);

        }

    } else {

        fprintf(stderr, "ERROR\nnot enough arguments\n");

        exit(EXIT_FAILURE);

    }

    printf("SUCCESS\n");

    return 0;

}
