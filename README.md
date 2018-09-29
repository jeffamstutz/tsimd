# tsimd - Fundamental C++ SIMD types for Intel CPUs (sse to avx512)

This library is header-only and is implemented according to which Intel ISA
flags are enabled in the translation unit for which they are used (e.g. -mavx
with gcc or clang).

Master Status: [![Build Status](https://travis-ci.org/jeffamstutz/tsimd.svg?branch=master)](https://travis-ci.org/jeffamstutz/tsimd)

## TODOs (contributions welcome!)

- unsigned integer pack<> types
- support for other CPU ISAs

## Build Requirements

### Using tsimd

- C++11 compiler

(unofficial list of compilers, not all are tested)

* GCC >= 4.8.1
* clang >= 3.4
* ICC >= 16
* Visual Studio 2015 (64-bit target)

### Building tsimd's examples/benchmarks/tests and installing from soure

- cmake >= 3.2

## Library layout and usage

The library is logically composed of 3 different components:

1. The pack<T, W> class, which is a _logical_ SIMD register
2. Functions which can load and store packs in and out of larger arrays.
3. Operators and functions to manipulate packs.

While there does not yet exist any true documentation, users are encouraged to
see what type aliases are defined in ```tsimd/detail/pack.h```, as well as what
operators and functions are available in ```tsimd/detail/operators/``` and
```tsimd/detail/functions/``` respectively. Generally speaking, each header
found in ```detail/``` encapsulates exactly one type, operator, or function to
aide in discovery.

## Example

### SAXPY

Consider the following function (kernel) taking values from two input arrays
and storing in an output array.

```cpp
// NOTE: n is the length of all 3 arrays
void saxpy(float a, int n, float x[], float y[], float out[])
{
  for (int i = 0; i < n; ++i) {
    const float xi = x[i];
    const float yi = y[i];
    const float result = a * xi + yi;
    out[i] = result;
  }
}
```

This kernel ends up applying the exact same formula to every element in the
data. SIMD instructions enable us to reduce the total number of iterations by a
factor of the CPU's SIMD register size. We do this by using tsimd types
instead of builtin types.

```cpp
// NOTE: n is the length of all 3 arrays
void saxpy_tsimd(float a, int n, float x[], float y[], float out[])
{
  using namespace tsimd;
  for (int i = 0; i < n; i += vfloat::static_size) {
    const vfloat xi = load<vfloat>(&x[i]);
    const vfloat yi = load<vfloat>(&y[i]);
    const vfloat result = a * xi + yi; // same formula!
    store(result, &out[i]);
  }
}
```

The advantage to this version (instead of using a specific SIMD width, say
```vfloat4``` or ```vfloat8```) is that the kernel function will be "widened"
to the best available width based on how it gets compiled. In other words:
4-wide for SSE, 8-wide for AVX/AVX2, and 16-wide for AVX512.
