/*
 *  Copyright (c) 2015, Andreas Tzomakas
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of messagesparser nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ATZ_STRUTIL_
#define _ATZ_STRUTIL_ 1

#include <string>
#include <sstream>

namespace MessagesParser
{

    template<class T>
    struct is_pointer
    {
        static const bool value = false;
    };

    template<class T>
    struct is_pointer<T *>
    {
        static const bool value = true;
    };

    template<class T>
    struct is_int
    {
        static const bool value = false;
    };

    template<>
    struct is_int<int>
    {
        static const bool value = true;
    };

    template<class T>
    struct is_uint
    {
        static const bool value = false;
    };

    template<>
    struct is_uint<unsigned int>
    {
        static const bool value = true;
    };


    class Str
    {

        public:

            Str() {}
            ~Str() {}

            template<class T>
            static bool str2number(std::string &data, T &number)
            {
                std::stringstream _ss(data);
                T _var;

                _ss >> _var;

                if (_ss.fail())
                    return false;
                else
                    number = _var;

                return true;
            }


            template<class T>
            static bool number2str(T number, std::string &data)
            {
                std::stringstream _ss(data);
                T _var;

                _ss << number;

                if (_ss.fail())
                    return false;
                else
                    data.assign(_ss.str());

                return true;
            }


            template<class T>
            static bool isNumber(std::string &data)
            {
                T _var;

                if (is_pointer<T>::value)
                    return false;

                return str2number(data, _var);
            }

            static void trim_whitespaces(std::string &data);
            static void trim_numbers(std::string &data);
            static void trim_tabs(std::string &data);
            static void trim_spaces(std::string &data);
            static void trim_char(std::string &data, char c);

            static bool starts_with(const std::string &str, std::string search, bool case_sensitive = false);
            static bool contains(const std::string &str, std::string search, bool case_sensitive = false);
            static bool ends_with(const std::string &str, std::string search, bool case_sensitive = false);

    };

}


#endif

