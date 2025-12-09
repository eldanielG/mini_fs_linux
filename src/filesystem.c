#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>   // getcwd, chdir
#include <errno.h>
#include "../include/filesystem.h"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

static char root_path[PATH_MAX];
static char current_path[PATH_MAX];

static int join_path(char *dst, size_t dstsz, const char *base, const char *name) {
    size_t blen = strlen(base);
    int needs_sep = (blen > 0 && base[blen - 1] != '/');
    if (snprintf(dst, dstsz, needs_sep ? "%s/%s" : "%s%s", base, name) >= (int)dstsz) {
        return -1;
    }
    return 0;
}

static int is_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode) != 0;
}

static int is_valid_component(const char *name) {
    if (!name || !*name) return 0;
    if (strchr(name, '/')) return 0;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) return 0;
    return 1;
}

int init_file_system() {
    if (getcwd(root_path, sizeof(root_path)) == NULL) {
        return -1;
    }
    strncpy(current_path, root_path, sizeof(current_path));
    current_path[sizeof(current_path) - 1] = '\0';
    return 0;
}

int free_file_system() {
    // Nada para liberar no backend do SO
    return 0;
}

int mini_mkdir(const char *name) {
    if (!is_valid_component(name)) return -1;
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;

    if (mkdir(path, 0755) == 0) return 0;
    // Se já existe e é diretório, considere erro (mantendo contrato antigo)
    return -1;
}

int cd(const char *path) {
    if (!path) return -1;
    if (strcmp(path, "/") == 0) {
        strncpy(current_path, root_path, sizeof(current_path));
        current_path[sizeof(current_path) - 1] = '\0';
        return 0;
    }
    if (strcmp(path, "..") == 0) {
        // Não subir acima do root
        if (strcmp(current_path, root_path) == 0) return 0;
        char *last = strrchr(current_path, '/');
        if (!last) return 0;
        *last = '\0';
        if (current_path[0] == '\0') {
            strncpy(current_path, root_path, sizeof(current_path));
            current_path[sizeof(current_path) - 1] = '\0';
        }
        // Garantir que não passou do root
        if (strncmp(current_path, root_path, strlen(root_path)) != 0) {
            strncpy(current_path, root_path, sizeof(current_path));
            current_path[sizeof(current_path) - 1] = '\0';
        }
        return 0;
    }
    if (!is_valid_component(path)) return -1;

    char candidate[PATH_MAX];
    if (join_path(candidate, sizeof(candidate), current_path, path) != 0) return -1;
    if (!is_dir(candidate)) return -1;

    strncpy(current_path, candidate, sizeof(current_path));
    current_path[sizeof(current_path) - 1] = '\0';
    return 0;
}

int touch(const char *name) {
    if (!is_valid_component(name)) return -1;
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;

    FILE *fp = fopen(path, "ab");
    if (!fp) return -1;
    fclose(fp);
    return 0;
}

int echo(const char *name, const char *content) {
    if (!is_valid_component(name)) return -1;
    if (!content) content = "";
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;

    FILE *fp = fopen(path, "wb");
    if (!fp) return -1;
    size_t len = strlen(content);
    if (len) fwrite(content, 1, len, fp);
    fclose(fp);
    return 0;
}

int cat(const char *name) {
    if (!is_valid_component(name)) return -1;
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;

    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;
    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        fwrite(buf, 1, n, stdout);
    }
    fclose(fp);
    return 0;
}

int rm(const char *name) {
    if (!is_valid_component(name)) return -1;
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;
    if (remove(path) == 0) return 0;
    return -1;
}

int mini_chmod(const char *name, int permissions) {
    if (!is_valid_component(name)) return -1;
    if (permissions < 0 || permissions > 0777) return -1;
    char path[PATH_MAX];
    if (join_path(path, sizeof(path), current_path, name) != 0) return -1;
    if (chmod(path, permissions) == 0) return 0;
    return -1;
}

int cp(const char *source, const char *destination) {
    if (!is_valid_component(source) || !is_valid_component(destination)) return -1;
    char src[PATH_MAX], dst[PATH_MAX];
    if (join_path(src, sizeof(src), current_path, source) != 0) return -1;
    if (join_path(dst, sizeof(dst), current_path, destination) != 0) return -1;

    FILE *fp_src = fopen(src, "rb");
    if (!fp_src) return -1;
    FILE *fp_dst = fopen(dst, "wb");
    if (!fp_dst) {
        fclose(fp_src);
        return -1;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fp_src)) > 0) {
        if (fwrite(buf, 1, n, fp_dst) != n) {
            fclose(fp_src);
            fclose(fp_dst);
            return -1;
        }
    }
    fclose(fp_src);
    fclose(fp_dst);
    return 0;
}

int mv(const char *source, const char *destination) {
    if (!is_valid_component(source) || !is_valid_component(destination)) return -1;
    char src[PATH_MAX], dst[PATH_MAX];
    if (join_path(src, sizeof(src), current_path, source) != 0) return -1;
    if (join_path(dst, sizeof(dst), current_path, destination) != 0) return -1;
    if (rename(src, dst) == 0) return 0;
    return -1;
}