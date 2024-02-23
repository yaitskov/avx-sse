CC=gcc -mavx2 -o3

add: add.c
	$(CC) -o add add.c

gather.exe: gather.c
	$(CC) -o $@ $<

prefix-to-comma: prefix-to-comma.c
	$(CC) -o $@ $<
count-chars: count-chars.c
	$(CC) -o $@ $<


count-chars-in-file: count-chars-in-file.c
	$(CC) -o $@ $<

count-chars-in-file-seq: count-chars-in-file-seq.c
	$(CC) -o $@ $<


count-char-in-file-with-avx.exe: count-char-in-file-with-avx.c
	$(CC) -o $@ $<

count-char-in-file-with-avx-popcnt.exe: count-char-in-file-with-avx-popcnt.c
	gcc -march=native  -o3 -o $@ $<
#	gcc -march=sapphirerapids  -o3 -o $@ $<


parseint.exe: parseint.c
	$(CC) -o $@ $<

clean:
	rm *.exe
