#include <iostream>
#include <vector>
#include <string>

const size_t ALPHABET_SIZE = 26;

struct TTrie {
public:
    std::vector<int> find(const std::string& text);
    explicit TTrie(const std::string& pattern) : current_state(0), pattern(pattern),
                                                 pattern_size(pattern.size()) {
        state.emplace_back(-1, -1, 0); // Инициализация корня
    }

private:
    struct TVertex {
        std::vector<int> next;
        int              parent;
        size_t           depth;
        int              income_symbol;
        int              link;

        TVertex() : next(ALPHABET_SIZE, -1), parent(-1), depth(-1), income_symbol(-1), link(-1) {}

        explicit TVertex(int parent = -1, int income_symbol = -1, int depth = -1,
                         int link = -1) : next(ALPHABET_SIZE, -1), parent(parent),
                                          depth(depth), income_symbol(income_symbol), link(link) {}
    };

    int                  current_state;
    std::vector<TVertex> state;
    const std::string&   pattern;
    size_t               pattern_size;

    // Функция перехода в автомате.
    int go(int state, int symbol);
    // Функция возвращает суффиксиальную ссылку для данного индекса
    int get_link(int state);
};


// Функция находит все вхождения шаблона (где '?' - произвольный символ алфавита) в тексте.
// Возвращаемое значение - индексы начала вхождения шаблона.
std::vector<int> TTrie::find(const std::string& text) {
    std::vector<int> result;

    for (size_t i = 0; i < text.size(); ++i) {
        current_state = go(current_state, text[i] - 'a');
        if (state[current_state].depth == pattern_size) {
            result.push_back(i - pattern_size + 1);
        }
    }

    return result;
}


int TTrie::get_link(int state_ind) {
    if (state[state_ind].link != -1) {
        return state[state_ind].link;
    }
    if (state_ind == 0 || state[state_ind].parent == 0) {
        return state[state_ind].link = 0;
    }
    return state[state_ind].link = go(get_link(state[state_ind].parent), state[state_ind].income_symbol);
}


int TTrie::go(int state_ind, int symbol) {
    if (state[state_ind].next[symbol] != -1) {
        return state[state_ind].next[symbol];
    }
    // Создаем вершину в автомате, есть совпадение с шаблоном.
    // Так мы получим не все возможные варианты строк (каждый '?' - любой символ),
    // а только те, которые действительно встречаются в тексте.
    if (state[state_ind].depth != pattern_size &&
        (pattern[state[state_ind].depth] - 'a' == symbol || pattern[state[state_ind].depth] == '?')) {
        state.emplace_back(state_ind, symbol, state[state_ind].depth + 1);
        return state[state_ind].next[symbol] = state.size() - 1;
    }
    if (state_ind == 0) {
        return state[state_ind].next[symbol] = 0;
    }
    return state[state_ind].next[symbol] = go(get_link(state_ind), symbol);
}
