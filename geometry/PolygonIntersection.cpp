#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

namespace Geom {
    const double eps = 1e-10;
    const double inf = 1e9;
    struct Point {
        double x, y;
        Point(double x, double y) : x(x), y(y) {}
        Point(const Point& p) : x(p.x), y(p.y) {}
        Point operator+(const Point& p) const {
            return {this->x + p.x, this->y + p.y};
        }
        Point operator-(const Point& p) const {
            return {this->x - p.x, this->y - p.y};
        }
        double get_distance() const {
            return sqrt(x * x + y * y);
        }
        bool operator==(const Point& p) const {
            return std::abs(this->x - p.x) < eps && std::abs(this->y - p.y) < eps;
        }
    };

    struct Segment {
        Point start, end;
        Segment(Point start, Point end) : start(start), end(end) {}
    };

    struct Polygon {
        std::vector<Point> vertices;
        Polygon(const std::vector<Point>& vertices) : vertices(vertices) {}
        Polygon() = default;

        Point& operator[](size_t index) {
            return vertices[index % vertices.size()];
        }

        const Point& operator[](size_t index) const {
            return vertices[index % vertices.size()];
        }

        void add_vertice(const Point& p) {
            vertices.push_back(p);
        }

        void reverse() {
            for (auto& i : vertices) {
                i.x = -i.x;
                i.y = -i.y;
            }
        }

        int get_lowest_point() const {
            Point current_min(inf, inf);
            int index = -1;
            for (int i = 0; i < vertices.size(); ++i) {
                if (vertices[i].y < current_min.y) {
                    current_min = vertices[i];
                    index = i;
                }
                if (std::abs(vertices[i].y - current_min.y) < eps && std::abs(vertices[i].x - current_min.x) < eps) {
                    current_min = vertices[i];
                    index = i;
                }
            }
            return index;
        }
    };

    double angle(const Point& b, const Point& p2, const Point& p3)  {
        Point a(p3.x - p2.x, p3.y - p2.y);
        if (a == Point{0, 0} || b == Point{0, 0}) {
            return 0;
        }
        return acos((a.x * b.x + a.y * b.y) / (a.get_distance() * b.get_distance()));
    }

    Polygon get_minkowski_sum(Polygon& p1, Polygon& p2) {
        int i = 0, j = 0;
        int n = p1.vertices.size(), m = p2.vertices.size();
        int start1 = p1.get_lowest_point(), start2 = p2.get_lowest_point();
        Point previous(-1, 0);
        Polygon result;
        while (i < n || j < m) {
            result.add_vertice(p1[start1 + i] + p2[start2 + j]);
            if (angle(previous, p1[start1 + i], p1[start1 + i + 1]) <
                angle(previous, p2[start2 + j], p2[start2 + j + 1])) {
                previous.x = (p1[start1 + i + 1] - p1[start1 + i]).x;
                previous.y = (p1[start1 + i + 1] - p1[start1 + i]).y;
                ++i;
            }

            else if (angle(previous, p1[start1 + i], p1[start1 + i + 1]) >
                     angle(previous, p2[start2 + j], p2[start2 + j + 1])) {
                previous.x = (p2[start2 + j + 1] - p2[start1 + j]).x;
                previous.y = (p2[start2 + j + 1] - p2[start1 + j]).y;
                ++j;
            }
            else {
                previous.x = (p2[start2 + j + 1] - p2[start1 + j]).x;
                previous.y = (p2[start2 + j + 1] - p2[start1 + j]).y;
                ++j;
                ++i;
            }
        }
        return result;
    }


    double points_distance(const Point& p1, const Point& p2) {
        return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    }

    bool is_point_inside(const Point& p, const Polygon& pol) {
        int n = pol.vertices.size();
        for (int i = 0; i < n; ++i) {
            Point v1 = p - pol.vertices[(i + 1) % n];
            Point v2 = pol[i] - pol[i + 1];
            if (v1.x * v2.y - v1.y * v2.x > 0) {
                return false;
            }
        }
        return true;
    }

    std::istream& operator>>(std::istream& in, Polygon& p) {
        int n;
        std::cin >> n;
        for (int i = 0; i < n; ++i) {
            double x, y;
            in >> x >> y;
            p.add_vertice({x, y});
        }
        return in;
    }

}


int main() {
    Geom::Polygon a, b;
    std::cin >> a >> b;
    a.reverse();
    Geom::Polygon c = Geom::get_minkowski_sum(a, b);

    if (Geom::is_point_inside({0, 0}, c)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
