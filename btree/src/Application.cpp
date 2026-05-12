#include <BTREE/Application.h>

void Application::run()
{
    BTree<std::string, char> btree1;
    btree1.setOutputMode(BTree<std::string, char>::OutputMode::SIDEWAYS);

    std::string start = "a";
    for (int i = 0; i < 26; ++i)
    {
        btree1.insert({start, 'a' + i});
        start = ('a' + i + 1);

    }

    std::cout << "BTree1: \n" << btree1 << "\n";
    std::cout << "Size: " << btree1.keyCount() << '\n';

    BTree<int, int, 4> btree2;
    btree2.setOutputMode(BTree<int, int, 4>::OutputMode::HIERARCHY);

    btree2.insert({25, 25});
    btree2.insert({36, 36});
    btree2.insert({28, 28});
    btree2.insert({33, 33});
    btree2.insert({48, 48});
    btree2.insert({44, 44});
    btree2.insert({38, 38});
    btree2.insert({39, 39});
    btree2.insert({42, 42});
    btree2.insert({10, 10});
    btree2.insert({45, 45});
    btree2.insert({46, 46});
    btree2.insert({9, 9});
    btree2.insert({15, 15});
    btree2.insert({20, 20});
    btree2.insert({18, 18});
    btree2.insert({40, 40});
    btree2.insert({16, 16});
    btree2.insert({21, 21});
    btree2.insert({22, 22});
    btree2.insert({35, 34});
    btree2.insert({12, 12});
    btree2.insert({14, 14});
    btree2.insert({24, 24});
    btree2.insert({26, 26});
    btree2.insert({27, 27});
    btree2.insert({30, 30});
    btree2.insert({32, 32});

    // TODO Potential bug: Removing parent causes a bug where key is removed, but value stays with child key instead.
    btree2.remove(44);

    std::cout << "BTree2: " << btree2 << '\n';
    std::cout << "Key Count: " << btree2.keyCount() << '\n';
}