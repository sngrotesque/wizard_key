#include <WukMemory.hh>
#include <WukSIMD.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>

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

void sse_test(wByte *out, const wByte *in, wSize length, wByte key[16])
{
    if (!out || !in || !key) {
        throw wuk::Exception(wuk::Error::ERR, "sse_test",
            "out or in or key is nullptr.");
    }
    if (!length) {
        throw wuk::Exception(wuk::Error::ERR, "sse_test",
            "length is 0.");
    }
    sse::s128 k = sse::loadu128(key);

    for (wSize i = 0; i < length; i += 16) {
        sse::s128 buffer = sse::loadu128(in + i);
        sse::s128 res = sse::xor128(buffer, k);
        sse::storeu128(out + i, res);
    }
}

void avx_test(wByte *out, const wByte *in, wSize length, wByte key[32])
{
    if (!out || !in || !key) {
        throw wuk::Exception(wuk::Error::ERR, "sse_test",
            "out or in or key is nullptr.");
    }
    if (!length) {
        throw wuk::Exception(wuk::Error::ERR, "sse_test",
            "length is 0.");
    }
    avx::s256 k = avx::loadu256(key);

    for (wSize i = 0; i < length; i += 32) {
        avx::s256 buffer = avx::loadu256(in + i);
        avx::s256 res = avx::xor256(buffer, k);
        avx::storeu256(out + i, res);
    }
}

void test()
{
    wuk::WukTime timer;

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

    // SPEED_TEST(sse_test(output, input, length, key));
    // for (wSize i = 0; i < length; i += 16) {
    //     if (memcmp(output + i, key, 16) != 0) {
    //         wuk::m_free(output);
    //         wuk::m_free(input);
    //         throw wuk::Exception(wuk::Error::ERR, "test",
    //             "failed to encryption.");
    //     }
    // }

    SPEED_TEST(avx_test(output, input, length, key));
    for (wSize i = 0; i < length; i += 32) {
        if (memcmp(output + i, key, 32) != 0) {
            test_free<wByte>(output, length, 32);
            test_free<wByte>(input, length, 32);
            throw wuk::Exception(wuk::Error::ERR, "test",
                "failed to encryption.");
        }
    }

    test_free<wByte>(output, length, 32);
    test_free<wByte>(input, length, 32);
}

int main()
{
    try {
        test();
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
