#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/filesystem.h"

void display_menu() {
    printf("\n=== Mini File System ===\n");
    printf("1. Create Directory (mkdir)\n");
    printf("2. Change Directory (cd)\n");
    printf("3. Create File (touch)\n");
    printf("4. Write to File (echo)\n");
    printf("5. Read File (cat)\n");
    printf("6. Copy File (cp)\n");
    printf("7. Move/Rename File (mv)\n");
    printf("8. Delete File (rm)\n");
    printf("9. Change Permissions (chmod)\n");
    printf("10. Delete Directory (rmdir)\n");
    printf("0. Exit\n");
    printf("Select an option: ");
}

int main() {
    int choice;
    if (init_file_system() != 0) {
        fprintf(stderr, "Failed to initialize MiniFS.\n");
        return 1;
    }

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char dirname[256];
                printf("Enter directory name: ");
                scanf("%s", dirname);
                int result = mini_mkdir(dirname);
                printf("mkdir returned: %d\n", result);
                break;
            }
            case 2: {
                char path[256];
                printf("Enter directory: ");
                scanf("%s", path);
                int result = cd(path);
                printf("cd returned: %d\n", result);
                break;
            }
            case 3: {
                char filename[256];
                printf("Enter file name: ");
                scanf("%s", filename);
                int result = touch(filename);
                printf("touch returned: %d\n", result);
                break;
            }
            case 4: {
                char filename[256];
                char content[1024];
                printf("Enter file name: ");
                scanf("%s", filename);

                printf("Enter content (use _ for spaces): ");
                scanf("%s", content);

                // substituir _ por espaço
                for (int i = 0; i < strlen(content); i++) {
                    if (content[i] == '_')
                        content[i] = ' ';
                }

                int result = echo(filename, content);
                printf("echo returned: %d\n", result);
                break;
            }
            case 5: {
                char filename[256];
                printf("Enter file name: ");
                scanf("%s", filename);
                printf("Content:\n");
                int result = cat(filename);
                printf("\ncat returned: %d\n", result);
                break;
            }
            case 6: {
                char src[256];
                char dst[256];
                printf("Source: ");
                scanf("%s", src);
                printf("Destination: ");
                scanf("%s", dst);
                int result = cp(src, dst);
                printf("cp returned: %d\n", result);
                break;
            }
            case 7: {
                char src[256];
                char dst[256];
                printf("Source: ");
                scanf("%s", src);
                printf("Destination: ");
                scanf("%s", dst);
                int result = mv(src, dst);
                printf("mv returned: %d\n", result);
                break;
            }
            case 8: {
                char filename[256];
                printf("File to delete: ");
                scanf("%s", filename);
                int result = rm(filename);
                printf("rm returned: %d\n", result);
                break;
            }
            case 9: {
                char filename[256];
                int perm;
                printf("File name: ");
                scanf("%s", filename);
                printf("Permissions (octal, ex: 755): ");
                scanf("%o", &perm);
                int result = mini_chmod(filename, perm);
                printf("chmod returned: %d\n", result);
                break;
            }

            case 10: {
                char dirname[256];
                printf("Directory to delete: ");
                scanf("%s", dirname);
                int result = rm(dirname);
                printf("rm returned: %d\n", result);
                break;
            }

            case 0:
                printf("Exiting the Mini File System.\n");
                free_file_system();
                exit(0);
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}