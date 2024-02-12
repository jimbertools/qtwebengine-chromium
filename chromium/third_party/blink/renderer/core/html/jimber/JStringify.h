#ifndef JXSTRINGIFY_H
#define JXSTRINGIFY_H

// #include "JxFunctionTypes.h"
// #include "../jimberbroker/JMediaPlayerHandler.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "include/core/SkColor.h"

typedef std::shared_ptr<std::vector<std::string>> vecStrPtr;
namespace Jimber {
    class JStringify {
    private:
        vecStrPtr _values;
        bool _newLineAdded = false;

    public:
        JStringify()
            : _values(std::make_shared<std::vector<std::string>>(std::vector<std::string>()))
        {
        }

        JStringify(const JStringify& other)
        {
            this->_values = other._values;
        }

        template <class T>
        JStringify operator<<(T var)
        {
            _values->push_back(std::to_string(var));
            return *this;
        }

        JStringify operator<<(std::string var)
        {
            _values->push_back(var);

            return *this;
        }

        JStringify operator<<(const char* var)
        {
            _values->push_back(std::string(var));
            return *this;
        }

        JStringify operator<<(char* var)
        {
            _values->push_back(std::string(var));
            return *this;
        }
        

        std::string toString()
        {
            std::stringstream ss;
            for (auto value : *_values) {
                ss << value;

                if (value[value.size() - 1] != '\n')
                    ss << ";";
            }
            return ss.str();
        }
        ~JStringify()
        {
        }
    };
}
#endif