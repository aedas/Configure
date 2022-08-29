FLAGS=-g3 -Wall -Wextra -fsanitize=address -fmessage-length=0 -fPIC
C_SRCS="./.config/hi.c" "./Configure.c" 
CPP_SRCS="./.config/hello.cpp" 
LIBS="-LC:/Projects/Libraries/C++/libpng16/lib" "-LC:/Projects/Libraries/C++/zlib12/lib" 
LIBSPATH="-lpng16" "-lz"
INCLUDE="-IC:/Projects/Libraries/C++/libpng16/include/libpng16" "-IC:/Projects/Libraries/C++/zlib12/include"
TARGET="configure"
C_OBJS=${C_SRCS:.c"=.o"}
CPP_OBJS=${CPP_SRCS:.cpp"=.o"}
all:${C_OBJS} ${CPP_OBJS}
	g++ ${INCLUDE} ${LIBPATHS} ${FLAGS} -o ${TARGET} ${C_OBJS} ${CPP_OBJS} ${LIBS}
	rm ${C_OBJS} ${CPP_OBJS}
${C_OBJS}:%.o":
	gcc -c ${FLAGS} -o $@ $*.c"
${CPP_OBJS}:%.o":
	g++ -c ${FLAGS} -o $@ $*.cpp"