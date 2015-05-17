rasterize_serial.exe: rasterize_serial.c lodepng.o
	gcc rasterize_serial.c -o rasterize_serial.exe lodepng.o

lodepng.o: lodepng.c lodepng.h
	gcc -c lodepng.c -o lodepng.o

