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

#ifndef _ATZ_XML_
#define _ATZ_XML_ 1

#include <iostream>
#include <fstream>

#include "log.h"
#include "output.h"

namespace MessagesParser
{
    namespace Output
    {

        typedef enum
        {
            eXml_V_1_0,
            eXml_V_1_1,
        } XmlVersion;

        class Xml: public Output
        {
            public:

                typedef enum
                {
                    eTxtFile,
                    eXmlFile,
                    eStdout
                } Type;


            private:
                void            _RawXmlStart();
                void            _RawXmlEnd();

            protected:

                XmlVersion      m_xmlVersion;
                std::string     m_xmlVersionStr;

                std::ofstream   m_fstream;

                bool            m_bXMLSpecials;

                unsigned long   (Xml::*m_pSpecialChars)(std::string &data);
                unsigned long   _UseSpecialChars(std::string &data);
                unsigned long   _NoSpecialChars(std::string &data);

                void            No_RawData(std::string &data);
                void            Yes_RawData(std::string &data);

                virtual void    BeforeDump(std::string filename);
                virtual void    AfterDump();

                virtual void    StartLog();
                virtual void    EndLog();
                virtual void    StartBlock(std::string &);
                virtual void    EndBlock(std::string &);
                virtual void    WriteBlock(Msg *);


            public:
                Xml();
                ~Xml();

                inline void SetXMLSpecials(bool bMode=true) { m_bXMLSpecials = bMode; }

                inline void SetXmlVersion(XmlVersion mode)
                {
                    m_xmlVersion = mode;

                    if (m_xmlVersion == eXml_V_1_0)
                        m_xmlVersionStr = "1.0";
                    else if (m_xmlVersion == eXml_V_1_1)
                        m_xmlVersionStr = "1.1";
                }
        };

    }
}

#endif
