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

#ifndef _ATZ_CONFIG_
#define _ATZ_CONFIG_ 1

#include <string>
#include <vector>

#define _STR_CONC(_left, _right) _left _right
#define _CFG_BEGIN_STR          "BEGIN:"
#define _CFG_END_STR            "END"

#define _CFG_FORMATS_STR        _STR_CONC(_CFG_BEGIN_STR,"FORMATS")
#define _CFG_PARTS_STR          _STR_CONC(_CFG_BEGIN_STR,"PARTS")
#define _CFG_RULES_STR          _STR_CONC(_CFG_BEGIN_STR,"RULES")

#define _CFG_IDENTIFIER_STR     _STR_CONC(_CFG_BEGIN_STR,"IDENTIFIER")

#define _CFG_STR_SIG            "[W]"
#define _CFG_CHR_SIG            "[C]"
#define _CFG_ASCII_SIG          "[A]"


#define _CFG_STR_SPACE          "[:SPACE:]"
#define _CFG_STR_TAB            "[:TAB:]"
#define _CFG_STR_SLASH_R        "[:SLASH_R:]"
#define _CFG_STR_SLASH_N        "[:SLASH_N:]"
#define _CFG_STR_CRLF           "[:CRLF:]"
#define _CFG_STR_COMMA          "[:COMMA:]"

#define _CFG_CHR_SPACE          ' '
#define _CFG_CHR_TAB            '\t'
#define _CFG_CHR_SLASH_R        '\r'
#define _CFG_CHR_SLASH_N        '\n'
#define _CFG_CHR_CRLF           "\r\n"
#define _CFG_CHR_COMMA          ','

namespace MessagesParser
{

    namespace Config
    {

        typedef struct LogFormat
        {
            LogFormat()
            {
                str_delimiter.clear();
                expected_parts  = 0;
                based_on_part   = -1;
            }

            std::string     str_delimiter;
            unsigned short  expected_parts;
            int             based_on_part;
        } LogFormat;


        typedef std::vector<std::string>    Parts_t;
        typedef std::vector<LogFormat *>    Formats_t;


        class Config
        {

            protected:
                std::string     m_cfg;


                std::string     m_strIdentifier;
                size_t          m_identifier;

                std::vector<std::string>    m_parts_names;
                Formats_t                   m_formats;


                bool _Read_Block(std::string block, void (Config::*callback)(std::string) );

                void _Read_Identifier(std::string str);
                void _Read_Formats(std::string str);
                void _Read_Parts(std::string str);
                void _Read_Rules(std::string str);

            public:
                Config();
                ~Config();

                bool Read_Config(std::string filename);

                inline size_t Get_Identifier() { return m_identifier; }

                inline Formats_t *Get_Formats() { return &m_formats; }
                inline Parts_t *Get_Parts() { return &m_parts_names; }
        };
    }
}

#endif
