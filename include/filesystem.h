#ifndef FILESYSTEM_H
#define FILESYSTEM_H

// API do MiniFS
int mini_mkdir(const char *path);
int cd(const char *path);
int touch(const char *filename);
int echo(const char *filename, const char *content);
int cat(const char *filename);
int cp(const char *source, const char *destination);
int mv(const char *source, const char *destination);
int rm(const char *filename);
int mini_chmod(const char *filename, int permissions);
int init_file_system();
int free_file_system();

#define PERM_READ  0b100
#define PERM_WRITE 0b010
#define PERM_EXEC  0b001

#endif // FILESYSTEM_H