app: main.o mesh.o
	g++ src/main.o src/mesh.o -o main -lglfw -lGL -lGLEW -lX11 -lGLU

main.o: src/main.cpp
	g++ -c src/main.cpp -o src/main.o

mesh.o: src/mesh.cpp
	g++ -c src/mesh.cpp -o src/mesh.o

clean:
	rm **/*.o main
