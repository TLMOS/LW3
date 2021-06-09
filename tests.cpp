#include <chrono>
#include <iostream>
#include <thread>
#include "tests.h"
#include "binary_tree.h"
#include "set.h"
#include <fstream>

void TimeTestTree(int max, int step, int iters) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    
    std::ofstream f_insert;
    std::ofstream f_remove;
    std::ofstream f_contains;
    std::string path_insert = "./time_estimation/txt/insert_" + std::to_string(max) + ".txt";
    std::string path_remove = "./time_estimation/txt/remove_" + std::to_string(max) + ".txt";
    std::string path_contains = "./time_estimation/txt/contains_" + std::to_string(max) + ".txt";
    f_insert.open(path_insert);
    f_remove.open(path_remove);
    f_contains.open(path_contains);

    BTree<int>* tree = new BTree<int>();
    for (int n = 0; n <= max; n += step)
    {
        double acc_insert = 0;
        double acc_remove = 0;
        double acc_contains = 0;
        for (int i = 0; i <= step; i++)
            tree->Insert(std::rand());
        for (int i = 0; i <= iters; i++)
        {
            int x = std::rand();
            auto t1 = high_resolution_clock::now();
            tree->Insert(x);
            auto t2 = high_resolution_clock::now();
            duration<double, std::milli> ms_double = t2 - t1;
            acc_insert += ms_double.count();
            
            t1 = high_resolution_clock::now();
            tree->Remove(x);
            t2 = high_resolution_clock::now();
            ms_double = t2 - t1;
            acc_remove += ms_double.count();
            
            x = std::rand();
            t1 = high_resolution_clock::now();
            tree->Contains(x);
            t2 = high_resolution_clock::now();
            ms_double = t2 - t1;
            acc_contains += ms_double.count();
        }
        acc_insert /= iters;
        acc_remove /= iters;
        acc_contains /= iters;
        f_insert << n << " " << acc_insert << "\n";
        f_remove << n << " " << acc_remove << "\n";
        f_contains << n << " " << acc_contains << "\n";
    }
    f_insert.close();
    f_remove.close();
    f_contains.close();
    delete tree;
}

void TestTreeCreation(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Creation: ";

    //Empty
    BTree<int>* tree = new BTree<int>();
    assert(tree->GetSize() == 0);
    delete(tree);

    //By items and count, copying 
    size_t max_size = 15;
    int* items = (int*)std::malloc(max_size * sizeof(int));
    for (size_t i = 0; i < max_size; i++) {
        items[i] = i;
    }
    for (size_t count = 0; count < max_size; count++) {
        tree = new BTree<int>(items, count);
        BTree<int>* tree_copy = new BTree<int>(*tree);
        for (int i = 0; i < count; i++) {
            assert(tree->Contains(items[i]));
            assert(tree_copy->Contains(items[i]));
        }
        assert(tree->GetSize() == count);
        assert(tree_copy->GetSize() == count);
        delete(tree);
        delete(tree_copy);
    }
    free(items);

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeGetAndThreads(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Get and Threading: ";

    size_t max_size = 15;
    int* items = (int*)std::malloc(max_size * sizeof(int));
    for (size_t i = 0; i < max_size; i++) {
        items[i] = i;
    }
    for (size_t count = 0; count < max_size; count++) {
        BTree<int>* tree = new BTree<int>(items, count);
        for (int i = 0; i < count; i++) {
            assert(tree->Get(i) == items[i]);
        }
        assert(tree->GetSize() == count);
        delete(tree);
    }
    free(items);

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeInsert(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Insert: ";

    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < 15; i++)
    {
        tree->Insert(i);
        assert(tree->Contains(i));
        assert(tree->GetSize() == i + 1);
    }
    delete tree;

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeRemove(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Remove: ";

    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < 15; i++)
    {
        tree->Insert(i);
        if (i % 2 == 0)
        {
            tree->Remove(i);
            assert(!tree->Contains(i));
        }
    }
    delete tree;

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeEquals(std::string prefix, bool silent)
{
    if(!silent)
        std::cout << prefix << "Equals: ";

    size_t size = 15;
    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < size; i++)
        tree->Insert(std::rand());
    BTree<int>* tree_copy = new BTree<int>(*tree);
    assert(tree->GetHeight() == tree_copy->GetHeight());
    assert(tree->GetSize() == tree_copy->GetSize());
    for (int i = 0; i < size; i++)
        assert(tree->Get(i) == tree_copy->Get(i));
    delete tree;
    delete tree_copy;

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeStringConversion(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "To/From string: ";

    size_t size = 15;
    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < size; i++)
        tree->Insert(std::rand());
    std::string format = "{N}[L](R)";
    BTree<int>* tree_copy = new BTree<int>(tree->ToString(format), format);
    assert(tree->GetHeight() == tree_copy->GetHeight());
    assert(tree->GetSize() == tree_copy->GetSize());
    for (int i = 0; i < size; i++)
        assert(tree->Get(i) == tree_copy->Get(i));
    delete tree;
    delete tree_copy;

    if (!silent)
        std::cout << "OK\n";
}

int  ProcessInt(int x)
{
    if (x % 2 == 0)
        return (x * 3) % 7;
    else
        return x * 4 - 1;
}

void TestTreeMap(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Map: ";

    size_t size = 15;
    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < size; i++)
        tree->Insert(std::rand());
    BTree<int>* tree_processed = tree->Map(ProcessInt);
    assert(tree->GetSize() == tree_processed->GetSize());
    for (int i = 0; i < size; i++)
        assert(tree_processed->Contains(ProcessInt(tree->Get(i))));
    delete tree;
    delete tree_processed;

    if (!silent)
        std::cout << "OK\n";
}

bool CheckInt(int x)
{
    return (x % 2 == 0) || (x % 3 == 0);
}

void TestTreeWhere(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Where: ";

    size_t size = 15;
    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < size; i++)
        tree->Insert(std::rand());
    BTree<int>* tree_processed = tree->Where(CheckInt);
    for (int i = 0; i < size; i++)
        if (CheckInt(tree->Get(i)))
        assert(tree_processed->Contains(tree->Get(i)));
    delete tree;
    delete tree_processed;

    if (!silent)
        std::cout << "OK\n";
}

int SumInt(int a, int b)
{
    return a + b;
}

void TestTreeReduce(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Reduce: ";

    int sum = 0;
    size_t size = 15;
    BTree<int>* tree = new BTree<int>();
    for (int i = 0; i < size; i++) {
        int x = std::rand();
        tree->Insert(x);
        sum += x;
    }
    int result = tree->Reduce(SumInt, 0);
    assert(result == sum);
    delete tree;

    if (!silent)
        std::cout << "OK\n";
}

void TestTreeAll(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "BTree tests:\n";
    prefix += "----";

    TestTreeCreation(prefix, silent);
    TestTreeGetAndThreads(prefix, silent);
    TestTreeInsert(prefix, silent);
    TestTreeRemove(prefix, silent);
    TestTreeEquals(prefix, silent);
    TestTreeStringConversion(prefix, silent);
    TestTreeMap(prefix, silent);
    TestTreeWhere(prefix, silent);
    TestTreeReduce(prefix, silent);
}

//Test all
void TestAll(std::string prefix, bool silent)
{
    if (!silent)
        std::cout << prefix << "Tests:\n";
    prefix += "----";
    TestTreeAll(prefix, silent);
}

//Tools
void RepeatedTest(void (*test_func)(std::string, bool), int n, std::string prefix, bool silent)
{
    if (!silent) {
        std::cout << prefix << "Repeated test (" << n << " times):\n";
        prefix += "----";
        std::cout << prefix << "Init test:\n";
        prefix += "----";
        test_func(prefix, false);
    }
    if (!silent)
        std::cout << prefix << "Repeated tests: ";
    for (int i = 0; i < n; i++)
        test_func("", true);
    if (!silent)
        std::cout << "OK\n";
}

void WaitKey()
{
    char c;
    std::cout << "Enter any key to continue: ";
    std::cin >> c;
}