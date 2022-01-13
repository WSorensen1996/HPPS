OPENMP?= -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  # had to run 'brew install libomp' first 
DEBUG?=
CC?=gcc
CFLAGS?=-Wextra -Wall -pedantic -std=c99 -Ofast
LDFLAGS?=-lm
EXECUTABLES=threading_tester

all: $(EXECUTABLES)


%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(DEBUG) $(OPENMP)



