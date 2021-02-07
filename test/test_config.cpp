#include "jaysheng/config.h"
#include "jaysheng/log.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

#if 1
jaysheng::ConfigVar<int>::ptr g_int_value_config =
    jaysheng::Config::Lookup("system.port", (int)8080, "system port");

jaysheng::ConfigVar<float>::ptr g_int_valuex_config =
    jaysheng::Config::Lookup("system.port", (float)8080, "system port");

jaysheng::ConfigVar<float>::ptr g_float_value_config =
    jaysheng::Config::Lookup("system.value", (float)10.2f, "system value");

jaysheng::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config =
    jaysheng::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

jaysheng::ConfigVar<std::list<int>>::ptr g_int_list_value_config =
    jaysheng::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int list");

jaysheng::ConfigVar<std::set<int>>::ptr g_int_set_value_config =
    jaysheng::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

jaysheng::ConfigVar<std::unordered_set<int>>::ptr g_int_uset_value_config =
    jaysheng::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2}, "system int uset");

jaysheng::ConfigVar<std::map<std::string, int>>::ptr g_str_int_map_value_config =
    jaysheng::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system str int map");

jaysheng::ConfigVar<std::unordered_map<std::string, int>>::ptr g_str_int_umap_value_config =
    jaysheng::Config::Lookup("system.str_int_umap", std::unordered_map<std::string, int>{{"k", 2}}, "system str int umap");
#endif

void config() {
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "before: " << g_float_value_config->toString();

#define XX(g_var, name, prefix)                                                                     \
    {                                                                                               \
        auto v = g_var->getValue();                                                                 \
        for (auto &i : v) {                                                                         \
            JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix " " #name ": " << i;                  \
        }                                                                                           \
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
    }

#define XX_M(g_var, name, prefix)                                                                   \
    {                                                                                               \
        auto v = g_var->getValue();                                                                 \
        for (auto &i : v) {                                                                         \
            JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix " " #name ": { "                      \
                                                   << i.first << " - " << i.second << " }";         \
        }                                                                                           \
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
    }

    XX(g_int_vec_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_uset_value_config, int_uset, before);
    XX_M(g_str_int_map_value_config, int_uset, before);
    XX_M(g_str_int_umap_value_config, int_uset, before);

    YAML::Node root = YAML::LoadFile("E:/IDE/_ProgramFile/C_Program/Mystery/Mystery/config/test.yml");
    jaysheng::Config::LoadFromYaml(root);

    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "after: " << g_float_value_config->toString();

    XX(g_int_vec_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_uset_value_config, int_uset, after);
    XX_M(g_str_int_map_value_config, int_uset, after);
    XX_M(g_str_int_umap_value_config, int_uset, after);
}

void print_yaml(YAML::Node &node, int level) {
    if (node.IsScalar()) {
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << std::string(level * 4, ' ')
                                               << node.Scalar() << " - " << node.Type() << " - " << level;
    } else if (node.IsNull()) {
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << std::string(level * 4, ' ')
                                               << "NULL - " << node.Type() << " - " << level;
    } else if (node.IsMap()) {
        for (auto it = node.begin();
             it != node.end(); ++it) {
            JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << std::string(level * 4, ' ')
                                                   << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    } else if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); ++i) {
            JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << std::string(level * 4, ' ')
                                                   << i << " - " << node[i].Type() << " - " << level;
            YAML::Node tmp = node[i];
            print_yaml(tmp, level + 1);
        }
    }
}

void test_yaml() {

    YAML::Node root = YAML::LoadFile("E:/IDE/_ProgramFile/C_Program/Mystery/Mystery/config/test.yml");
    print_yaml(root, 0);
    //JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << root;
}

class Person {
public:
    Person() {}
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "[ Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           << " ]";
        return ss.str();
    }

    bool operator==(const Person &oth) const {
        return m_name == oth.m_name && m_age == oth.m_age && m_sex == oth.m_sex;
    }
};

namespace jaysheng {

template <> // std::string 转 Person
class LexicalCast<std::string, Person> {
public:
    Person operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};
template <> // Person 转 std::string
class LexicalCast<Person, std::string> {
public:
    std::string operator()(const Person &p) {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
} // namespace jaysheng

jaysheng::ConfigVar<Person>::ptr g_person =
    jaysheng::Config::Lookup("class.person", Person(), "system person");

jaysheng::ConfigVar<std::map<std::string, Person>>::ptr g_person_map =
    jaysheng::Config::Lookup("class.map", std::map<std::string, Person>(), "system person_map");

jaysheng::ConfigVar<std::map<std::string, std::vector<Person>>>::ptr g_person_vec_map =
    jaysheng::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person>>(), "system person_vec_map");

void test_class() {
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "before: " << g_person->getValue().toString() << " - " << g_person->toString();

#define XX_PM(g_var, prefix)                                                                                      \
    {                                                                                                             \
        auto m = g_person_map->getValue();                                                                        \
        for (auto &i : m) {                                                                                       \
            JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix << ": " << i.first << " - " << i.second.toString(); \
        }                                                                                                         \
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << #prefix << ": size=" << m.size();                               \
    }

    g_person->addListener(10, [](const Person &old_value, const Person &new_value) {
        JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "old_value= " << old_value.toString()
                                               << " new_value= " << new_value.toString();
    });

    XX_PM(g_person_map, "class.map before");
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "before: " << g_person_vec_map->toString();

    YAML::Node root = YAML::LoadFile("E:/IDE/_ProgramFile/C_Program/Mystery/Mystery/config/test.yml");
    jaysheng::Config::LoadFromYaml(root);

    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "after: " << g_person->getValue().toString() << " - " << g_person->toString();
    XX_PM(g_person_map, "class.map after");
    JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "after: " << g_person_vec_map->toString();
}

void test_log() {
    static jaysheng::Logger::ptr system_log = JAYSHENG_LOG_NAME("system");
    //static jaysheng::Logger::ptr root_log = JAYSHENG_LOG_ROOT();
    JAYSHENG_LOG_INFO(system_log) << "hello system1" << std::endl;
    std::cout << jaysheng::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("E:/IDE/_ProgramFile/C_Program/Mystery/Mystery/config/log.yml");
    jaysheng::Config::LoadFromYaml(root);
    std::cout << "=====================" << std::endl;
    std::cout << jaysheng::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << root << std::endl;
    JAYSHENG_LOG_INFO(system_log) << "hello system2";
    //JAYSHENG_LOG_INFO(root_log) << "hello system2";

    system_log->setFormatter("%d - %m%n");
    JAYSHENG_LOG_INFO(system_log) << "hello system2";
    //JAYSHENG_LOG_INFO(root_log) << "hello system2";

    static jaysheng::Logger::ptr root_log = JAYSHENG_LOG_ROOT();
    JAYSHENG_LOG_INFO(root_log) << "hello root" << std::endl;
    std::cout << jaysheng::LoggerMgr::GetInstance()->toYamlString() << std::endl;
}

int main(int argc, char **argv) {
    // test_yaml();
    // test_config();
    // test_class();
    // test_log();
    // jaysheng::EnvMgr::GetInstance()->init(argc, argv);
    // test_loadconf();
    // std::cout << " ==== " << std::endl;
    // sleep(10);
    // test_loadconf();
    // return 0;
    // jaysheng::Config::Visit([](jaysheng::ConfigVarBase::ptr var) {
    //     JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "name=" << var->getName()
    //                                            << " description=" << var->getDescription()
    //                                            << " typename=" << var->getTypeName()
    //                                            << " value=" << var->toString();
    // });

    return 0;
}
