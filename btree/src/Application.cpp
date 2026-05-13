#include <BTREE/Application.h>

void Application::run()
{
    BTree<int, std::string, 3> btree2;
    btree2.setOutputMode(BTree<int, std::string, 3>::OutputMode::HIERARCHY);

    btree2.insert({1,"staying"});
    btree2.insert({2,"deleted"});
    btree2.insert({3,"filler"});
    btree2.insert({4,"filler"});
    btree2.insert({5,"filler"});
    btree2.insert({6,"filler"});
    btree2.insert({7,"filler"});
    btree2.insert({8,"filler"});
    btree2.insert({9,"filler"});
    btree2.insert({10,"filler"});
    
    btree2.remove(2);
    
    std::cout << "BTree2: " << btree2 << '\n';
    std::cout << "Key Count: " << btree2.keyCount() << '\n';
}