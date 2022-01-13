#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <sys/time.h>

double seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL); // The null is for timezone information.
  return tv.tv_sec + tv.tv_usec/1000000.0;
}

struct particle {
  double x;
  double y;
  double z;
  double mass;
  double vx;
  double vy;
  double vz;
};

int read_particle(FILE *f, struct particle *p) {
  return fscanf(f, "%lf %lf %lf %lf %lf %lf %lf\n",
                &p->mass,
                &p->x, &p->y, &p->z,
                &p->vx, &p->vy, &p->vz) != 7;
}

struct particle* read_particles(const char *fname, int *n) {
  int capacity = 1000000, i = 0;

  struct particle *ps = malloc(capacity * sizeof(struct particle));

  FILE *f = fopen(fname, "r");
  assert(f != NULL);

  while (1) {
    struct particle p;
    if (!read_particle(f, &p)) {
      assert(i < capacity);
      ps[i] = p;
      i++;
    } else {
      *n = i;
      break;
    }
  }

  return ps;
}

void write_particles(const char *fname, int n, struct particle *ps) {
  FILE *f = fopen(fname, "w");
  assert(f != NULL);

  for (int i = 0; i < n; i++) {
    fprintf(f, "%f %f %f %f %f %f %f\n",
            ps[i].mass,
            ps[i].x, ps[i].y, ps[i].z,
            ps[i].vx, ps[i].vy, ps[i].vz);
  }
}

#endif
