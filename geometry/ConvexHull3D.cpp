#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>

struct Point {
    double x, y, z;
    Point *next;
    Point *prev;
    int num;
};


struct Face {
    int num1, num2, num3;

    void reorder() {
        if (num2 < num1 && num2 < num3) {
            std::swap(num1, num2);
            std::swap(num2, num3);
        } else if (num3 < num1 && num3 < num2) {
            std::swap(num2, num3);
            std::swap(num1, num2);
        }
    }
};

class ChanIsGenius {
public:
    std::vector<Face> build_convex_hull(const Point *input, size_t size);

private:
    Point NULL_POINT = {INFINITY, INFINITY, INFINITY, nullptr, nullptr};
    Point * NULL_POINT_PTR = &NULL_POINT;
    void perform_event(Point& p);
    static bool compare_points_by_x(const Point &a, const Point &b);

    static bool compare_faces(const Face &lhs, const Face &rhs);

    double turn(Point *p1, Point *p2, Point *p3);
    double time(Point *p1, Point *p2, Point *p3);
    void rotate(double &x, double &y, double angle);
    void rotate(Point& p, double angle);

    void build_lower_hull(Point *input, size_t size, Point **output, Point **buffer);

    const double SMALL_ANGLE = 1e-2;

    enum POINT_ACTION {
        LEFT_HALF_ACTION,
        RIGHT_HALF_ACTION,
        LEFT_HALF_RAISE,
        LEFT_HALF_DESCEND,
        RIGHT_HALF_RAISE,
        RIGHT_HALF_DESCEND
    };

};

int main() {
    int n, m;
    std::cin >> m;

    ChanIsGenius solver;
    for (int i = 0; i < m; ++i) {
        std::cin >> n;
        auto *P = new Point[n]; // input

        for (int j = 0; j < n; j++) {
            P[j].num = j;
            std::cin >> P[j].x;
            std::cin >> P[j].y;
            std::cin >> P[j].z;
        }

        auto result = solver.build_convex_hull(P, n);

        std::cout << result.size() << '\n';
        for (auto &face : result) {
            std::cout << "3 " << face.num1 << ' ' << face.num2 << ' ' << face.num3 << '\n';
        }
        delete[] P;
    }
    return 0;
}

void ChanIsGenius::perform_event(Point& p) {
    if (p.prev->next != &p) {
        p.prev->next = &p;
        p.next->prev = &p;
    } else {
        p.prev->next = p.next;
        p.next->prev = p.prev;
    }
}


// Построение полной оболочки будет состоять из построения двух "нижних" оболочек. Сначала для исходного множества,
// а затем для отраженного по вертикали.
// В целом, вся эта функция - это технические детали и подготовка данных для построения нижней оболочки
std::vector<Face> ChanIsGenius::build_convex_hull(const Point* input, size_t size) {
    std::vector<Face> result;
    auto * points = new Point[size];
    memcpy(points, input, size * sizeof(Point));
    for (size_t i = 0; i < size; i++) {
        rotate(points[i], SMALL_ANGLE);
    }
    std::sort(points, points + size, compare_points_by_x);
    points[0].prev = NULL_POINT_PTR;
    for (size_t i = 0; i < size - 1; ++i) {
        points[i].next = &points[i + 1];
        points[i + 1].prev = &points[i];
    }
    points[size - 1].next = NULL_POINT_PTR;

    auto **output = new Point *[2 * size], **buffer = new Point *[2 * size];
    // Построение "нижней" оболочки
    build_lower_hull(points, size, output, buffer);

    for (size_t i = 0; output[i] != NULL_POINT_PTR; ++i) {
        Face f = {output[i]->prev->num, output[i]->num, output[i]->next->num};
        if (output[i]->prev->next != output[i]) {
            output[i]->prev->next = output[i];
            output[i]->next->prev = output[i];
        } else {
            output[i]->prev->next = output[i]->next;
            output[i]->next->prev = output[i]->prev;
            std::swap(f.num1, f.num2);
        }
        f.reorder();
        result.push_back(f);
    }
    // Отражение
    for (size_t i = 0; i < size; i++) {
        points[i].z = -points[i].z;
    }

    points[0].prev = NULL_POINT_PTR;
    for (size_t i = 0; i < size - 1; ++i) {
        points[i].next = &points[i + 1];
        points[i + 1].prev = &points[i];
    }
    points[size].next = NULL_POINT_PTR;
    // Построение "верхней" оболочки
    build_lower_hull(points, size, output, buffer);

    for (size_t i = 0; output[i] != NULL_POINT_PTR; ++i) {
        Face f = {output[i]->prev->num, output[i]->num, output[i]->next->num};
        if (output[i]->prev->next != output[i]) {
            output[i]->prev->next = output[i];
            output[i]->next->prev = output[i];
            std::swap(f.num1, f.num2);
        } else {
            output[i]->prev->next = output[i]->next;
            output[i]->next->prev = output[i]->prev;
        }
        f.reorder();
        result.push_back(f);

    }
    std::sort(result.begin(), result.end(), compare_faces);
    delete[] points;
    delete[] output;
    delete[] buffer;
    return result;
}


// Векторное произведение для определения направления поворота.
double ChanIsGenius::turn(Point *p1, Point *p2, Point *p3) {
    if (p1 == NULL_POINT_PTR || p2 == NULL_POINT_PTR || p3 == NULL_POINT_PTR) {
        return 1.0;
    }
    return (p2->x - p1->x) * (p3->y - p1->y) - (p3->x - p1->x) * (p2->y - p1->y);
}

// По проекциям трёх точек (x, z - ty) получаем момент времени t, когда знак поворота меняется.
// То есть пытаемся найти такое t, при котором точки лежат на одной прямой.
double ChanIsGenius::time(Point *p1, Point *p2, Point *p3) {
    if (p1 == NULL_POINT_PTR || p2 == NULL_POINT_PTR || p3 == NULL_POINT_PTR) {
        return INFINITY;
    }
    return ((p2->x - p1->x) * (p3->z - p1->z) - (p3->x - p1->x) * (p2->z - p1->z)) / turn(p1, p2, p3);
}

// Непосредственно построение "нижней" оболочки. Она описывается при помощи проекции оболочки на плоскость и последовательности событий,
// возникающей при вращении оболочки вокруг координатной оси.
// input - двусвязанный список с вершинами.
// size - размер списка
// output - хранилище событий удаления и вставки
// buffer - временной хранилище
// (Предполагаем, что input отсортирован по x координате)
void ChanIsGenius::build_lower_hull(Point* input, size_t size, Point** output, Point** buffer) {
    // Терминальные случаи
    if (size == 0) { return; }
    if (size == 1) {
        output[0] = input->prev = input->next = NULL_POINT_PTR;
        return;
    }
    Point * u = input; //<- Указатель на вершину моста в левой части оболочки (см. рис. 1 в статье).

    // Продвигаемся до середины списка, чтобы начать фазу построения первоначального моста.
    for (size_t i = 0; i < size / 2 - 1; ++i) {
        u = u->next;
    }
    // v - самая левая вершина в правой части (см. тот же рисунок).
    Point * v = u->next;
    Point * mid = u->next;

    // Рекурсивно строим левую и правую оболочку.
    build_lower_hull(input, size / 2, buffer, output);
    build_lower_hull(mid, size - size / 2, buffer + size / 2 * 2, output + size / 2 * 2);

    // Сливание.
    // Находим стартовый мост. Пока обе пары векторов не образуют поворот против часовой стрелки продвигаем указатели
    // вдоль оболочек влево и вправо соотвественно.
    while (true) {
        if (turn(u, v, v->next) < 0) {
            v = v->next;
        }
        else if (turn(u->prev, u, v) < 0) {
            u = u->prev;
        }
        else {
            break;
        }
    }
    // Теперь будем менять t от -inf до +inf и получим цепочку событий, которые определяют нашу нижнюю оболочку.
    // Известно, что существует всего 6 типов событий
    // 0 - удаление/вставка в левой половине
    // 1 - удаление/вствка в правой половине
    // 2 - вершина u поднялась, так что u->prev, u, v более не образуют правый поворот, поэтому удаляем u, новый мост - u->prev, v
    // 3 - вершина u->next опустилась, так что нужно добавить её в оболочку, новый мост - u->next, v
    // 4 - вершина v опустилась, так что удаляем её из оболочки, новый мост - u, v->next
    // 5 - веришна v->prev поднялась, так что добавлвяем её в оболчку, новый мост - u, v->prev
    // (условия в пунктах 3 - 5 определяются направлением поворота аналогично пункту 2)
    double t[6];
    double old_time = -INFINITY; // < Эта переменная нужна нам, чтобы мы не обрабатывали старые события повторно
    double new_time;
    int result = 0;

    // Процесс слияния всех событий в правильном хронологическом порядке
    for (size_t left = 0, right = size / 2 * 2 ; ;) {
        t[LEFT_HALF_ACTION] = time(buffer[left]->prev, buffer[left], buffer[left]->next);
        t[RIGHT_HALF_ACTION] = time(buffer[right]->prev, buffer[right], buffer[right]->next);
        t[LEFT_HALF_RAISE] = time(u, u->next, v);
        t[LEFT_HALF_DESCEND] = time(u->prev, u, v);
        t[RIGHT_HALF_RAISE] = time(u, v->prev, v);
        t[RIGHT_HALF_DESCEND] = time(u, v, v->next);
        POINT_ACTION min_index = LEFT_HALF_ACTION;
        new_time = INFINITY;
        for (size_t k = LEFT_HALF_ACTION; k <= RIGHT_HALF_DESCEND; k++) {
            // Левое неравенство нужно для предотвращения повторных добавлений событий
            if (t[k] > old_time && t[k] < new_time) {
                min_index = static_cast<POINT_ACTION>(k);
                new_time = t[k];
            }
        }
        if (new_time == INFINITY) { break; }
        // Действия напрямую вытекают из упомянутого выше рисунка из статьи
        switch (min_index) {
            case LEFT_HALF_ACTION:
                if (buffer[left]->x < u->x) {
                    output[result++] = buffer[left];
                }
                perform_event(*buffer[left++]);
                break;
            case RIGHT_HALF_ACTION:
                if (buffer[right]->x > v->x) {
                    output[result++] = buffer[right];
                }
                perform_event(*buffer[right++]);
                break;
            case LEFT_HALF_RAISE:
                output[result++] = u = u->next;
                break;
            case LEFT_HALF_DESCEND:
                output[result++] = u;
                u = u->prev;
                break;
            case RIGHT_HALF_RAISE:
                output[result++] = v = v->prev;
                break;
            case RIGHT_HALF_DESCEND:
                output[result++] = v;
                v = v->next;
                break;
            default:
                break;
        }
        old_time = new_time;
    }

    // Так мы обозначим конец последовательности событий
    output[result] = NULL_POINT_PTR;

    // Завершаем слияние
    u->next = v;
    v->prev = u;

    // Откатываем все события
    for (result--; result >= 0; result--) {
        if (output[result]->x <= u->x || output[result]->x >= v->x) {
            // Если мы сейчас откатываем вставку произошедшую строго внутри одной из половин, (то есть оба неравенства строгие)
            // значит достаточно просто выполнить perfrom_event и в дальнейший if мы не попадём. Иначе если достингнуто равенство,
            // то мы имеем дело со случаями 2 или 4 поэтому нужно не только выполнить act, но и исправить испорченные мосты. (для этого и нужен дальнейший if)
            perform_event(*output[result]);
            if (output[result] == u) {
                u = u->prev;
            }
            else if (output[result] == v) {
                v = v->next;
            }
        }
            // Сюда попадаем, если было удаление, и для его отката act не сработает, поэтому исправляем prev и next.
            // if аналогично предыдущему пунтку исправляет мосты
        else {
            u->next = output[result];
            output[result]->prev = u;
            v->prev = output[result];
            output[result]->next = v;
            if (output[result]->x < mid->x) {
                u = output[result];
            }
            else {
                v = output[result];
            }
        }
    }
}


bool ChanIsGenius::compare_faces(const Face& lhs, const Face& rhs) {
    if (lhs.num1 != rhs.num1) {
        return lhs.num1 < rhs.num1;
    }
    if (lhs.num2 != rhs.num2) {
        return lhs.num2 < rhs.num2;
    }
    return lhs.num3 < rhs.num3;
}

void ChanIsGenius::rotate(double& x, double& y, double angle) {
    double tmp_x = x * cos(angle) - y * sin(angle);
    double tmp_y = x * sin(angle) + y * cos(angle);
    x = tmp_x;
    y = tmp_y;
}

void ChanIsGenius::rotate(Point &p, double angle) {
    rotate(p.x, p.y, angle);
    rotate(p.z, p.x, angle);
    rotate(p.y, p.z, angle);
}

bool ChanIsGenius::compare_points_by_x(const Point& a, const Point& b) {
    return a.x < b.x;
}
