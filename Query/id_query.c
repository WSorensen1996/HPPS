#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "id_query.h"
#include "timing.h"



// This file (along with its implementation id_query.c) abstracts out
// the user-facing part of the query programs.  It implements the
// following algorithm:
//
// Records <- Read Dataset
// Index <- Produce Index From Records
// While Program Is Running:
//   Read Query From User
//   Lookup Query In Index
// Free Index
//
// Where the specifics of "Produce Index From Records", "Lookup Query
// In Index", and "Free Index" are provided via function pointers.
// This means we can write the main loop just once, and reuse it with
// different implementations of indexes.
//
// See the file id_query_naive.c for a usage example.

// A pointer to a function that produces an index, when called with an
// array of records and the size of the array.
// --- typedef void* (*mk_index_fn)(const struct record*, int);

// Freeing an array produced by a mk_index_fn.
// ---typedef void (*free_index_fn)(void*);

// Look up an ID in an index produced by mk_index_fn.
// ---typedef const struct record* (*lookup_fn)(void*, int64_t);

// Run a query loop, using the provided functions for managing the
// index.
// ---int id_query_loop(int argc, char** argv, mk_index_fn, free_index_fn, lookup_fn);




int id_query_loop(int argc, char** argv, mk_index_fn mk_index, free_index_fn free_index, lookup_fn lookup) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  uint64_t start, runtime;
  int n;

  start = microseconds();
  struct record *rs = read_records(argv[1], &n);
  runtime = microseconds()-start;

  if (rs) {
    printf("Reading records: %dms\n", (int)runtime/1000);

    start = microseconds();
    void *index = mk_index(rs, n);
    runtime = microseconds()-start;
    printf("Building index: %dms\n", (int)runtime/1000);

    char *line = NULL;
    size_t line_len;

    uint64_t runtime_sum = 0;
    while (getline(&line, &line_len, stdin) != -1) {
      int64_t needle = atol(line);

      start = microseconds();
      const struct record *r = lookup(index, needle);
      runtime = microseconds()-start;

      if (r) {
        
        printf("%ld: %s %f %f\n", (long)needle, r->name, r->lon, r->lat);
      } else {
        printf("%ld: not found\n", (long)needle);
      }

      printf("Query time: %dus\n", (int)runtime);
      runtime_sum += runtime;
    }

    printf("Total query runtime: %dus\n", (int)runtime_sum);

    free(line);
    free_index(index);
    free_records(rs, n);
    return 0;
  } else {
    fprintf(stderr, "Failed to read input from %s (errno: %s)\n",
            argv[1], strerror(errno));
    return 1;
  }
}
