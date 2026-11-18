#include <jni.h>
#include <string>
#include <cmath>
#include <cstdint>
#include <algorithm>

extern "C" JNIEXPORT jint JNICALL
Java_com_example_duplicatecleaner_NativeLib_calculateNativeSimilarity(
        JNIEnv* env,
        jobject /* this */,
        jlong dHashA,
        jlong aHashA,
        jint rAvgA,
        jint gAvgA,
        jint bAvgA,
        jlong dHashB,
        jlong aHashB,
        jint rAvgB,
        jint gAvgB,
        jint bAvgB) {

    uint64_t xorD = static_cast<uint64_t>(dHashA ^ dHashB);
    uint64_t xorA = static_cast<uint64_t>(aHashA ^ aHashB);

#if defined(__GNUC__) || defined(__clang__)
    int dHashDiff = __builtin_popcountll(xorD);
    int aHashDiff = __builtin_popcountll(xorA);
#else
    int dHashDiff = 0;
    for (uint64_t v = xorD; v > 0; v >>= 1) dHashDiff += (v & 1);
    int aHashDiff = 0;
    for (uint64_t v = xorA; v > 0; v >>= 1) aHashDiff += (v & 1);
#endif

    double colorDiff = (std::abs(rAvgA - rAvgB) + std::abs(gAvgA - gAvgB) + std::abs(bAvgA - bAvgB)) / 3.0;

    double dHashScore = ((64.0 - dHashDiff) / 64.0) * 100.0;
    double aHashScore = ((64.0 - aHashDiff) / 64.0) * 100.0;
    double colorScore = ((255.0 - colorDiff) / 255.0) * 100.0;

    double finalScore = (dHashScore * 0.65) + (aHashScore * 0.20) + (colorScore * 0.15);
    int similarity = static_cast<int>(finalScore);

    if (similarity < 0) similarity = 0;
    if (similarity > 100) similarity = 100;

    return similarity;
}
