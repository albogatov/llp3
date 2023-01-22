all: main

main: main.o schema.o relation.o file_ops.o database.o type_ops.o
	gcc main.o relation.o schema.o file_ops.o type_ops.o database.o -o main  -std=c99

main.o:
	gcc -c src/main.c  -std=c99

relation.o:
	gcc -c src/relation.c  -std=c99

schema.o:
	gcc -c src/schema.c  -std=c99

file_ops.o: type_ops.o
	gcc -c src/file_ops.c  -std=c99

database.o:
	gcc -c src/database.c  -std=c99

type_ops.o:
	gcc -c src/type_ops.c  -std=c99

exec:
	./main

clean:
	del -rf *.o; \
	del main.exe
	