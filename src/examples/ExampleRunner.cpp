#ifdef BUILD_EXAMPLE_EXE
#include <iostream>
#include <fstream>
#include <string>

int main_example(int argc, char** argv)
{
    std::cout << "ExampleRunner: loading examples/scene1/scene.json\n";
    std::ifstream ifs("examples/scene1/scene.json");
    if (!ifs) { std::cerr << "Cannot open scene file\n"; return 1; }
    std::string s((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    std::cout << s << "\n";
    return 0;
}

int main(int argc, char** argv)
{
    return main_example(argc, argv);
}

#endif // BUILD_EXAMPLE_EXE
