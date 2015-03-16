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

#include "log.h"
#include "strutil.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

namespace MessagesParser
{
    Log::Log()
    {
    }


    Log::~Log()
    {
        Clear();
    }


    void Log::Add(std::string &data)
    {
        Msg *pMsg = new (std::nothrow) Msg(data, m_cfg.Get_Parts()->size());
        Msg::Msg_Part *p;

        if ( ! pMsg)
        {
            std::cerr << "Couldn't allocate memory, aborting adding log line..." << std::endl;
            return;
        }

        for (Config::Formats_t::iterator iter = m_cfg.Get_Formats()->begin(); iter != m_cfg.Get_Formats()->end(); iter++)
        {
            Config::LogFormat *_format = *iter;

            pMsg->Add_Format(_format);
        }

        pMsg->Set_Identifier(m_cfg.Get_Identifier());

        size_t _part_number = 0;
        for (Config::Parts_t::iterator iter = m_cfg.Get_Parts()->begin(); iter != m_cfg.Get_Parts()->end(); iter++)
        {
            pMsg->Set_Part_Name(_part_number, *iter);
            _part_number++;
        }
        ///////////////////////////////////////////////////////

        pMsg->Extract();

        p = pMsg->Get_Part(pMsg->Get_Identifier());

        if (p)
        {
            m_log.insert(std::pair<std::string, Msg *>(p->_data, pMsg));
        }
        else
        {
            delete pMsg;
        }

    }


    void Log::Clear()
    {
        Log_t::iterator iter;
        
        for (iter = m_log.begin(); iter != m_log.end(); ++iter)
        {
            Msg *p = iter->second;

            if (p)
                delete p;
        }

        m_log.clear();
    }

}
