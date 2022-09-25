
#define MAX_PATH_SIZE 256

#define C_SUFFIX ".c"
#define CPP_SUFFIX ".cpp"

#define C_SRC ".config/c_src"
#define CPP_SRC ".config/cpp_src"
#define INCLUDE ".config/include"
#define TARGET ".config/target"
#define LIBS ".config/libs"
#define MAKEFILE "makefile"

#define CONFIG_INST "CONFIG_INSTRUCTIONS"
#define INSTRUCTIONS "===== INSTRUCTIONS TO BUILD PROJECT =====\n\nOVERVIEW:\n\tSTEP I: CONFIGURE PATHS\n\tSTEP II: BUILD MAKEFILE\n\tSTEP III: BUILD PROJECT\n\nSTEP I: CONFIGURE PATHS\n\nThere are multiple files under ./config. Required format will be listed at the end of STEP.\n\n1. cpp_src, c_src:\n\tNo need to add values. './configure' will modify them accordingly.\n\n2. include:\n\tAdd library include paths.\n\n3.libs:\n\tFirst include all library search paths.\n\tLeave an empty line, and include library names in the same order as library search paths.\n\tFor libraries, leave out \"lib\" prefix and \"dll\" suffix (i.e. write \"a\" for \"liba.dll\")\n\n4.target:\n\tDefine the name of built product.\n\tIf left empty, a.out or liba.so or liba.dll will be generated after building.\nLeave out suffixes for -slib calls.\n\nFormatting:\n\tLeave out flags (-I/l/L)\n\tLeave out quotation marks (\"\")\n\tUse foreslash (/), do NOT use backslash (\\)\n\nSTEP II: BUILD MAKEFILE\n\nCall \"./configure [-mode]\" to build makefile.\nAvailable modes are:\n\t-exe: To build an executable (defaulted to a.out if target is empty)\n\t-slib: To build a shared library (defaulted to liba.so if target is empty)\n\t-init: To initialize or to reset all configuration files (but NOT makefile). Please manually backup if needed.\n\t-info: To update instructions.\n\nSTEP III: BUILD PROJECT\n\nCall \"make\" and obtain your target.\n\nManual Troubleshooting:\n\tUnexpected errors may occur due to faulty makefile or wrong paths.\n\tManually fine tune makefile or configuration files and call \"make\" again.\n\nADDITIONAL COMMENTS\n\nIt is NOT recommended to put .c and .cpp files of the same name in the same location.\nThis may disrupt makefile process and lead to unexpected results."

#define CONFIG_PATH "./.config"

#define DLL_MODE "-slib"
#define EXE_MODE "-exe"
#define INIT_MODE "-init"
#define INST_MODE "-info"

#define LINUX_OS "-linux"
#define WINDOWS_32_OS "-w32"
#define WINDOWS_64_OS "-w64"

#define LINUX_CMD "g"
#define WINDOWS_32_CMD "i686-w64-mingw32-g"
#define WINDOWS_64_CMD "x86_64-w64-mingw32-g"

#define ASAN "-fsanitize=address"


#define DLL_DEFAULT "liba"
#define EXE_DEFAULT "a.out"

#define FLAG_FULL "FLAGS=-g3 -Wall -Wextra -fmessage-length=0"
#define ASAN_FULL "ASAN="
#define OS_FULL "OS="
#define C_SRCS_FULL "C_SRCS="
#define MODE_FULL "MODE="
#define CPP_SRCS_FULL "CPP_SRCS="
#define LIB_SRCS_FULL "LIBS="
#define LIB_PATH_FULL "LIBSPATH="
#define INCLUDE_FULL "INCLUDE="
#define TARGET_FULL "TARGET="
#define MAKEFILE_OBJS "C_OBJS=${C_SRCS:.c\"=.o\"}\nCPP_OBJS=${CPP_SRCS:.cpp\"=.o\"}"
#define MAKEFILE_L_CALLS "L_C_OBJS=${C_SRCS:.c\"=.l\"}\nL_CPP_OBJS=${CPP_SRCS:.cpp\"=.l\"}"
#define MAKEFILE_W32_CALLS "W32_C_OBJS=${C_SRCS:.c\"=.w32\"}\nW32_CPP_OBJS=${CPP_SRCS:.cpp\"=.w32\"}"
#define MAKEFILE_W64_CALLS "W64_C_OBJS=${C_SRCS:.c\"=.w64\"}\nW64_CPP_OBJS=${CPP_SRCS:.cpp\"=.w64\"}"
#define MAKEFILE_EXE "all:${C_OBJS} ${CPP_OBJS}\n\t${OS}++ ${INCLUDE} ${LIBSPATH} ${FLAGS} ${ASAN} ${MODE} -o ${TARGET} ${C_OBJS} ${CPP_OBJS} ${LIBS}\n\trm ${C_OBJS} ${CPP_OBJS}\n${C_OBJS}:%%.o\":\n\t${OS}cc -c ${INCLUDE} ${FLAGS} ${ASAN} -fPIC -o $@ $*.c\"\n${CPP_OBJS}:%%.o\":\n\t${OS}++ -c ${INCLUDE} ${FLAGS} ${ASAN} -fPIC -o $@ $*.cpp\""
#define MAKEFILE_SLIB "all:\n\techo \"Usage: make [-os]\\nAvailable targets: linux, w32, w64\\n\"\nlinux:${L_C_OBJS} ${L_CPP_OBJS}\n\tg++ ${INCLUDE} ${LIBSPATH} ${FLAGS} ${ASAN} ${MODE} -o ${TARGET}.so ${C_OBJS} ${CPP_OBJS} ${LIBS}\n\trm ${C_OBJS} ${CPP_OBJS}\nw32:${W32_C_OBJS} ${W32_CPP_OBJS}\n\ti686-w64-mingw32-g++ ${INCLUDE} ${LIBSPATH} ${FLAGS} ${MODE} -o ${TARGET}.dll ${C_OBJS} ${CPP_OBJS} ${LIBS}\n\trm ${C_OBJS} ${CPP_OBJS}\nw64:${W64_C_OBJS} ${W64_CPP_OBJS}\n\tx86_64-w64-mingw32-g++ ${INCLUDE} ${LIBSPATH} ${FLAGS} ${MODE} -o ${TARGET}.dll ${C_OBJS} ${CPP_OBJS} ${LIBS}\n\trm ${C_OBJS} ${CPP_OBJS}\n${L_C_OBJS}:%%.l\":\n\tgcc -c ${INCLUDE} ${FLAGS} ${ASAN} -fPIC -o $*.o\" $*.c\n${L_CPP_OBJS}:%%.l\":\n\tg++ -c ${INCLUDE} ${FLAGS} ${ASAN} -fPIC -o $*.o\" $*.cpp\"\n${W32_C_OBJS}:%%.w32\":\n\ti686-w64-mingw32-gcc -c ${INCLUDE} ${FLAGS} -fPIC -o $*.o\" $*.c\n${W32_CPP_OBJS}:%%.w32\":\n\ti686-w64-mingw32-g++ -c ${INCLUDE} ${FLAGS} -fPIC -o $*.o\" $*.cpp\"\n${W64_C_OBJS}:%%.w64\":\n\tx86_64-w64-mingw32-gcc -c ${INCLUDE} ${FLAGS} -fPIC -o $*.o\" $*.c\n${W64_CPP_OBJS}:%%.w64\":\n\tx86_64-w64-mingw32-g++ -c ${INCLUDE} ${FLAGS} -fPIC -o $*.o\" $*.cpp\""



#define CMP_LEN 7

#define FPIC "-fPIC"
#define SHARED "-shared"

#ifndef CONFIGURE_H
#define CONFIGURE_H

int print_error_msg();
int print_need_init_msg();
int check_config_files();
int init_config_files();
int init_config_instructions();
int write_src_file();
int find_src_files(FILE *, FILE *, char *);
int write_makefile(char *, char *);
int write_src_cmd(FILE *, FILE *, FILE *);
int write_lib_cmd(FILE *, FILE *);
int write_include_cmd(FILE *, FILE *);
int write_target_cmd(FILE *, FILE *, char *);
int is_dir(char *);
int str_ends_with(char *, char *);

#endif /* CONFIGURE_H */