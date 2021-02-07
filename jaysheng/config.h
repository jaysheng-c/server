
/************************************
@brief:			配置系统
@file:			config.h
@version:
@company:
@author: 		jaysheng
@email:
@create_time:	2020-05-26 22:20
@update_time:	2020-05-29 15:25

2020-05-28 13:29 : 添加 class LexicalCast 作为基础转化，修改class ConfigVar模板参数 特例化实现方式参数转化，使其支持复杂类型转化
				   容器“片特化”，支持vector
2020-05-28 21:10 : 支持复杂类型转化(list, set, unordered_set, map(std::string, *), unordered_map(std::string, *))

2020-05-29 15:25 : 添加配置的事件机制，当一个配置项发生修改时，能够反向的通知对应的代码(回调)

************************************/

// 配置系统原则，约定优于配置

// 自定义类型需要实现jaysheng::LexicalCast的片特化，实现后即可支持jaysheng::Config解析自定义类型，
// 自定义类型可以和常规STL容器一起使用

#ifndef __JAYSHENG_CONFIG_H__
#define __JAYSHENG_CONFIG_H__

#include "log.h"
#include <boost/lexical_cast.hpp>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace jaysheng {

// 基类
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string &name, const std::string &description = "")
        : m_name(name),
          m_description(description) {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower); // 将首字母转成小写
    }
    virtual ~ConfigVarBase() {}

    const std::string &getName() const { return m_name; }
    const std::string &getDescription() const { return m_description; }

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string &val) = 0;
    virtual std::string getTypeName() const = 0;

protected:
    std::string m_name;
    std::string m_description;
};

// F : from_type  T : to_type
template <class F, class T> // 基础转化
class LexicalCast {
public:
    T operator()(const F &v) {
        return boost::lexical_cast<T>(v);
    }
};

template <class T> // std::string 转 std::vector
class LexicalCast<std::string, std::vector<T>> {
public:
    std::vector<T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};
template <class T> // std::vector 转 std::string
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> // std::string 转 std::list
class LexicalCast<std::string, std::list<T>> {
public:
    std::list<T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};
template <class T> // std::list 转 std::string
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> // std::string 转 std::set
class LexicalCast<std::string, std::set<T>> {
public:
    std::set<T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};
template <class T> // std::set 转 std::string
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> // std::string 转 std::unordered_set
class LexicalCast<std::string, std::unordered_set<T>> {
public:
    std::unordered_set<T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};
template <class T> // std::unordered_set 转 std::string
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator()(const std::unordered_set<T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> // std::string 转 std::map
class LexicalCast<std::string, std::map<std::string, T>> {
public:
    std::map<std::string, T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin();
             it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};
template <class T> // std::map 转 std::string
class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator()(const std::map<std::string, T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T> // std::string 转 std::unordered_map
class LexicalCast<std::string, std::unordered_map<std::string, T>> {
public:
    std::unordered_map<std::string, T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin();
             it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};
template <class T> // std::unordered_map 转 std::string
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, T> &v) {
        YAML::Node node;
        for (auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

// FromStr  T operator()(const std::string&)
// ToStr std::string operator()(const T&)
template <class T, class FromStr = LexicalCast<std::string, T>,
          class ToStr = LexicalCast<T, std::string>> // 特例化
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void(const T &old_value, const T &new_value)> on_change_cb; // 回调函数 cb : call_back

    ConfigVar(const std::string &name, const T &default_value, const std::string &description = "")
        : ConfigVarBase(name, description),
          m_val(default_value) {
    }
    //~MyClass();

    std::string toString() override {
        try {
            //return boost::lexical_cast<std::string>(m_val);
            return ToStr()(m_val);
        } catch (std::exception &e) {
            JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "ConfigVar::toString exception"
                                                    << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    bool fromString(const std::string &val) override {
        try {
            //m_val = boost::lexical_cast<T>(val);
            setValue(FromStr()(val));
        } catch (std::exception &e) {
            JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "ConfigVar::toString exception"
                                                    << e.what() << " convert: string to" << typeid(m_val).name();
        }
        return false;
    }

    const T getValue() const { return m_val; }

    //void setValue(const T& v) { m_val = v; }
    void setValue(const T &v) {
        if (v == m_val) {
            return;
        }
        for (auto &i : m_cbs) {
            i.second(m_val, v);
        }
        m_val = v;
    }

    std::string getTypeName() const override { return typeid(T).name(); }

    void addListener(uint64_t key, on_change_cb cb) {
        m_cbs[key] = cb;
    }

    void delListener(uint64_t key) {
        m_cbs.erase(key);
    }

    on_change_cb getListener(uint64_t key) {
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    void clearListen() {
        m_cbs.clear();
    }

private:
    T m_val;
    /*
			名称: m_cbs
			功能: 变更回调函数组，functional没有比较的函数，所以要使用map中的key来删除functional
			参数: uint64_t key, 要求唯一，一般使用hash
		*/
    std::map<uint64_t, on_change_cb> m_cbs;
};

class Config {
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name,
                                             const T &default_value, const std::string &description = "") {
        auto it = GetDatas().find(name);
        if (it != GetDatas().end()) {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
            if (tmp) {
                JAYSHENG_LOG_INFO(JAYSHENG_LOG_ROOT()) << "Lookup name=" << name << " exists";
                return tmp;
            } else {
                JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                                                        << typeid(T).name() << " real_type=" << it->second->getTypeName()
                                                        << " " << it->second->toString();
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789") != std::string::npos) {
            JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        GetDatas()[name] = v;
        return v;
    }

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
        auto it = GetDatas().find(name);
        if (it == GetDatas().end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    static void LoadFromYaml(const YAML::Node &root);
    static ConfigVarBase::ptr LookupBase(const std::string &name);

private:
    // 防止static静态成员变量初始化顺序问题，在使用s_datas时，该变量未被初始化
    //static ConfigVarMap s_datas;
    static ConfigVarMap &GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }
};

} // namespace jaysheng

#endif // !__JAYSHENG_CONFIG_H__
