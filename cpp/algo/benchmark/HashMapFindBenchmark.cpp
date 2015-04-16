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

template<typename C, typename T>
static void BM_HashMapConstFind(benchmark::State& state)
{
    C hash_map;
    T value = GetValue<T>::Get();
    for (int i = 0; i < state.range_x(); ++i)
    {
        hash_map[i] = value;
    }
    int key_to_find = state.range_x() - 1;

    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.Find(key_to_find, value);
        }
    }
}

template<typename C, typename T>
static void BM_StdMapConstFind(benchmark::State& state)
{
    C hash_map;
    T value = GetValue<T>::Get();
    for (int i = 0; i < state.range_x(); ++i)
    {
        hash_map[i] = value;
    }
    int key_to_find = state.range_x() - 1;

    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.find(key_to_find);
        }
    }
}

typedef HashMap<int, int> IntHashMap;
typedef std::map<int, int> IntStdMap;
typedef std::tr1::unordered_map<int, int> IntUnorderedMap;

typedef HashMap<int, std::string> StrHashMap;
typedef std::map<int, std::string> StrStdMap;
typedef std::tr1::unordered_map<int, std::string> StrUnorderedMap;

BENCHMARK_TEMPLATE2(BM_HashMapConstFind, IntHashMap, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapConstFind, IntStdMap, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapConstFind, IntUnorderedMap, int)->Range(8, 8<<10);

BENCHMARK_TEMPLATE2(BM_HashMapConstFind, StrHashMap, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapConstFind, StrStdMap, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapConstFind, StrUnorderedMap, std::string)->Range(8, 8<<10);

template<typename C, typename T>
static void BM_HashMapSeqFind(benchmark::State& state)
{
    C hash_map;
    T value = GetValue<T>::Get();
    for (int i = 0; i < state.range_x(); ++i)
    {
        hash_map[i] = value;
    }

    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.Find(i, value);
        }
    }
}

template<typename C, typename T>
static void BM_StdMapSeqFind(benchmark::State& state)
{
    C hash_map;
    T value = GetValue<T>::Get();
    for (int i = 0; i < state.range_x(); ++i)
    {
        hash_map[i] = value;
    }

    while (state.KeepRunning())
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            hash_map.find(i);
        }
    }
}

BENCHMARK_TEMPLATE2(BM_HashMapSeqFind, IntHashMap, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapSeqFind, IntStdMap, int)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapSeqFind, IntUnorderedMap, int)->Range(8, 8<<10);

BENCHMARK_TEMPLATE2(BM_HashMapSeqFind, StrHashMap, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapSeqFind, StrStdMap, std::string)->Range(8, 8<<10);
BENCHMARK_TEMPLATE2(BM_StdMapSeqFind, StrUnorderedMap, std::string)->Range(8, 8<<10);


BENCHMARK_MAIN();



