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

#include "strutil.h"

#include <string>
#include <cctype>
#include <algorithm>
#include <locale>
#include <functional>


namespace MessagesParser
{

    template<>
    bool Str::isNumber<std::string>(std::string &data)
    {
        return false;
    }

    template<>
    bool Str::isNumber<char>(std::string &data)
    {
        return false;
    }

    void Str::trim_whitespaces(std::string &data)
    {
        data.erase( std::remove_if(data.begin(), data.end(), ::isspace), data.end());
    }


    void Str::trim_numbers(std::string &data)
    {
        data.erase( std::remove_if(data.begin(), data.end(), ::isdigit), data.end());
    }


    void Str::trim_tabs(std::string &data)
    {
        data.erase( std::remove(data.begin(), data.end(), '\t'), data.end());
    }


    void Str::trim_spaces(std::string &data)
    {
        data.erase( std::remove(data.begin(), data.end(), ' '), data.end());
    }


    void Str::trim_char(std::string &data, char c)
    {
        data.erase( std::remove(data.begin(), data.end(), c), data.end());
    }

}
