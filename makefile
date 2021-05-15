compile:
	@gcc *.c -o shell.out 

clean_up:
	rm *.out

check_leak:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./test.out 