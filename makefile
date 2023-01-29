app: main.o
	g++ src/main.o -o main -lglfw -lGL -lGLEW -lX11 -lGLU

main.o: src/main.cpp
	g++ -c src/main.cpp -o src/main.o

clean:
	rm **/*.o main
