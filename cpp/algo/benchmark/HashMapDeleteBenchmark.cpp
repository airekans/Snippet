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
static void BM_HashMapDelete(benchmark::State& state)
{
    HashMap<int, T> hash_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.Insert(i, value);
        }

        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.Delete(i);
        }
    }
}

template<typename T>
static void BM_StdMapDelete(benchmark::State& state)
{
    std::map<int, T> std_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map[i] = value;
        }

        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map.erase(i);
        }
    }
}

template<typename T>
static void BM_StdUnorderedMapDelete(benchmark::State& state)
{
    std::tr1::unordered_map<int, T> std_map;
    T value = GetValue<T>::Get();
    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map[i] = value;
        }

        for (int i = 0; i < state.range_x(); ++i)
        {
            std_map.erase(i);
        }
    }
}

BENCHMARK_TEMPLATE(BM_HashMapDelete, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdMapDelete, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdUnorderedMapDelete, int)->Range(8, 8<<10);

BENCHMARK_TEMPLATE(BM_HashMapDelete, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdMapDelete, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE(BM_StdUnorderedMapDelete, std::string)->Range(8, 8<<10);


BENCHMARK_MAIN();


