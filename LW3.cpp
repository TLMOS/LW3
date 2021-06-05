#include <iostream>
#include <string>
#include "binary_tree.h"

template<class T>
T GetKey(BNode<T>* node)
{
    if (node)
        return node->key;
    else
        return T() - 1;
}

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

        std::cout << node->key << " (L: " << node->left_is_thread << " R: " << node->right_is_thread << ")" << std::endl;
        
        BNode<T>* node_l = node->left_is_thread ? NULL : node->left;
        BNode<T>* node_r = node->right_is_thread ? NULL : node->right;
        PrintTree(node_l, prefix + (isLeft ? "|  " : "   "), true);
        PrintTree(node_r, prefix + (isLeft ? "|  " : "   "), false);
    }
}

template<class T>
void PrettyPrint(BTree<T>* tree)
{
    PrintTree(tree->GetRoot());
    std::cout << "Items: ";
    for (size_t i = 0; i < tree->GetSize(); i++)
        std::cout << (*tree)[i] << " ";
    std::cout << '\n';
}

int f(int x) {
    if (x % 2 == 0)
        x *= 2;
    else
        x--;
    return x;
}

bool isEven(int x)
{
    return (x % 2 == 0);
}

int sum(int a, int b)
{
    return a + b;
}

int main()
{
    std::cout << "Init tree:\n";
    BTree<int>* tree0 = new BTree<int>();
    for (int i = 0; i < 15; i++)
        tree0->Insert(i);
    BTree<int>* tree = new BTree<int>(tree0);
    PrettyPrint(tree);
    std::cout << '\n';

    std::cout << "Fromated string [N]{R}(L): ";
    std::cout << tree->ToString("[N]{R}(L)") << "\n";
    std::cout << '\n';

    std::cout << "Remove items: ";
    for (int i = 0; i < 5; i++) {
        tree->Remove(i);
        std::cout << i << " ";
    }
    std::cout << '\n';
    PrettyPrint(tree);
    std::cout << '\n';
    
    std::cout << "Contains 3: " << tree->Contains(3) << '\n';
    std::cout << "Contains 5: " << tree->Contains(5) << '\n';
    std::cout << '\n';

    std::cout << "Map:\n";
    BTree<int>* tree2 = tree->Map(f);
    PrettyPrint(tree2);
    std::cout << '\n';

    std::cout << "Where:\n";
    BTree<int>* tree3 = tree->Where(isEven);
    PrettyPrint(tree3);
    std::cout << '\n';

    std::cout << "Sum: " << tree3->Reduce(sum, 0) << '\n';
    std::cout << '\n';

    std::cout << "Equals and sub trees:\n";
    std::cout << "Self equality: " << tree->Equals(tree) << '\n';
    std::cout << "Unequality: " << tree->Equals(tree3) << '\n';
    std::cout << "Unequality: " << tree3->Equals(tree) << '\n';
    BTree<int>* tree4 = tree->GetSubTree(11);
    std::cout << "Sub tree from 11:\n";
    PrettyPrint(tree4);
    std::cout << "Sub tree in parent tree: " << tree->Contains(tree4) << '\n';
    std::cout << '\n';

    std::cout << "From string:\n";
    std::string src = "{{}[4]({7}[34]())}[5](2)";
    std::string format = "{L}[N](R)";
    std::cout << "Source: " << src << "\n";
    std::cout << "Format: " << format << "\n";
    BTree<int>* tree6 = new BTree<int>(tree->ToString(format), format);
    std::cout << "Self equality: " << tree->Equals(tree6) << '\n';
    BTree<int>* tree5 = new BTree<int>(src, format);
    PrettyPrint(tree5);
    std::cout << '\n';

    std::cout << "ICollection:\n";
    PrintTree(tree->GetRoot());
    std::cout << "First: " << tree->GetFirst() << "\n";
    std::cout << "Last: " << tree->GetLast() << "\n";
    std::cout << "Size: " << tree->GetSize() << "\n";

    std::cout << "Items: ";
    for (size_t i = 0; i < tree->GetSize(); i++)
        std::cout << (*tree)[i] << " ";
    std::cout << "\n";
}

