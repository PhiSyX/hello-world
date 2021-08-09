#include <algorithm>
#include <vector>
#include <parallel/algorithm>
#include <memory>

int cmp(const void *a, const void *b);
void fn();

int main(int argc, char **argv)
{
    auto lim = atoi(argv[1]);

    std::vector<int> vec;

    vec.reserve(lim);

    while (--lim)
        vec.push_back(random());

    if (*argv[2] == 'q')
    {
        qsort(&vec[0], vec.size(), sizeof(int), cmp);
    }
    else if (*argv[2] == 'p')
    {
        __gnu_parallel::sort(
            vec.begin(),
            vec.end(),
            [](const auto &a, const auto &b)
            { return a < b; });
    }
    else
    {
        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b)
                  { return a < b; });
    }

    fn();
}

int cmp(const void *a, const void *b)
{
    if (*(int *)a < *(int *)b)
    {
        return -1;
    }
    else if (*(int *)a > *(int *)b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void fn()
{
    FILE *file = fopen("/etc/passwd", "r");
    if (!file)
        ; // do error things

    std::shared_ptr<FILE> ptr(file, fclose);

    char buffer[1024];
    fread(buffer, sizeof(buffer), 1, ptr.get());
}
