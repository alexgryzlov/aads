#include <iostream>
#include <cmath>
#include <iomanip>

namespace Geom {
    const double eps = 1e-10;
    struct Point {
        double x, y, z;
        Point(double x, double y, double z) : x(x), y(y), z(z) {}
        Point() : x(0), y(0), z(0) {}
    };

    struct Segment {
        Point start, end;
        Segment(Point start, Point end) : start(start), end(end) {}
        Segment() : start(), end() {}
    };

    double points_distance(const Point& p1, const Point& p2) {
        return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
    }

    double point_segment_distance(const Point& p, const Segment& seg) {
        Point right_point = seg.end;
        Point left_point = seg.start;

        double d_end   = points_distance(p, seg.end);
        double d_start = points_distance(p, seg.start);

        if (d_end < d_start) {
            std::swap(right_point, left_point);
            std::swap(d_end, d_start);
        }

        Point mid_point = Point((right_point.x + left_point.x) / 2,
                                (right_point.y + left_point.y) / 2,
                                (right_point.z + left_point.z) / 2);
        double d_mid = points_distance(p, mid_point);

        while (std::abs(d_end - d_start) > eps) {
            mid_point = Point((right_point.x + left_point.x) / 2,
                              (right_point.y + left_point.y) / 2,
                              (right_point.z + left_point.z) / 2);
            d_mid = points_distance(p, mid_point);
            if (d_mid < d_start + eps) {
                right_point = left_point;
                left_point = mid_point;
                d_end = d_start;
                d_start = d_mid;
            }
            else {
                right_point = mid_point;
                d_end = d_mid;
            }
        }
        return d_mid;
    }

    double segments_distance(const Segment& seg1, const Segment& seg2) {
        Point start = seg1.start;
        Point end = seg1.end;
        while (points_distance(start, end) > eps) {
            Point m1(start.x + (end.x - start.x) / 3, start.y + (end.y - start.y) / 3, start.z + (end.z - start.z) / 3);
            Point m2(end.x - (end.x - start.x) / 3, end.y - (end.y - start.y) / 3, end.z - (end.z - start.z) / 3);
            if (point_segment_distance(m1, seg2) < point_segment_distance(m2, seg2) + eps)
                end = m2;
            else
                start = m1;
        }
        return point_segment_distance(start, seg2);
    }


    std::istream& operator>>(std::istream& input, Segment& seg) {
        double x, y, z;
        input >> x >> y >> z;
        seg.start = Point(x, y, z);
        input >> x >> y >> z;
        seg.end = Point(x, y, z);
        return input;
    }
}


int main() {
    std::fixed(std::cout);
    std::cout << std::setprecision(12);
    Geom::Segment seg1, seg2;
    std::cin >> seg1 >> seg2;

    std::cout << Geom::segments_distance(seg1, seg2);
    return 0;
}