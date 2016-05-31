pgm: main.o imgproc.o crypto.o stegano.o utilities.o
	gcc main.o imgproc.o crypto.o stegano.o utilities.o -lm -o pgm 

main.o: main.c imgproc.h crypto.h stegano.h utilities.h
	gcc -c -std=c99 main.c -lm

imgproc.o: imgproc.c imgproc.h consts.h
	gcc -c -std=c99 imgproc.c -lm

crypto.o: crypto.c crypto.h consts.h
	gcc -c -std=c99 crypto.c -lm

stegano.o: stegano.c stegano.h consts.h
	gcc -c -std=c99 stegano.c -lm

utilities.o: utilities.c utilities.h
	gcc -c -std=c99 utilities.c -lm

clean:
	rm *.o

