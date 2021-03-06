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

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <ctime>

#include "text.h"
#include "strutil.h"

namespace MessagesParser
{

    namespace Output
    {

        Text::Text(): Output()
        {
            ShowRawData();      // set the default values

            m_stream = &m_fstream;
        }

        Text::~Text()
        {
        }

        void Text::_RawTextStart()
        {
            (*m_stream) << "+" << Msg::TagRawData << std::endl;
        }

        void Text::_RawTextEnd()
        {
            (*m_stream) << std::endl << "-" << Msg::TagRawData << std::endl;
        }

        void Text::No_RawData(std::string &data)
        {
            // Do Nothing, just return
            return;
        }

        void Text::Yes_RawData(std::string &data)
        {
            _RawTextStart();
            (*m_stream) << data;
            _RawTextEnd();
        }

        void Text::StartLog()
        {
            (*m_stream) << "START: LOG" << std::endl;
        }

        void Text::EndLog()
        {
            (*m_stream) << "END: LOG" << std::endl;
        }

        void Text::StartBlock(std::string &key)
        {
            (*m_stream) << std::string("START: " + key);
            (*m_stream) << std::endl;
        }

        void Text::EndBlock(std::string &key)
        {
            (*m_stream) << std::string("END: " + key);
            (*m_stream) << std::endl;
        }


        void Text::WriteBlock(Msg *pMsg)
        {
            Msg::Msg_Part *pParts;
            std::string _name;
            std::string _value;

            pParts = pMsg->Parts();

            for (unsigned short part = 0; part < pMsg->Total_Parts(); part++)
            {
                _name.assign(pParts[part]._name);
                _value.assign(pParts[part]._data);

                (*m_stream) << "\t" << _name << "=\"" << _value << "\"" << std::endl;
            }
            (this->*m_pRawData)(pMsg->RawData());

        }

        void Text::BeforeDump(std::string filename)
        {

                if (m_fstream.is_open())
                {
                    m_fstream.close();
                }

                m_fstream.open(filename.c_str(), std::ofstream::out);

                if ( ! m_fstream.is_open())
                {
                    throw(-1);

                    return;
                }

                m_stream = &m_fstream;

        }

        void Text::AfterDump()
        {
                if (m_fstream.is_open())
                {
                    m_fstream.close();
                }
        }

    }

}
