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

#ifndef _ATZ_NUMERICS_
#define _ATZ_NUMERICS_ 1

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
    struct is_unsigned
    {
        const static bool value = false;
    };

    template<>
    struct is_unsigned<unsigned>
    {
        const static bool value = true;
    };

    // short
    template<class T>
    struct is_short
    {
        const static bool value = false;
    };

    template<>
    struct is_short<short>
    {
        const static bool value = true;
    };

    template<>
    struct is_short<unsigned short>
    {
        const static bool value = true;
    };
    //////


    // long
    template<class T>
    struct is_long
    {
        const static bool value = false;
    };

    template<>
    struct is_long<long>
    {
        const static bool value = true;
    };

    template<>
    struct is_long<unsigned long>
    {
        const static bool value = true;
    };
    //////

    // int
    template<class T>
    struct is_int
    {
       const static bool value = false;
    };

    template<>
    struct is_int<int>
    {
       const static bool value = true;
    };

    template<>
    struct is_int<unsigned int>
    {
       const static bool value = true;
    };
    //////

    // float
    template<class T>
    struct is_float
    {
       const static bool value = false;
    };

    template<>
    struct is_float<float>
    {
       const static bool value = true;
    };
    //////

    // double
    template<class T>
    struct is_double
    {
       const static bool value = false;
    };

    template<>
    struct is_double<double>
    {
       const static bool value = true;
    };

    template<>
    struct is_double<long double>
    {
       const static bool value = true;
    };
    //////

    template<typename T>
    bool isShort(T)
    {
        return is_short<T>::value;
    }

    template<typename T>
    bool isLong(T)
    {
        return is_long<T>::value;
    }

    template<typename T>
    bool isInt(T)
    {
        return is_int<T>::value;
    }


    template<typename T>
    bool isFloat(T)
    {
        return is_float<T>::value;
    }

    template<typename T>
    bool isDouble(T)
    {
        return is_double<T>::value;
    }

}

#endif
