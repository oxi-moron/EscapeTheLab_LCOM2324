#include <stdio.h>
#include <math.h>

struct Point2d {
    double x;
    double y;
};

struct Point2d point2DSum(struct Point2d a, struct Point2d b) {
    return (struct Point2d){a.x + b.x, a.y + b.y};
}

double distance(struct Point2d a, struct Point2d b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    struct Point2d point2D = {1, 3};
    point2D = point2DSum(point2D, point2D);
    printf("(%f, %f)\n", point2D.x, point2D.y);
    printf("%f\n", distance(point2D, point2D));
    return 0;
}
