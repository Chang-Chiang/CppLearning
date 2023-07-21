template <typename Iterator, typename Compare> // 迭代器类型, 函数对象
Iterator my_find_if(Iterator first, Iterator last, Compare comp) {
    for (; first != last; ++first) {
        if (comp(*first)) { // comp.operator()(*first)
            return first;
        }
    }
    return last;
}

template <typename Compare, typename T>
class _mybind1st { // 绑定器是函数对象的一个应用
public:
    _mybind1st(Compare comp, T val) : _comp(comp), _val(val) {}
    bool operator()(const T& second) { return _comp(_val, second); }

private:
    Compare _comp;
    T       _val;
};

template <typename Compare, typename T> // 二元函数对象, 元素类型
_mybind1st<Compare, T> mybind1st(Compare comp, const T& val) {
    // 直接使用函数模板, 好处是, 可以进行类型的推演
    return _mybind1st<Compare, T>(comp, val); // 返回一元函数对象
}
