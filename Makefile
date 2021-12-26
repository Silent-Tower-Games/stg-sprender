CC=gcc
CFLAGS=-pedantic -O3
INCLUDE_PATHS=
LIBRARY_PATHS=
LIBS=
SDL2=`sdl2-config --libs`

application:
	make lib
	make objs
	${CC} ${CFLAGS} src/test.o -o main ${SDL2} -L`pwd` ${LIBRARY_PATHS} -lsprender ${LIBS} -Wl,-rpath=./

application-static:
	make lib-static
	make objs
	${CC} ${CFLAGS} src/test.o libsprender.a -o main ${SDL2}

lib:
	make objs-lib
	${CC} ${CFLAGS} src/Sprender/Sprender.o -shared -o libsprender.so ${SDL2}

lib-static:
	make objs-lib
	ar rcs libsprender.a src/Sprender/Sprender.o

objs:
	${CC} ${CFLAGS} -c src/test.c -o src/test.o ${INCLUDE_PATHS}

objs-lib:
	${CC} ${CFLAGS} -c src/Sprender/Sprender.c -o src/Sprender/Sprender.o ${INCLUDE_PATHS} -fPIC

clear:
	rm -f src/*.o src/**/*.o ./main ./*.so ./*.a

valgrind:
	valgrind ./main -v --leak-check=full 2> valgrind.txt
