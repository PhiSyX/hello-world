#include <cstddef>
#include <cstdint>
#include <iostream>

using std::cout;
using std::endl;

template <typename T>
struct Vector
{
    void push_back(const T &t)
    {
        if (size + 1 > capacity)
        {
            if (capacity == 0)
            {
                capacity = 1;
            }

            auto new_item = new T[capacity *= 2];

            for (size_t c = 0; c < size; ++c)
            {
                new_item[c] = items[c];
            }

            delete[] items;

            items = new_item;
        }

        items[size++] = t;
    }

    T &operator[](size_t position)
    {
        return items[position];
    }

    template <typename C>
    bool is_sorted(C predicate)
    {
        if (!size)
        {
            return true;
        }

        for (size_t c = 0; c < size - 1; ++c)
        {
            if (!predicate(items[c], items[c + 1]))
            {
                return false;
            }
        }

        return true;
    }

    ~Vector()
    {
        delete[] items;
    }

    size_t size{0},
        capacity{0};

    T *items{nullptr};
};

int main()
{
    Vector<uint32_t> vec;

    for (unsigned int n = 0; n < 10; ++n)
    {
        vec.push_back(n);
    }

    cout << vec.size << endl;
    cout << vec.is_sorted([](const auto &a, const auto &b)
                          { return a < b; })
         << endl;
}