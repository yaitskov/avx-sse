CC=gcc -mavx2 -o3

add: add.c
	$(CC) -o add add.c

prefix-to-comma: prefix-to-comma.c
	$(CC) -o $@ $<
count-chars: count-chars.c
	$(CC) -o $@ $<


count-chars-in-file: count-chars-in-file.c
	$(CC) -o $@ $<

count-chars-in-file-seq: count-chars-in-file-seq.c
	$(CC) -o $@ $<


count-char-in-file-with-avx: count-char-in-file-with-avx.c
	$(CC) -o $@ $<

count-char-in-file-with-avx-popcnt: count-char-in-file-with-avx-popcnt.c
	$(CC) -o $@ $<


parseint.exe: parseint.c
	$(CC) -o $@ $<
