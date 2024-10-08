all:
	gcc torlink.c -o torlink.so -fPIC -shared -ldl -D_GNU_SOURCE