//
//  Exception.hpp
//  exception
//
// The MIT License (MIT)
//
// Copyright (c) 2016 André Pereira Henriques
// aphenriques (at) outlook (dot) com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef exception_Exception_hpp
#define exception_Exception_hpp

#include <stdexcept>
#include <string>
#include <type_traits>

namespace exception {
    template<typename T>
    class Exception : public T {
        static_assert(std::is_base_of<std::exception, T>::value == true, "exception::Exception<T>: typename T must be derived from std::exception");

    public:
        Exception(const std::string &fileName, unsigned lineNumber, const std::string &functionName, const std::string &errorMessage);
    };

    using RuntimeException = Exception<std::runtime_error>;
    using LogicException = Exception<std::logic_error>;

    //--

    template<typename T>
    Exception<T>::Exception(const std::string &fileName, unsigned lineNumber, const std::string &functionName, const std::string &errorMessage) : T(errorMessage + " at " + fileName + ":" + std::to_string(lineNumber) + ":(" + functionName + ")") {}
}

#endif /* exception_Exception_hpp */
