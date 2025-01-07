#include <stdint.h>
#include <stdio.h>

// Crossfade function
int32_t crossfade(int32_t a, int32_t b, uint32_t f) {
    // Ensure f is within bounds [0, 4095]
    if (f > 4095) {
        f = 4095;
    }

    // Calculate the crossfade using fixed-point arithmetic
    int64_t diff = (int64_t)(b - a);  // Difference between values (prevent overflow)
    int64_t crossfade_val = (int64_t)a * (4095) + diff * f;

    // Divide by 4095 to normalize
    int32_t result = (int32_t)(crossfade_val / 4095);

    return result;  // Return the blended value
}

int main() {
    int32_t a = -524288;  // Example: +2^19 (max)
    int32_t b = 524288; // Example: -2^19 (min)

    // Test crossfade with different values of f
    for (uint32_t f = 0; f <= 4095; f += 255) {
        int32_t output = crossfade(a, b, f);
        printf("f: %4u -> Crossfade Output: %d\n", f, output);
    }

    return 0;
}
