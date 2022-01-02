CC=gcc
CFLAGS=-pedantic -O3
INCLUDE_PATHS=
LIBRARY_PATHS=
SDL2=`sdl2-config --libs`

.PHONY=application
application:
	make lib
	make objs
	${CC} ${CFLAGS} src/test.o -o main ${SDL2} -L`pwd` ${LIBRARY_PATHS} -lsprender -lFNA3D -lm -Wl,-rpath=./

.PHONY=application-static
application-static:
	make lib-static
	make objs
	${CC} ${CFLAGS} src/test.o libsprender.a -o main ${SDL2}

.PHONY=lib
lib:
	make objs-lib
	${CC} ${CFLAGS} src/Sprender/Camera.o src/Sprender/Matrix.o src/Sprender/RenderMode.o src/Sprender/Sprender.o src/Sprender/SpriteBatch.o src/Sprender/Shader.o src/Sprender/Texture.o -shared -o libsprender.so ${SDL2}

.PHONY=lib-static
lib-static:
	make objs-lib
	ar rcs libsprender.a src/Sprender/Camera.o
	ar rcs libsprender.a src/Sprender/Matrix.o
	ar rcs libsprender.a src/Sprender/RenderMode.o
	ar rcs libsprender.a src/Sprender/Sprender.o
	ar rcs libsprender.a src/Sprender/SpriteBatch.o
	ar rcs libsprender.a src/Sprender/Shader.o
	ar rcs libsprender.a src/Sprender/Texture.o

.PHONY=objs
objs:
	${CC} ${CFLAGS} -c src/test.c -o src/test.o ${INCLUDE_PATHS}

.PHONY=objs-lib
objs-lib:
	${CC} ${CFLAGS} -c src/Sprender/Camera.c -o src/Sprender/Camera.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/Matrix.c -o src/Sprender/Matrix.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/RenderMode.c -o src/Sprender/RenderMode.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/Sprender.c -o src/Sprender/Sprender.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/SpriteBatch.c -o src/Sprender/SpriteBatch.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/Shader.c -o src/Sprender/Shader.o ${INCLUDE_PATHS} -fPIC
	${CC} ${CFLAGS} -c src/Sprender/Texture.c -o src/Sprender/Texture.o ${INCLUDE_PATHS} -fPIC

.PHONY=clean
clean:
	rm -f src/*.o src/**/*.o ./main ./*.so ./*.a

.PHONY=valgrind
valgrind:
	valgrind ./main --leak-check=full 1> valgrind.txt

.PHONY=apitrace
apitrace:
	apitrace trace ./main
	qapitrace main.trace
	rm main.trace
