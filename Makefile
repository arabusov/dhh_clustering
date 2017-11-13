all:
	gcc spiral.c -o spiral
	./spiral test.pat
	gcc -c soft_impl.c -o soft_impl.o
	gcc main.c soft_impl.o -o soft_impl
clean:
	rm -rf soft_impl.o soft_impl spiral test.pat
