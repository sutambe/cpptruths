#include <assert.h>
#include <stdio.h>
#include <utility>
#include <new>

template<class T, int Cap>
class fixed_capacity_vector {
    int size_ = 0;
    alignas(T) char buffer_[Cap][sizeof(T)];
public:
    fixed_capacity_vector() = default;
    explicit fixed_capacity_vector(int initial_size) noexcept {
        for (int i=0; i < initial_size; ++i) {
            this->emplace_back();
        }
    }
    fixed_capacity_vector(const fixed_capacity_vector& rhs) noexcept {
        for (const auto& elt : rhs) {
            this->emplace_back(elt);
        }
    }
    fixed_capacity_vector(fixed_capacity_vector&& rhs) noexcept {
        for (auto& elt : rhs) {
            this->emplace_back(std::move(elt));
        }
    }
    fixed_capacity_vector& operator=(const fixed_capacity_vector& rhs) = delete;
    ~fixed_capacity_vector() {
        for (int i=0; i < size_; ++i) {
            (*this)[i].~T();
        }
    }

    template<class... Args>
    void emplace_back(Args&&... args) {
        assert(size_ < Cap);
        new ((void*)buffer_[size_]) T(std::forward<Args>(args)...);
        size_ += 1;
    }

    template<class F>
    void emplace_back_with_result_of(const F& factory_fn) {
        assert(size_ < Cap);
        new ((void*)buffer_[size_]) T(factory_fn());
        size_ += 1;
    }

    const T& operator[](int i) const { return *(const T*)buffer_[i]; }
    T& operator[](int i) { return *(T*)buffer_[i]; }
};

struct Widget {
    int value_;
    explicit Widget(int i) : value_(i) { puts("construct from int"); }
    Widget(Widget&&) noexcept { puts("move-ctor"); }
    Widget& operator=(Widget&&) noexcept { puts("move-assign"); return *this; }
    Widget(const Widget&) noexcept { puts("copy-ctor"); }
    Widget& operator=(const Widget&) noexcept { puts("copy-assign"); return *this; }
    ~Widget() { puts("destructor"); };
    static Widget make() { return Widget(42); }
};

int main()
{
    fixed_capacity_vector<Widget, 10> vec;
    puts("----with emplace_back----");
    vec.emplace_back(Widget::make());
    puts("----with emplace_back_with_result_of----");
    vec.emplace_back_with_result_of([&]() { return Widget::make(); });
    puts("----done----");
}
