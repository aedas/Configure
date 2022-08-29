#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Configure.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_error_msg();
        return 1;
    }
    if (strncmp(argv[1], DLL_MODE, CMP_LEN) && strncmp(argv[1], EXE_MODE, CMP_LEN) && strncmp(argv[1], INIT_MODE, CMP_LEN)) {
        print_error_msg();
        return 1;
    }
    if (strncmp(argv[1], INIT_MODE, CMP_LEN) == 0) {
        return init_config_files() || init_config_instructions();
    }
    if (check_config_files()) {
        print_need_init_msg();
        return 1;
    }
    return write_src_file() || write_makefile(argv[1]);
}

int print_error_msg() {
    printf("Usage: ./configure [-mode]\n");
    printf("Available modes:\n");
    printf("-init for initializing or resetting configuration files\n");
    printf("-dll for shared libraries\n");
    printf("-exe for executables\n");
    return 0;
}

int print_need_init_msg() {
    printf("ERROR: Configuration folder or files not found\n");
    printf("Run './configure -init' to generate/reset folder and related files\n");
    return 0;
}

int check_config_files() {
    FILE *c_src = fopen(C_SRC, "r");
    if (!c_src) {
        return 1;
    }
    FILE *cpp_src = fopen(CPP_SRC, "r");
    if (!cpp_src) {
        fclose(c_src);
        return 1;
    }
    FILE *libs = fopen(LIBS, "r");
    if (!libs) {
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    FILE *include = fopen(INCLUDE, "r");
    if (!include) {
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    FILE *target = fopen(TARGET, "r");
    if (!include) {
        fclose(include);
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    fclose(target);
    fclose(include);
    fclose(libs);
    fclose(cpp_src);
    fclose(c_src);
    return 0;

}

int init_config_files() {
    struct stat st = {0};
    if (stat(CONFIG_PATH, &st) == -1) {
        mkdir(CONFIG_PATH, 0700);
    }
    
    FILE *c_src = fopen(C_SRC, "w");
    if (!c_src) {
        return 1;
    }
    FILE *cpp_src = fopen(CPP_SRC, "w");
    if (!cpp_src) {
        fclose(c_src);
        return 1;
    }
    FILE *libs = fopen(LIBS, "w");
    if (!libs) {
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    FILE *include = fopen(INCLUDE, "w");
    if (!include) {
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    FILE *target = fopen(TARGET, "w");
    if (!include) {
        fclose(include);
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        return 1;
    }
    fclose(target);
    fclose(include);
    fclose(libs);
    fclose(cpp_src);
    fclose(c_src);
    return 0;
}


int init_config_instructions() {
    FILE *file = fopen(CONFIG_INST, "w");
    if (!file) {
        return 1;
    }
    fprintf(file, INSTRUCTIONS);
    fclose(file);
    printf("Initialization done\n");
    printf("See ");
    printf(CONFIG_INST);
    printf(" for next steps\n");
    return 0;
}

int write_src_file() {
    FILE *c_file = fopen(C_SRC, "w");
    if (!c_file) {
        return 1;
    }
    FILE *cpp_file = fopen(CPP_SRC, "w");
    if (!cpp_file) {
        fclose(c_file);
        return 1;
    }
    if (find_src_files(c_file, cpp_file, "./")) {
        fclose(cpp_file);
        fclose(c_file);
        return 1;
    }
    fclose(cpp_file);
    fclose(c_file);

    return 0;
}

int find_src_files(FILE *c_src, FILE *cpp_src, char *path) {
    DIR *dp;
    struct dirent *ep;
    dp = opendir(path);
    if (dp) {
        char *full_path = malloc(MAX_PATH_SIZE * sizeof(char));
        while (ep = readdir(dp)) {
            if (!full_path) {
                free(full_path);
                closedir(dp);
                return 1;
            }
            if (strlen(path) + strlen(ep->d_name) >= MAX_PATH_SIZE) {              
                free(full_path);
                closedir(dp);
                return 1;
            }
            strncpy(full_path, path, MAX_PATH_SIZE);
            int n = strlen(path);
            full_path[n] = '\0';
            strncat(full_path, ep->d_name, MAX_PATH_SIZE - n);
            int m = strlen(full_path);
            if (!str_ends_with(full_path, ".")) {
                if (is_dir(full_path)) {
                    strncat(full_path, "/", MAX_PATH_SIZE - m);
                    if (find_src_files(c_src, cpp_src, full_path)) {
                        free(full_path);
                        closedir(dp);
                        return 1;
                    }
                } else if (str_ends_with(full_path, C_SUFFIX)) {
                    fprintf(c_src, full_path);
                    fprintf(c_src, "\n");
                } else if (str_ends_with(full_path, CPP_SUFFIX)) {
                    fprintf(cpp_src, full_path);
                    fprintf(cpp_src, "\n");
                }
            }
            full_path[0] = '\0';
        }
        free(full_path);
        closedir(dp);
        return 0;
    }
    return 1;
}


int write_makefile(char * mode) {
    FILE *makefile = fopen(MAKEFILE, "w");
    if (!makefile) {
        return 1;
    }
    FILE *c_src = fopen(C_SRC, "r");
    if (!c_src) {
        fclose(makefile);
        return 1;
    }
    FILE *cpp_src = fopen(CPP_SRC, "r");
    if (!cpp_src) {
        fclose(c_src);
        fclose(makefile);
        return 1;
    }
    FILE *libs = fopen(LIBS, "r");
    if (!libs) {
        fclose(cpp_src);
        fclose(c_src);
        fclose(makefile);
        return 1;
    }
    FILE *include = fopen(INCLUDE, "r");
    if (!include) {
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        fclose(makefile);
        return 1;
    }
    FILE *target = fopen(TARGET, "r");
    if (!include) {
        fclose(include);
        fclose(libs);
        fclose(cpp_src);
        fclose(c_src);
        fclose(makefile);
        return 1;
    }
    fprintf(makefile, FLAG_FULL);
    fprintf(makefile, " ");
    if (strncmp(mode, DLL_MODE, CMP_LEN)) {
        fprintf(makefile, FPIC);
    } else {
        fprintf(makefile, SHARED);
    }
    fprintf(makefile, "\n");
    write_src_cmd(makefile, c_src, cpp_src);
    write_lib_cmd(makefile, libs);
    write_include_cmd(makefile, include);
    write_target_cmd(makefile, target, mode);
    fprintf(makefile, MAKEFILE_OBJS);
    fprintf(makefile, "\n");
    fprintf(makefile, MAKEFILE_ALL);
    fclose(target);
    fclose(include);
    fclose(libs);
    fclose(cpp_src);
    fclose(c_src);
    fclose(makefile);
    return 0;
}

int write_src_cmd(FILE *makefile, FILE *c_src, FILE *cpp_src) {
    char c[2];
    char *word = malloc(MAX_PATH_SIZE * (sizeof(char) + 2));
    if (!word) {
        return 1;
    }
    c[0] = '\0';
    c[1] = '\0';
    word[0] = '\0';
    fprintf(makefile, C_SRCS_FULL);
    while (fread(&c, sizeof(char), 1, c_src)) {
        if (c[0] != '\n' && c[0] != 13) {
            strncat(word, c, 1);
        } else {
            if (c[0] == 13) {
                fread(&c, sizeof(char), 1, c_src);
            }
            fprintf(makefile, "\"");
            fprintf(makefile, word);
            fprintf(makefile, "\"");
            fprintf(makefile, " ");
            word[0] = '\0';
        }
        c[0] = '\0';
    }
    if (word[0] && word[0]!= '\n' && word[0] != 13) {
        fprintf(makefile, "\"");
        fprintf(makefile, word);
        fprintf(makefile, "\"");
        word[0] = '\0';
    }
    fprintf(makefile, "\n");
    c[0] = '\0';
    c[1] = '\0';
    word[0] = '\0';
    fprintf(makefile, CPP_SRCS_FULL);
    while (fread(&c, sizeof(char), 1, cpp_src)) {
        if (c[0] != '\n' && c[0] != 13) {
            strncat(word, c, 1);
        } else {
            if (c[0] == 13) {
                fread(&c, sizeof(char), 1, cpp_src);
            }
            fprintf(makefile, "\"");
            fprintf(makefile, word);
            fprintf(makefile, "\"");
            fprintf(makefile, " ");
            word[0] = '\0';
        }
        c[0] = '\0';
    }
    if (word[0] && word[0]!= '\n' && word[0] != 13) {
        fprintf(makefile, "\"");
        fprintf(makefile, word);
        fprintf(makefile, "\"");
        word[0] = '\0';
    }
    free(word);
    fprintf(makefile, "\n");
    return 0;
}

int write_lib_cmd(FILE *makefile, FILE *lib_src) {
    char c[2];
    int is_path = 1;
    char *word = malloc(MAX_PATH_SIZE * (sizeof(char) + 2));
    if (!word) {
        return 1;
    }
    c[0] = '\0';
    c[1] = '\0';
    word[0] = '\0';
    fprintf(makefile, LIB_SRCS_FULL);
    while (fread(&c, sizeof(char), 1, lib_src)) {
        if (is_path) {
            if (c[0] != '\n' && c[0] != 13) {
                strncat(word, c, 1);
            } else {
                if (word[0]) {
                    if (c[0] == 13) {
                        fread(&c, sizeof(char), 1, lib_src);
                    }
                    fprintf(makefile, "\"-L");
                    fprintf(makefile, word);
                    fprintf(makefile, "\"");
                    fprintf(makefile, " ");
                } else {
                    is_path = 0;
                    fprintf(makefile, "\n");
                    fprintf(makefile, LIB_PATH_FULL);
                }
                word[0] = '\0';
            }
        } else {
            if (c[0] != '\n' && c[0] != 13) {
                strncat(word, c, 1);
            } else {
                if (word[0]) {
                    if (c[0] == 13) {
                        fread(&c, sizeof(char), 1, lib_src);
                    }
                    fprintf(makefile, "\"-l");
                    fprintf(makefile, word);
                    fprintf(makefile, "\"");
                    fprintf(makefile, " ");
                } else {
                    is_path = 0;
                }
                word[0] = '\0';
            }
        }
        c[0] = '\0';
    }
    if (word[0] && word[0]!= '\n' && word[0] != 13) {
        fprintf(makefile, "\"-l");
        fprintf(makefile, word);
        fprintf(makefile, "\"");
        word[0] = '\0';
    }
    free(word);
    fprintf(makefile, "\n");
    return 0;
}

int write_include_cmd(FILE *makefile, FILE *include_src) {
    char c[2];
    char *word = malloc(MAX_PATH_SIZE * (sizeof(char) + 2));
    if (!word) {
        return 1;
    }
    c[0] = '\0';
    c[1] = '\0';
    word[0] = '\0';
    fprintf(makefile, INCLUDE_FULL);
    while (fread(&c, sizeof(char), 1, include_src)) {
        if (c[0] != '\n' && c[0] != 13) {
            strncat(word, c, 1);
        } else {
            if (c[0] == 13) {
                fread(&c, sizeof(char), 1, include_src);
            }
            fprintf(makefile, "\"-I");
            fprintf(makefile, word);
            fprintf(makefile, "\"");
            fprintf(makefile, " ");
            word[0] = '\0';
        }
        c[0] = '\0';
    }
    if (word[0] && word[0]!= '\n' && word[0] != 13) {
        fprintf(makefile, "\"-I");
        fprintf(makefile, word);
        fprintf(makefile, "\"");
        word[0] = '\0';
    }
    free(word);
    fprintf(makefile, "\n");   
    return 0;
}

int write_target_cmd(FILE *makefile, FILE *target_src, char *mode) {
    char c[2];
    char *word = malloc(MAX_PATH_SIZE * (sizeof(char) + 2));
    if (!word) {
        return 1;
    }
    c[0] = '\0';
    c[1] = '\0';
    word[0] = '\0';
    int not_empty = 0;
    fprintf(makefile, TARGET_FULL);
    while (fread(&c, sizeof(char), 1, target_src)) {
        not_empty = 1;
        if (c[0] != '\n' && c[0] != 13) {
            strncat(word, c, 1);
        } else {
            if (c[0] == 13) {
                fread(&c, sizeof(char), 1, target_src);
            }
            fprintf(makefile, "\"");
            fprintf(makefile, word);
            fprintf(makefile, "\"");
            word[0] = '\0';
            break;
        }
        c[0] = '\0';
    }
    if (!not_empty) {
        if (strncmp(mode, DLL_MODE, CMP_LEN)) {
            fprintf(makefile, "\"");
            fprintf(makefile, EXE_DEFAULT);
            fprintf(makefile, "\"");
        } else {
            fprintf(makefile, "\"");
            fprintf(makefile, DLL_DEFAULT);
            fprintf(makefile, "\"");
        }
    }
    if (not_empty && word[0] && word[0]!= '\n' && word[0] != 13) {
        fprintf(makefile, "\"");
        fprintf(makefile, word);
        fprintf(makefile, "\"");
        word[0] = '\0';
    }
    free(word);
    fprintf(makefile, "\n");   
    return 0;

}


int is_dir(char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return !S_ISREG(path_stat.st_mode);
}

int str_ends_with(char *src, char *suffix) {
    if (!src || !suffix) {
        return -1;
    }
    int src_len = strlen(src);
    int suffix_len = strlen(suffix);
    if (suffix_len > src_len) {
        return 0;
    }
    return strncmp(src + src_len - suffix_len, suffix, suffix_len) == 0;
}


