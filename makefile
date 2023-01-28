app: Application.o
	g++ src/Application.o -o app -lglfw -lGL

Application.o: src/Application.cpp
	g++ -c src/Application.cpp -o src/Application.o

clean:
	rm **/*.o app
