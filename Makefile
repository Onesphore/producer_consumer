CC= gcc
CFLAGS= -g3 -O0

FILE=producer_consumer

default: ${FILE}

${FILE}: ${FILE}.c semaphore.c
	${CC} ${CFLAGS} ${FILE}.c -o $@ -lpthread

run: ${FILE} 
	./${FILE}

gdb: ${FILE}
	gdb ./${FILE} 

clean: 
	rm -rf ${FILE}
