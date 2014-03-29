#include <stdlib.h>
#include <stdio.h>

typedef struct point {
	float x;
	float y;
} Point;

typedef struct triangle {
	Point a;
	Point b;
	Point c;
} Triangle;

typedef struct line {
	float m;
	float b;
} Line;

typedef enum {O_ABOVE, O_BELOW} Orientation;

typedef struct triangle_info {
	Triangle t;
	Point centroid;
	Line lines[3];
	Orientation orientations[3];
} TriangleInfo;

Point create_point(float x, float y) {
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

Triangle create_triangle(Point a, Point b, Point c) {
	Triangle t;
	t.a = a;
	t.b = b;
	t.c = c;
	return t;
}

Point calculate_centroid(Triangle t) {
	Point centroid;
	centroid.x = (t.a.x + t.b.x + t.c.x) / 3.0;
	centroid.y = (t.a.y + t.b.y + t.c.y) / 3.0;
	return centroid;
}

Line calculate_line(Point a, Point b) {
	Line l;
	l.m = (b.y - a.y) / (b.x - a.x);
	l.b = b.y - l.m * b.x;
	return l;
}

Orientation calculate_line_orientation(Line l, Point centroid) {
	// y >= mx +b --> O_ABOVE
	if (centroid.y >= l.m * centroid.x + l.b) return O_ABOVE;
	return O_BELOW;
}

TriangleInfo calculate_triangle_info(Triangle t) {
	TriangleInfo ti;
	ti.t = t;
	ti.centroid = calculate_centroid(t);
	ti.lines[0] = calculate_line(t.a, t.b);
	ti.lines[1] = calculate_line(t.b, t.c);
	ti.lines[2] = calculate_line(t.c, t.a);
	ti.orientations[0] = calculate_line_orientation(ti.lines[0], ti.centroid);
	ti.orientations[1] = calculate_line_orientation(ti.lines[1], ti.centroid);
	ti.orientations[2] = calculate_line_orientation(ti.lines[2], ti.centroid);
	return ti;
}

typedef struct grid_point {
	int i;
	int j;
} GridPoint;

typedef struct grid {
	int dimX;
	int dimY;
	int* grid_memory;
} Grid;

Grid allocate_grid(int dimX, int dimY) {
	Grid g;
	g.dimX = dimX;
	g.dimY = dimY;
	g.grid_memory = (int*)malloc(sizeof(int) * dimX * dimY);
	return g;
}

#define calculate_cell_index(grid, grid_point) grid_point.j * grid.dimX + grid_point.i;
#define iterate_grid(g,expr) do {\
	GridPoint igp;\
	for(igp.i = 0; igp.i < g.dimX; igp.i++) {\
		for(igp.j = 0; igp.j < g.dimY; igp.j++) {\
			int cell_index = calculate_cell_index(g,igp);\
			expr;\
		}\
	}\
} while(0);
#define iterate_grid_rows(g,cell_expr,before_row_expr,after_row_expr) do {\
	GridPoint igp;\
	for(igp.j = 0; igp.j < g.dimY; igp.j++) {\
		int row_index = igp.j;\
		before_row_expr;\
		for(igp.i = 0; igp.i < g.dimX; igp.i++) {\
			int cell_index = calculate_cell_index(g,igp);\
			cell_expr;\
		}\
		after_row_expr;\
	}\
} while(0);

void initialize_grid(Grid g, int iv) {
	iterate_grid(g, g.grid_memory[cell_index] = iv);
}

GridPoint snap_point_to_grid(Point p, Grid g) {
	GridPoint gp;
	gp.i = (int)(p.x / g.dimX);
	gp.j = (int)(p.y / g.dimY);
	return gp;
}

typedef struct bounding_box {
	int minX;
	int minY;
	int maxX;
	int maxY;
} BoundingBox;

typedef struct bounding_box_info {
	BoundingBox bb;
	int depthX;
	int depthY;
	int numGridPoints;
} BoundingBoxInfo;

BoundingBox calculate_least_bounding_box(Triangle t) {
	BoundingBox bb;

	// minX
	bb.minX = t.a.x;
	if (t.b.x < bb.minX) bb.minX = t.b.x;
	if (t.c.x < bb.minX) bb.minX = t.c.x;

	// minY
	bb.minY = t.a.y;
	if (t.b.y < bb.minY) bb.minY = t.b.y;
	if (t.c.y < bb.minY) bb.minY = t.c.y;

	// maxX
	bb.maxX = t.a.x;
	if (t.b.x > bb.maxX) bb.maxX = t.b.x;
	if (t.c.x > bb.maxX) bb.maxX = t.c.x;

	// maxY
	bb.maxY = t.a.y;
	if (t.b.y > bb.maxY) bb.maxY = t.b.y;
	if (t.c.y > bb.maxY) bb.maxY = t.c.y;

	return bb;
}

BoundingBoxInfo calculate_bounding_box_info(BoundingBox bb) {
	BoundingBoxInfo bbi;
	bbi.bb = bb;
	bbi.depthX = bb.maxX - bb.minX;
	bbi.depthY = bb.maxY - bb.minY;
	bbi.numGridPoints = bbi.depthX * bbi.depthY;
	return bbi;
}

typedef enum {T_INTERIOR, T_EXTERIOR} Membership;

int num_members = 0;
Membership membership_test(GridPoint gp, TriangleInfo ti) {
	Point p;
	p.x = gp.i;
	p.y = gp.j;
	if(
		calculate_line_orientation(ti.lines[0],p) == ti.orientations[0] &&
		calculate_line_orientation(ti.lines[1],p) == ti.orientations[1] &&
		calculate_line_orientation(ti.lines[2],p) == ti.orientations[2] 
	  ) {
		num_members++;
		return T_INTERIOR;
	}
	return T_EXTERIOR;
}

void print_grid_header(Grid g) {
	printf("+");
	int bi;
	for(bi = 0; bi < g.dimX; bi++) printf("-");
	printf("+\n");
}

void print_grid(Grid g) {
	print_grid_header(g);
	iterate_grid_rows(g, 
		printf("%s",(g.grid_memory[cell_index] == 0) ? "*" : " "),
		printf("|"),
		printf("|\n")
	);
	print_grid_header(g);
}

int main(int argc, char **argv) {
	
	Point a = create_point(1.02,2.11);
	Point b = create_point(64.28,28.79);
	Point c = create_point(51.63,38.99);

	Triangle t = create_triangle(a,b,c);

	TriangleInfo ti = calculate_triangle_info(t);

	Grid g = allocate_grid(70,40);
	initialize_grid(g, 0);

	iterate_grid(g, g.grid_memory[cell_index] = membership_test(igp,ti));
	print_grid(g);
	printf("Total number of members: %d\n",num_members);
	return 0;
}
