#include "binary_tree.h"
#include "set.h"
#pragma once

class SimpleInterpreter
{
private:
    BTree<int>* tree_;
    Set<int>* set_;

    enum class DataType
    {
        None = 1,
        BTree,
        Set
    };
    DataType data_type_;

    struct Array
    {
        size_t length{};
        int* items{};
    };

    Array* readArray();
    void SwitchDataType(DataType data_type);
    void SetItems(int* items, size_t length);
    void PrintStatus();
    void ProcessCommand(char* command);
    void ProcessBTreeCommand(char* command);
    void ProcessSetCommand(char* command);

public:
    SimpleInterpreter(int* items = NULL, size_t length = 0,
        DataType data_type = DataType::BTree);
    ~SimpleInterpreter();
    void Start();
};