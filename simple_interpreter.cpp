#include "simple_interpreter.h"
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "binary_tree.h"
#include "set.h"
#include "tests.h"

//Example functions
int Even(int x)
{
    return (int)(x % 2 == 0);
}

int Increment(int x)
{
    return x + 1;
}

bool IsEven(int x)
{
    return x % 2 == 0;
}

bool IsDigit(int x)
{
    return (x >= 0) && (x < 10);
}

int Sum(int a, int b)
{
    return a + b;
}

int Prod(int a, int b)
{
    return a * b;
}

//Help printers
void PrintHelp() {
    printf("Global commands:\n");
    printf("  Quit\n");
    printf("  = | items\n");
    printf("  switch | (BTree, Set)\n");
}

void PrintHelpBTree() //TODO
{
    PrintHelp();
    printf("BTree commands:\n");
    printf("  GetHeight\n");
    printf("  FromString | format tree\n");
    printf("  Insert | item\n");
    printf("  Remove | item\n");
    printf("  Contains | item\n");
    printf("  ContainsSubTree | format tree\n");
    printf("  Equals | format tree\n");
    printf("  GetSubTree | item\n");
    printf("  Map | (Even, Increment)\n");
    printf("  Where | (Even, Digit)\n");
    printf("  Reduce | (Sum, Prod)\n");
    printf("  ToString | format\n");
    printf("  GetFirst\n");
    printf("  GetLast\n");
    printf("  Get | index\n");
    printf("  GetSize\n");
}

void PrintHelpSet() //TODO
{
    PrintHelp();
    printf("Set commands:\n");
    printf("  FromString | set\n");
    printf("  Add | item\n");
    printf("  Remove | item\n");
    printf("  Contains | item\n");
    printf("  ContainsSubSet | set\n");
    printf("  Equals | set\n");
    printf("  Union | set\n");
    printf("  Intersection | set\n");
    printf("  Subtraction | set\n");
    printf("  Map | (Even, Increment)\n");
    printf("  Where | (Even, Digit)\n");
    printf("  Reduce | (Sum, Prod)\n");
    printf("  ToString\n");
    printf("  GetFirst\n");
    printf("  GetLast\n");
    printf("  Get | index\n");
    printf("  GetSize\n");
}

//Data printers
template<class T>
void PrintTree(const BNode<T>* node, const std::string& prefix = "", bool isLeft = false)
{
    if (node)
    {
        std::cout << prefix;
        if (isLeft)
            std::cout << char(195) << char(196) << char(196);
        else
            std::cout << char(192) << char(196) << char(196);

        std::cout << node->key << std::endl;

        BNode<T>* node_l = node->left_is_thread ? NULL : node->left;
        BNode<T>* node_r = node->right_is_thread ? NULL : node->right;
        PrintTree(node_l, prefix + (isLeft ? "|  " : "   "), true);
        PrintTree(node_r, prefix + (isLeft ? "|  " : "   "), false);
    }
}

//Class methods
SimpleInterpreter::Array* SimpleInterpreter::readArray()
{
    std::vector<int> items;
    std::string src;
    std::getline(std::cin, src);

    src = src.substr(2, src.length() - 3);
    size_t comma_index = src.find(",");
    while (comma_index != std::string::npos)
    {
        int item = std::stoi(src.substr(0, comma_index));
        items.push_back(item);
        src = src.substr(comma_index + 2, src.length() - comma_index - 2);
        comma_index = src.find(",");
    }
    if (src.length() > 0)
    {
        int item = TFromString(src.substr(0, comma_index));
        items.push_back(item);
    }

    Array* array = new Array();
    array->length = items.size();
    array->items = (int*)std::malloc(items.size() * sizeof(int));
    std::copy(items.begin(), items.end(), array->items);
    items.clear();

    return array;
}

void SimpleInterpreter::SwitchDataType(DataType data_type)
{
    int* items;
    size_t length = 0;
    switch (data_type_)
    {
    case DataType::BTree:
        length = tree_->GetSize();
        items = (int*)std::malloc(length * sizeof(int));
        for (int i = 0; i < length; i++)
            items[i] = tree_->Get(i);
        delete(tree_);
        break;
    case DataType::Set:
        length = set_->GetSize();
        items = (int*)std::malloc(length * sizeof(int));
        for (int i = 0; i < length; i++)
            items[i] = set_->Get(i);
        delete(set_);
        break;
    default:
        throw std::invalid_argument("Got wrong sequence type.");
        break;
    }

    switch (data_type)
    {
    case DataType::BTree:
        tree_ = new BTree<int>(items, length);
        break;
    case DataType::Set:
        set_ = new Set<int>(items, length);
        break;
    default:
        throw std::invalid_argument("Got wrong data type.");
        break;
    }

    data_type_ = data_type;
}

void SimpleInterpreter::SetItems(int* items, size_t length)
{
    switch (data_type_)
    {
    case DataType::BTree:
        delete(tree_);
        tree_ = new BTree<int>(items, length);
        break;
    case DataType::Set:
        delete(set_);
        set_ = new Set<int>(items, length);
        break;
    default:
        throw std::invalid_argument("Got wrong sequence type.");
        break;
    }
}

void SimpleInterpreter::PrintStatus()
{
    size_t length;
    switch (data_type_)
    {
    case DataType::BTree:
        printf("BTree:\n");
        PrintTree(tree_->GetRoot());
        std::cout << "Items: ";
        for (size_t i = 0; i < tree_->GetSize(); i++)
            std::cout << (*tree_)[i] << " ";
        break;
    case DataType::Set:
        printf("Set: ");
        std::cout << set_->ToString();
        break;
    default:
        throw std::invalid_argument("Got wrong sequence type.");
        break;
    }
    printf("\n");
}

void SimpleInterpreter::ProcessCommand(char* command)
{
    if (!std::strcmp(command, "=")) {
        Array* array = readArray();
        SetItems(array->items, array->length);
        free(array->items);
        PrintStatus();
    }
    else if (!strcmp(command, "test")) {
        int n_tests = 0;
        scanf_s(" %d", &n_tests);
        if (n_tests >= 0) {
            if (n_tests == 1)
                TestAll("", false);
            else
                RepeatedTest(TestAll, n_tests, "", false);
        }
        else
            printf("Error:  Invalid arguments.\n");
    }
    else if (!strcmp(command, "Switch")) {
        char data_type_string[32];
        scanf_s("%s", data_type_string, 32);
        data_type_string[31] = 0;
        if (!strcmp(data_type_string, "BTree"))
            SwitchDataType(DataType::BTree);
        else if (!strcmp(data_type_string, "Set"))
            SwitchDataType(DataType::Set);
        else
            printf("Error: Unknown data type.");
        PrintStatus();
    }
    else {
        switch (data_type_)
        {
        case DataType::BTree:
            ProcessBTreeCommand(command);
            break;
        case DataType::Set:
            ProcessSetCommand(command);
            break;
        }
    }
}

void SimpleInterpreter::ProcessBTreeCommand(char* command)
{
    if (!std::strcmp(command, "GetHeight")) {
        printf("Output: %d\n", (int)tree_->GetHeight());
    }
    else if (!std::strcmp(command, "FromString")) {
        std::string format;
        std::string src;
        std::getline(std::cin, format, ' '); //Get rid off space
        std::getline(std::cin, format, ' ');
        std::getline(std::cin, src);
        delete tree_;
        tree_ = new BTree<int>(src, format);
        PrintStatus();
    }
    else if (!std::strcmp(command, "Insert")) {
        int item;
        scanf_s(" %d", &item);
        tree_->Insert(item);
        PrintStatus();
    }
    else if (!std::strcmp(command, "Remove")) {
        int item;
        scanf_s(" %d", &item);
        if (!tree_->Contains(item)) {
            printf("Error: Item not found.\n");
            return;
        }
        tree_->Remove(item);
        PrintStatus();
    }
    else if (!std::strcmp(command, "Contains")) {
        int item;
        scanf_s(" %d", &item);
        printf("Output: ");
        if(tree_->Contains(item))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "ContainsSubTree")) {
        std::string format;
        std::string src;
        std::getline(std::cin, format, ' '); //Get rid off space
        std::getline(std::cin, format, ' ');
        std::getline(std::cin, src);
        BTree<int>* tree = new BTree<int>(src, format);

        printf("Output: ");
        if (tree_->Contains(tree))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "Equals")) {
        std::string format;
        std::string src;
        std::getline(std::cin, format, ' '); //Get rid off space
        std::getline(std::cin, format, ' ');
        std::getline(std::cin, src);
        BTree<int>* tree = new BTree<int>(src, format);
        printf("Output: ");
        if (tree_->Equals(tree))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "GetSubTree")) {
        int item;
        scanf_s(" %d", &item);
        if (!tree_->Contains(item)) {
            printf("Error: Item not found.\n");
            return;
        }
        BTree<int>* tree = tree_->GetSubTree(item);
        printf("Output:\n");
        delete tree_;
        tree_ = tree;
        PrintStatus();
    }
    else if (!std::strcmp(command, "Map")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        BTree<int>* tree = NULL;
        if (!std::strcmp(f_name, "Even"))
            tree = tree_->Map(Even);
        else if (!std::strcmp(f_name, "Increment"))
            tree = tree_->Map(Increment);
        else
            printf("Error: Unknown function. Should be Even or Increment\n");
        if (tree) {
            delete(tree_);
            tree_ = tree;
        }
        PrintStatus();
    }
    else if (!std::strcmp(command, "Where")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        BTree<int>* tree = NULL;
        if (!std::strcmp(f_name, "Even"))
            tree = tree_->Where(IsEven);
        else if (!std::strcmp(f_name, "Digit"))
            tree = tree_->Where(IsDigit);
        else
            printf("Error: Unknown function. Should be Even or Digit\n");
        if (tree) {
            delete(tree_);
            tree_ = tree;
        }
        PrintStatus();
    }
    else if (!std::strcmp(command, "Reduce")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        int result = 0;
        if (!std::strcmp(f_name, "Sum"))
            result = tree_->Reduce(Sum, 0);
        else if (!std::strcmp(f_name, "Prod"))
            result = tree_->Reduce(Prod, 1);
        else
            printf("Error: Unknown function. Should be Sum or Prod\n");
        if (result)
            printf("Output: %d\n", result);
    }
    else if (!std::strcmp(command, "ToString")) {
        std::string format;
        std::getline(std::cin, format, ' '); //Get rid off space
        std::getline(std::cin, format, '\n');
        printf("Output: ");
        std::cout << tree_->ToString(format) << '\n';
    }
    else if (!std::strcmp(command, "GetFirst")) {
        printf("Output: %d\n", tree_->GetFirst());
    }
    else if (!std::strcmp(command, "GetLast")) {
        printf("Output: %d\n", tree_->GetLast());
    }
    else if (!std::strcmp(command, "Get")) {
        int index;
        scanf_s(" %d", &index);
        if (index < 0 || index >= tree_->GetSize()) {
            printf("Error: Index out of range.\n");
            return;
        }
        printf("Output: %d\n", tree_->Get(index));
    }
    else if (!std::strcmp(command, "GetSize")) {
    printf("Output: %d\n", (int)tree_->GetSize());
    }
    else if (!std::strcmp(command, "Help")) {
    PrintHelpBTree();
    }
    else
        printf("Error: Unknown command. Try help for a list of commands.\n");
}

void SimpleInterpreter::ProcessSetCommand(char* command)
{
    if (!std::strcmp(command, "FromString")) {
        Array* arr = readArray();
        delete set_;
        set_ = new Set<int>(arr->items, arr->length);
        free(arr->items);
        delete arr;
        PrintStatus();
    }
    else if (!std::strcmp(command, "Add")) {
        int item;
        scanf_s(" %d", &item);
        set_->Add(item);
        PrintStatus();
    }
    else if (!std::strcmp(command, "Remove")) {
        int item;
        scanf_s(" %d", &item);
        if (!set_->Contains(item)) {
            printf("Error: Item not found.\n");
            return;
        }
        set_->Remove(item);
        PrintStatus();
    }
    else if (!std::strcmp(command, "Contains")) {
        int item;
        scanf_s(" %d", &item);
        printf("Output: ");
        if (set_->Contains(item))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "ContainsSubSet")) {
        Array* arr = readArray();
        Set<int>* set = new Set<int>(arr->items, arr->length);
        free(arr->items);
        delete arr;

        printf("Output: ");
        if (set_->Contains(set))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "Union")) {
        Array* arr = readArray();
        Set<int>* set = new Set<int>(arr->items, arr->length);
        Set<int>* tmp = set_->Union(set);
        delete set_;
        set_ = tmp;
        free(arr->items);
        delete arr;
        PrintStatus();
    }
    else if (!std::strcmp(command, "Intersection")) {
        Array* arr = readArray();
        Set<int>* set = new Set<int>(arr->items, arr->length);
        Set<int>* tmp = set_->Intersection(set);
        delete set_;
        set_ = tmp;
        free(arr->items);
        delete arr;
        PrintStatus();
    }
    else if (!std::strcmp(command, "Subtraction")) {
        Array* arr = readArray();
        Set<int>* set = new Set<int>(arr->items, arr->length);
        Set<int>* tmp = set_->Subtraction(set);
        delete set_;
        set_ = tmp;
        free(arr->items);
        delete arr;
        PrintStatus();
    }
    else if (!std::strcmp(command, "Equals")) {
        Array* arr = readArray();
        Set<int>* set = new Set<int>(arr->items, arr->length);
        free(arr->items);
        delete arr;
        printf("Output: ");
        if (set_->Equals(set))
            printf("YES.\n");
        else
            printf("NO.\n");
    }
    else if (!std::strcmp(command, "Map")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        Set<int>* set = NULL;
        if (!std::strcmp(f_name, "Even"))
            set = set_->Map(Even);
        else if (!std::strcmp(f_name, "Increment"))
            set = set_->Map(Increment);
        else
            printf("Error: Unknown function. Should be Even or Increment\n");
        if (set) {
            delete(set_);
            set_ = set;
        }
        PrintStatus();
    }
    else if (!std::strcmp(command, "Where")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        Set<int>* set = NULL;
        if (!std::strcmp(f_name, "Even"))
            set = set_->Where(IsEven);
        else if (!std::strcmp(f_name, "Digit"))
            set = set_->Where(IsDigit);
        else
            printf("Error: Unknown function. Should be Even or Digit\n");
        if (set) {
            delete(set_);
            set_ = set;
        }
        PrintStatus();
    }
    else if (!std::strcmp(command, "Reduce")) {
        char f_name[16];
        scanf_s("%s", f_name, 16);
        f_name[15] = 0;

        int result = 0;
        if (!std::strcmp(f_name, "Sum"))
            result = set_->Reduce(Sum, 0);
        else if (!std::strcmp(f_name, "Prod"))
            result = set_->Reduce(Prod, 1);
        else
            printf("Error: Unknown function. Should be Sum or Prod\n");
        if (result)
            printf("Output: %d\n", result);
    }
    else if (!std::strcmp(command, "ToString")) {
        printf("Output: ");
        std::cout << set_->ToString() << '\n';
    }
    else if (!std::strcmp(command, "GetFirst")) {
        printf("Output: %d\n", set_->GetFirst());
    }
    else if (!std::strcmp(command, "GetLast")) {
        printf("Output: %d\n", set_->GetLast());
    }
    else if (!std::strcmp(command, "Get")) {
        int index;
        scanf_s(" %d", &index);
        if (index < 0 || index >= set_->GetSize()) {
            printf("Error: Index out of range.\n");
            return;
        }
        printf("Output: %d\n", set_->Get(index));
    }
    else if (!std::strcmp(command, "GetSize")) {
        printf("Output: %d\n", (int)set_->GetSize());
    }
    else if (!std::strcmp(command, "Help")) {
        PrintHelpBTree();
    }
    else
        printf("Error: Unknown command. Try help for a list of commands.\n");
}

SimpleInterpreter::SimpleInterpreter(int* items, size_t length, DataType data_type)
{
    data_type_ = DataType::BTree;
    tree_ = new BTree<int>(items, length);
    SwitchDataType(data_type);
}

SimpleInterpreter::~SimpleInterpreter()
{
    switch (data_type_)
    {
    case DataType::BTree:
        delete(tree_);
        break;
    case DataType::Set:
        delete(set_);
        break;
    }
}

void SimpleInterpreter::Start()
{
    while (true) {
        char command[32];
        printf("(User): ");
        scanf_s("%s", command, 32);
        command[31] = 0;

        if (!strcmp(command, "Quit"))
            break;

        ProcessCommand(command);
    }
}
