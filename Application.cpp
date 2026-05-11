#include "Application.h"

void Application::run()
{
    BTree<std::string, char> btree1;
    btree1.setOutputMode(BTree<std::string, char>::OutputMode::HIERARCHY);

    std::string start = "a";
    for (int i = 0; i < 26; ++i)
    {
        btree1.insert({start, 'a' + i});
        start = ('a' + i + 1);

    }
//    btree1.remove(2);
//    char *value = btree1.find(1);
//    if (value)
//    {
//        *value = '0';
//    }
    std::cout << "BTree1: \n" << btree1 << "\n";
    std::cout << "Size: " << btree1.keyCount() << '\n';
}