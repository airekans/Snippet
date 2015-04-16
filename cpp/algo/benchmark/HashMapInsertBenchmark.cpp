#include "HashMap.h"

#include <benchmark/benchmark.h>

#include <string>
#include <tr1/unordered_map>
#include <map>
#include <utility>

using namespace snippet::algo;

static const std::string gs_str_value = "0123456789";

template<typename T>
struct GetValue;

template<>
struct GetValue<int>
{
    static int Get()
    {
        return 123456;
    }
};

template<>
struct GetValue<std::string>
{
    static const std::string& Get()
    {
        return gs_str_value;
    }
};

template<typename T>
static void BM_HashMapInsert(benchmark::State& state)
{
    HashMap<int, T> hash_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.Insert(i, value);
        }
    }
}

template<typename T>
static void BM_StdMapInsert(benchmark::State& state)
{
    std::map<int, T> std_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map.insert(std::make_pair(i, value));
        }
    }
}

template<typename T>
static void BM_StdUnorderedMapInsert(benchmark::State& state)
{
    std::tr1::unordered_map<int, T> std_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map.insert(std::make_pair(i, value));
        }
    }
}

// Register the function as a benchmark
// BM for <int, int>
BENCHMARK_TEMPLATE(BM_HashMapInsert, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdMapInsert, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdUnorderedMapInsert, int)->Range(8, 8<<10);

BENCHMARK_TEMPLATE(BM_HashMapInsert, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdMapInsert, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdUnorderedMapInsert, std::string)->Range(8, 8<<10);


BENCHMARK_MAIN();


