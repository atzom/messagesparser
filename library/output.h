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

#ifndef _ATZ_OUTPUT_
#define _ATZ_OUTPUT_ 1

#include <iostream>
#include <fstream>

#include "log.h"

namespace MessagesParser
{

    namespace Output
    {

        typedef enum
        {
            eTxtFile,
            eXmlFile,
            eStdout
        } Type;

        class Output
        {

            private:
                virtual void    _No_RawData(std::string &data);
                virtual void    _Yes_RawData(std::string &data);


            protected:

                std::ostream    *m_stream;

                void            (Output::*m_pRawData)(std::string &data);
                virtual void    No_RawData(std::string &data)   = 0;
                virtual void    Yes_RawData(std::string &data)  = 0;

                virtual void    BeforeDump(std::string filename);
                virtual void    AfterDump();

                virtual void    StartLog()                  = 0;
                virtual void    EndLog()                    = 0;
                virtual void    StartBlock(std::string &)   = 0;
                virtual void    EndBlock(std::string &)     = 0;
                virtual void    WriteBlock(Msg *)           = 0;

            public:
                Output();
                ~Output();

                virtual void    ShowRawData(bool bMode = false)
                {
                    if (bMode)
                        m_pRawData = &Output::_Yes_RawData;
                    else
                        m_pRawData = &Output::_No_RawData;
                }


                void Dump(Log *, std::string filename);

        };

    }
}

#endif
