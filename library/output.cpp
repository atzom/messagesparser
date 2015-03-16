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

#include "output.h"
#include "strutil.h"

namespace MessagesParser
{

    namespace Output
    {

        Output::Output()
        {
            m_pRawData = &Output::_No_RawData;                  // point somewhere valid regardless of the default selected options, these will be checked later on
        }

        Output::~Output()
        {
        }

        void Output::_No_RawData(std::string &data)
        {
            No_RawData(data);
        }

        void Output::_Yes_RawData(std::string &data)
        {
            Yes_RawData(data);
        }

        void Output::BeforeDump(std::string filename)
        {
            // Do nothing, if something is needed to be run before the dump overload this function
        }

        void Output::AfterDump()
        {
            // Do nothing, if something is needed to be run before the dump overload this function
        }

        void Output::Dump(Log *pLog, std::string filename)
        {
            Log_t::iterator iter;
            std::string _default_key = "";
            std::string _prev_key = "__NONE__" ;
            std::string _key = "";

            time_t _now = time(0);
            char *_now_time = asctime(localtime(&_now));

            _prev_key += std::string(_now_time);

            _default_key.assign(_prev_key);

            // Before we start dumping things, we run a predump first (things that might be overloaded and needed to run before dump)
            
            try
            {
                BeforeDump(filename);
            }
            catch(...)
            {
                std::cerr << "Error at function 'BeforeDump', aborting dump..." << std::endl;
                return;
            }

            StartLog();

            for (iter = pLog->Begin(); iter != pLog->End(); ++iter)
            {
                Msg *p = iter->second;

                _key.assign(iter->first);

                if ( _key == _prev_key )
                {
                    WriteBlock(p);
                }
                else
                {
                    if (_prev_key != _default_key)
                        EndBlock(_prev_key);

                    StartBlock(_key);

                    _prev_key.assign(_key);
                }
            }

            if (pLog->Size() > 0)
            {
                EndBlock(_prev_key);
            }

            EndLog();

            // Things need to run after the dump (such as cleanup or closing opened files)
            AfterDump();

        }

    }

}
