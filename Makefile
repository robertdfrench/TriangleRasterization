rasterize_serial.exe: rasterize_serial.c lodepng.c lodepng.h
	gcc lodepng.c rasterize_serial.c -o rasterize_serial.exe
