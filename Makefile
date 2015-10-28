all: pam_cut.so

pam_cut.o: pam_cut.c
	gcc -fPIC -c pam_cut.c

pam_cut.so: pam_cut.o
	gcc -shared -o pam_cut.so pam_cut.o -lpam
