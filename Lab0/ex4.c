#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
    int w, h; // width, height
} Plane;

typedef struct {
    Plane base;
    int d; // depth
} Volume;

void print_plane(Plane *p) {
    printf("W %d, H %d\n", p->w, p->h);
}

void print_volume(Volume *v) {
    printf("W %d, H %d, D %d\n", v->base.w, v->base.h, v->d);
}

//challenge 3
Plane* new_plane(int w, int h) {
    Plane *p = malloc(sizeof(Plane));
    p->w = w;
    p->h = h;
    return p;
}


Volume *new_volume(Plane *p, int d) {
    Volume *v = malloc(sizeof(Volume));
    v->base.w = p->w;
    v->base.h = p->h;
    v->d = d;
    return v;
}

int main() {
    Plane p = {5, 2};
    Volume v = {p, 3};
    print_plane(&p);
    print_plane((Plane *) &v);
    print_volume(&v);

    //challenge 3
    Plane* plane = new_plane(10,20);
    printf("Width is %d", plane->w);

    return 0;
}