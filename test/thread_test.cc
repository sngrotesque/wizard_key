#include <WukMemory.hh>
#include <WukSIMD.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>

using namespace wuk::simd;

#define SPEED_TEST(func) \
    func; \
    double start = timer.time(); \
    func; \
    double stop = timer.time(); \
    double taken_time = stop - start; \
    double throughput = length / taken_time / (1024 * 1024); \
    printf("Token time: %.4lf\n", taken_time); \
    printf("Speed: %.2lf MB/s.\n", throughput);

template <typename T>
T *test_alloc(size_t length, size_t aligned_size)
{
    void *raw = operator new[](sizeof(T) * length, std::align_val_t(aligned_size), std::nothrow);
    if (!raw) return nullptr;

    T *p = static_cast<T *>(raw);

    // 构造对象（如果是 POD 类型可以省略）
    for (size_t i = 0; i < length; ++i) {
        new (p + i) T();
    }

    return p;
}

template <typename T>
void test_free(T* p, size_t length, size_t aligned_size)
{
    for (size_t i = 0; i < length; ++i) {
        p[i].~T();
    }

    operator delete[](p, std::align_val_t(aligned_size));
}

void sse_xor(wByte *out, const wByte *in, wSize length, wByte key[16])
{
    if (!out || !in || !key) {
        throw wuk::Exception(wuk::Error::ERR, "sse_xor",
            "out or in or key is nullptr.");
    }
    if (!length) {
        throw wuk::Exception(wuk::Error::ERR, "sse_xor",
            "length is 0.");
    }
    sse::s128 k = sse::loadu128(key);

    for (wSize i = 0; i < length; i += 16) {
        sse::s128 buffer = sse::loadu128(in + i);
        sse::s128 res = sse::xor128(buffer, k);
        sse::storeu128(out + i, res);
    }
}

void test()
{
    wuk::Time timer;
    wByte *key = (wByte *)"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\5\5\5\5\5\5\5\5\5\5\5\5\5\5\5\5";
    wSize length = 1024ULL * 1024 * 1024;
    wByte *input = test_alloc<wByte>(length, 32);
    if (!input) {
        throw wuk::Exception(wuk::Error::MEMORY, "test",
            "failed to allocate for input.");
    }
    wByte *output = test_alloc<wByte>(length, 32);
    if (!output) {
        throw wuk::Exception(wuk::Error::MEMORY, "test",
            "failed to allocate for output.");
    }

    SPEED_TEST(sse_xor(output, input, length, key));
    for (wSize i = 0; i < length; i += 16) {
        if (memcmp(output + i, key, 16) != 0) {
            test_free<wByte>(output, length, 32);
            test_free<wByte>(input, length, 32);
            throw wuk::Exception(wuk::Error::ERR, "test",
                "failed to encryption.");
        }
    }

    test_free<wByte>(output, length, 32);
    test_free<wByte>(input, length, 32);
}

void _thread_test(wByte *out, const wByte *in, wSize length, wByte key[16], wU32 thread_count)
{
    wSize chunk = length / thread_count;
    std::vector<std::thread> threads;
    for (wU32 i = 0; i < thread_count; ++i) {
        threads.emplace_back(sse_xor, out + i * chunk, in + i * chunk, chunk, key);
    }
    for (auto &t : threads) {
        t.join();
    }
}

void thread_test()
{
    wSize length = 4 * 1024ULL * 1024 * 1024;
    wByte *input = test_alloc<wByte>(length, 32);
    if (!input) {
        throw wuk::Exception(wuk::Error::MEMORY, "test",
            "failed to allocate for input.");
    }
    wByte *output = test_alloc<wByte>(length, 32);
    if (!output) {
        test_free<wByte>(input, length, 32);
        throw wuk::Exception(wuk::Error::MEMORY, "test",
            "failed to allocate for output.");
    }
    wByte *key = (wByte *)"AAAAAAAAAAAAAAAA";
    wuk::Time timer;

    SPEED_TEST(_thread_test(output, input, length, key, 8));

    for (wSize i = 0; i < length; i += 16) {
        if (memcmp(output + i, key, 16) != 0) {
            printf("Encryption failed.\n");
            break;
        }
    }

    test_free<wByte>(output, length, 32);
    test_free<wByte>(input, length, 32);
}

int main()
{
    try {
        thread_test();
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
