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

#include <algorithm>
#include "parser.h"
#include "strutil.h"


namespace MessagesParser
{

    Parser::Parser(): LF('\n'), CR('\r'), TAB('\t')
    {
        m_delimiter = LF;

        KeepTabs();
        KeepCR();
        KeepNumbers();
    }


    Parser::~Parser()
    {
        Close();
    }


    bool Parser::Open(std::string filename)
    {
        if (filename.length() == 0)
            return false;

        m_file.open(filename.c_str(), std::ifstream::in);

        if ( ! m_file.is_open())
            return false;

        return true;
    }


    void Parser::Close()
    {
        if (m_file.is_open())
            m_file.close();
    }


    void Parser::_Yes_Generic(std::string &)
    {
        // Do nothing since no rules are applied
    }

    void Parser::_No_Tabs(std::string &data)
    {
        Str::trim_tabs(data);
    }

    void Parser::_No_CR(std::string &data)
    {
        Str::trim_char(data, '\r');
    }

    void Parser::_No_Numbers(std::string &data)
    {
        Str::trim_numbers(data);
    }



    // For optimization purposes , separate this into 3 functions, in order to avoid putting the if..statement inside the while loop
    void Parser::ParseFile()
    {
        std::string line;

        while ( ( ! m_file.eof() ) && (m_file.good()) )
        {
            line.clear();

            std::getline(m_file, line, m_delimiter);

            (this->*m_pTabs)(line);
            (this->*m_pCR)(line);
            (this->*m_pNumbers)(line);

            if (line.length() > 0)
                m_log.Add(line);
        }
    }


    Log * Parser::GetLog()
    {
        return &m_log;
    }

}
