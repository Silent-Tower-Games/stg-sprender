CC=gcc
CFLAGS=-pedantic -O3

application:
	make lib
	make objs
	${CC} ${CFLAGS} src/test.o -o main `sdl2-config --libs` -L`pwd` -lsprender -Wl,-rpath=./

application-static:
	make lib-static
	make objs
	${CC} ${CFLAGS} src/test.o libsprender.a -o main `sdl2-config --libs`

lib:
	make objs-lib
	${CC} ${CFLAGS} src/Sprender/Sprender.o -shared -o libsprender.so `sdl2-config --libs`

lib-static:
	make objs-lib
	ar rcs libsprender.a src/Sprender/Sprender.o

objs:
	${CC} ${CFLAGS} -c src/test.c -o src/test.o

objs-lib:
	${CC} ${CFLAGS} -c src/Sprender/Sprender.c -o src/Sprender/Sprender.o -fPIC

clear:
	rm -f src/*.o src/**/*.o ./main ./*.so ./*.a

valgrind:
	valgrind ./main -v --leak-check=full 2> valgrind.txt
