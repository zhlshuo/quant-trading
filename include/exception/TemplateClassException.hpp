//
//  TemplateClassException.hpp
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

#ifndef exception_TemplateClassException_hpp
#define exception_TemplateClassException_hpp

#include <stdexcept>
#include <string>
#include <type_traits>

namespace exception {
    template<template<typename ...> class T, typename E>
    class TemplateClassException : public E {
        static_assert(std::is_base_of<std::exception, E>::value == true, "exception::TemplateClassException<T, E>: typename E must be derived from std::exception");

    public:
        template<typename ...A>
        inline TemplateClassException(A &&...arguments);
    };

    //--

    template<template<typename ...> class T, typename E>
    template<typename ...A>
    inline TemplateClassException<T, E>::TemplateClassException(A &&...arguments) : E(std::forward<A>(arguments)...) {}
}

#endif /* exception_TemplateClassException_hpp */
