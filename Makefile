test: shell.o sighandlers.o syswrappers.o utils.o flags.o
	rm -f run_tests
	gcc -Wall -o run_tests tests/tests.c shell.o sighandlers.o syswrappers.o utils.o -lcheck -lm -lpthread -lrt -lsubunit
	./run_tests

shell: shell.o sighandlers.o syswrappers.o utils.o flags.o
	rm -f shell
	gcc -g shell.o sighandlers.o syswrappers.o utils.o flags.o -o shell

shell.o: shell.c shell.h flags.c
	gcc -g -c shell.c

sighandlers.o: sighandlers.c sighandlers.h flags.c
	gcc -g -c sighandlers.c

syswrappers.o: syswrappers.c syswrappers.h
	gcc -g -c syswrappers.c

utils.o: utils.c utils.h
	gcc -g -c utils.c

flags.o: flags.c
	gcc -g -c flags.c


