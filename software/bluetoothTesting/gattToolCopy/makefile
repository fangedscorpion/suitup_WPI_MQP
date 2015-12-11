CC = gcc
GLIB = `pkg-config --cflags glib-2.0`
all: gattToolCopy

gattToolCopy: gattToolCopy.o util.o crypto.o gatt.o gattrib.o 
	$(CC) -o gattToolCopy gattToolCopy.o util.o crypto.o gattrib.o gatt.o

gattToolCopy.o: gattToolCopy.c
	$(CC) -c gattToolCopy.c $(GLIB) -lbluetooth  

util.o: util.c util.h
	$(CC) -c util.c -lbluetooth

log.o: log.c log.h util.h
	$(CC) -c log.c  $(GLIB) -lbluetooth

crypto.o: crypto.c crypto.h util.h
	$(CC) -c crypto.c

btio.o: btio.c btio.h
	$(CC) -c btio.c -lbluetooth $(GLIB)	

att.o: att.c att.h util.h
	$(CC) -c att.c -lbluetooth $(GLIB)

gattrib.o: gattrib.c gattrib.h btio.h log.h util.h att.h queue.h
	$(CC) -c gattrib.c -lbluetooth $(GLIB)

gatt.o: gatt.c gatt.h util.h att.h gattrib.h
	$(CC) -c gatt.c $(GLIB) -lbluetooth

clean:
	rm *.o gattToolCopy