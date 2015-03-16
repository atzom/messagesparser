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

#ifndef _ATZ_PARSER_
#define _ATZ_PARSER_ 1

#include <iostream>
#include <fstream>

#include "log.h"


namespace MessagesParser
{

    using namespace MessagesParser;

    class Parser
    {

        public:
            const char LF;
            const char CR;
            const char TAB;

        protected:

            Log             m_log;
            std::ifstream   m_file;

            char            m_delimiter;

            bool            m_bRemoveTabs;
            bool            m_bRemoveCR;
            bool            m_bRemoveNumbers;


            void            (Parser::*m_pTabs)(std::string &);
            void            (Parser::*m_pCR)(std::string &);
            void            (Parser::*m_pNumbers)(std::string &);

            void            _Yes_Generic(std::string &);

            void            _No_Tabs(std::string &);
            void            _No_CR(std::string &);
            void            _No_Numbers(std::string &);


        public:
            Parser();
            ~Parser();

            bool Open(std::string filename);
            void Close();

            void ParseFile();

            Log * GetLog();

            inline void SetLogFileDelimiter(char c) { m_delimiter = c; }

            inline void KeepTabs(bool mode = true)
            {
                if (mode)
                    m_pTabs = &Parser::_Yes_Generic;
                else
                    m_pTabs = &Parser::_No_Tabs;
            }

            inline void KeepCR(bool mode = true)
            {
                if (mode)
                    m_pCR = &Parser::_Yes_Generic;
                else
                    m_pCR = &Parser::_No_CR;
            }

            inline void KeepNumbers(bool mode=true)
            {
                if (mode)
                    m_pNumbers = &Parser::_Yes_Generic;
                else
                    m_pNumbers = &Parser::_No_Numbers;
            }

            bool Config(std::string filename) { return m_log.Read_Config(filename); }

    };

}

#endif
