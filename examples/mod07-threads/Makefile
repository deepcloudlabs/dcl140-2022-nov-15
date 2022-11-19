target : serial parallel

serial: serial_sum.c
	gcc -o serial_sum serial_sum.c

parallel: parallel_sum.c 
	gcc -o parallel_sum parallel_sum.c -lpthread -pthread

clean :
	rm -f *.o
