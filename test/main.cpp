#include <iostream>
#include <yaml-cpp/yaml.h>

// #include <yaml-cpp/yaml.h>

int main(int argc, char **argv) {
    std::cout << "hello my server project.\n";
    YAML::Node config = YAML::LoadFile("../config/config.yml");

    std::cout << "name:" << config["name"].as<std::string>() << std::endl;
    std::cout << "sex:" << config["sex"].as<std::string>() << std::endl;
    std::cout << "age:" << config["age"].as<int>() << std::endl;
    return 0;
}
