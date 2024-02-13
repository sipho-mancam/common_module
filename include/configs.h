#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

template<typename T>
T convertFromString(const std::string& str) {
    T result;
    std::istringstream iss(str);
    iss >> std::boolalpha >> result;  // Use std::boolalpha to properly handle bool values
    return result;
}

static std::vector<std::string> splitString(const std::string& s)
{
    std::istringstream stream(s);
    std::string token;
    std::vector<std::string> result;
    while (std::getline(stream, token, ' '))
    {
        result.push_back(token);
    }
    return result;
}


class ConfigReader {
public:
    ConfigReader(const std::string& filename)
        :context_set(false)

    {
        boost::property_tree::ini_parser::read_ini(filename, pt_);
    }
    
    template<class T>
    T getValue(const std::string& key)
    {   
        try {
            return context_group.get_child(key).get_value<T>();
        }
        catch (boost::exception& e) {
            std::cerr << "Key doesn't exist in the file" << std::endl;
            return "";
        }
    }


    template <class T2>
    std::vector<T2> getList(const std::string& key)
    {
        // this should read a space separated string, split it and convert it to type and return a list of that type.
        try {
            std::string output = context_group.get_child(key).get_value<std::string>();
            std::vector<std::string> res = splitString(output);
            std::vector<T2> output_list;
            for (auto& ele : res)
            {
                output_list.push_back(convertFromString<T2>(ele));
            }
            return output_list;
            
        }catch (boost::exception& e) {
            std::cerr << "Key doesn't exist in the file" << std::endl;
            return std::vector<T2>();
        }
    }

    void setGroupContext(const std::string& group)
    {
        try {
            context_group = pt_.get_child(group);
        }catch (boost::exception& e) {
            std::cerr << "[Error]: Group does not exist" << std::endl;
        }
    }


    boost::property_tree::ptree& getConfig() { return pt_;  }

private:
    bool context_set;
    boost::property_tree::ptree pt_;
    boost::property_tree::ptree context_group;
};


class ConfigWriter {
public:
    ConfigWriter():context_set(false){}
    ConfigWriter(const std::string& filename): context_set(false),outfile(filename){}
    ConfigWriter(boost::property_tree::ptree& t)
        : 
        pt_(t), context_set(false)
    {}

    void setConfig(boost::property_tree::ptree& t) { pt_ = t; }
    
    void addGroup(const std::string& groupName) {
        currentGroup_ = groupName;
        current_context = groupName;
        pt_.add_child(groupName, boost::property_tree::ptree());
    }

    void addComment(const std::string& comment) {
        // Add a comment using a special key
        //pt_.put_value("#" + comment);
        pt_.put(current_context + ".;" + comment, "");
    }

    void setContext(const std::string& contextName) {
        current_context = contextName;
        context_set = true;
    }

    void write(const std::string& key, const std::string& value) {
        if (!context_set)
            throw std::exception("Context not set");

        boost::property_tree::ptree& ctx = pt_.get_child(current_context);
        ctx.add_child(key, boost::property_tree::ptree(value));
    }

    template <class T>
    void writeList(const std::string& key, std::vector<T> l1)
    {
        if (!context_set)
            throw std::exception("Context not set");
        // step one, convert all the data to a long string
        // then call the write method
        std::string output_str;
        for (T& elem : l1)
        {
            output_str += convertToString(elem)+" ";
        }
        output_str.pop_back();
        write(key, output_str);
    }

    void saveToFile(const std::string& filename) const {
        boost::property_tree::write_ini(filename, pt_);
    }
    void saveToFile()
    {
        if (!outfile.empty()) {
            boost::property_tree::write_ini(outfile, pt_);
        }
    }

private:
    std::string currentGroup_;
    std::string current_context;
    bool context_set;
    boost::property_tree::ptree pt_;
    std::string outfile;

    template <typename T>
    std::string convertToString(const T& value) {
        static_assert(std::is_copy_constructible<T>::value, "Type must be copy-constructible");

        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
};


class ConfigParser {
private:
    ConfigReader reader;
    ConfigWriter writer;
 
public:

    ConfigParser(const std::string& filename);
    ConfigReader& getReader();
    ConfigWriter& getWriter();
};