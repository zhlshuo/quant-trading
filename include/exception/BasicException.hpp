//
//  BasicException.hpp
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

#ifndef exception_BasicException_hpp
#define exception_BasicException_hpp

#include <cstdio>
#include <exception>
#include <string>

namespace exception {
    template<std::size_t kMessageSize>
    class BasicException : public std::exception {
    public:
        BasicException(const char *message) noexcept;
        inline BasicException(const std::string &message) noexcept;

        const char * what() const noexcept override;

    private:
        char message_[kMessageSize];
    };

    using BasicException256 = BasicException<256>;

    //--

    template<std::size_t kMessageSize>
    BasicException<kMessageSize>::BasicException(const char *message) noexcept {
        char *iterator = message_;
        const char * const iteratorEnd = message_ + kMessageSize;
        bool copyIsComplete = false;
        while (iterator != iteratorEnd) {
            *iterator = *message;
            if (*message == '\0') {
                copyIsComplete = true;
                break;
            }
            ++iterator;
            ++message;
        }
        if (copyIsComplete == false) {
            static_assert(kMessageSize > 3, "BasicException<kMessageSize>::kMessageSize must be bigger than 3");
            message_[kMessageSize - 4] = '.';
            message_[kMessageSize - 3] = '.';
            message_[kMessageSize - 2] = '.';
            message_[kMessageSize - 1] = '\0';
        }
    }

    template<std::size_t kMessageSize>
    inline BasicException<kMessageSize>::BasicException(const std::string &message) noexcept : BasicException(message.c_str()) {}

    template<std::size_t kMessageSize>
    const char * BasicException<kMessageSize>::what() const noexcept {
        return message_;
    }
}

#endif /* exception_BasicException_hpp */

