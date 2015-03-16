/*
 * Copyright (C) 2015, Andreas Tzomakas
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 		Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <ctime>

#include "xml.h"
#include "strutil.h"

namespace MessagesParser
{

    namespace Output
    {

        struct invalid_xml_value: public std::binary_function<int , MessagesParser::Output::XmlVersion , bool >
        {

            bool operator() (const int c, const MessagesParser::Output::XmlVersion version) const
            {
                // explicitly remove the characters less than 0x20 if found since it is not allowed to exist into XML
                // from this rule '\t', '\r' and '\n' are excluded
                if ( 
                        (version == eXml_V_1_0) &&
                        ( c < 0x20 ) && 
                        (( c != '\t') && (c != '\r') && (c != '\n'))
                    )
                {
                    return true;
                }
                else if (
                            (version == eXml_V_1_1) &&
                            (c == 0x0)
                        )
                {
                    return true;
                }

                return false;
            }
        };

        Xml::Xml(): Output()
        {
            SetXmlVersion(eXml_V_1_0);
            SetXMLSpecials();   // set the default values

            m_pSpecialChars = &Xml::_NoSpecialChars;        // point somewhere valid regardless of the default selected options, these will be checked later on

            m_stream = &m_fstream;
        }

        Xml::~Xml()
        {
        }

        void Xml::_RawXmlStart()
        {
            (*m_stream) << "<field name=\"" << Msg::TagRawData << "\"><![CDATA[" << std::endl;
        }

        void Xml::_RawXmlEnd()
        {
            (*m_stream) << std::endl << "]]></field>" << std::endl;
        }

        void Xml::No_RawData(std::string &data)
        {
        }

        void Xml::Yes_RawData(std::string &data)
        {
            _RawXmlStart();

            data.erase( std::remove_if(data.begin(), data.end(), std::bind2nd(MessagesParser::Output::invalid_xml_value(), m_xmlVersion)), data.end());
            (*m_stream) << data;

            _RawXmlEnd();
        }

        unsigned long Xml::_UseSpecialChars(std::string &data)
        {
            char _schars[]="<>\'\"&";
            std::size_t _found = data.find_first_of(_schars, 0);
            std::string _str_r = "";
            unsigned long _changes = 0;

            while (_found != std::string::npos)
            {

                switch (data[_found])
                {
                    case '&':

                        _str_r = "&amp;";

                        break;
                    case '>':

                        _str_r = "&gt;";

                        break;
                    case '<':

                        _str_r = "&lt;";

                        break;
                    case '"':

                        _str_r = "&quot;";

                        break;
                    case '\'':

                        _str_r = "&apos;";

                        break;

                    default:
                        continue;
                }

                _changes++;
                data = data.substr(0, _found) + _str_r + data.substr(_found+1, data.length());

                _found = data.find_first_of(_schars, _found + _str_r.length());
            }

            data.erase( std::remove_if(data.begin(), data.end(), std::bind2nd(MessagesParser::Output::invalid_xml_value(), m_xmlVersion)), data.end());

            return _changes;
        }

        unsigned long Xml::_NoSpecialChars(std::string &data)
        {
            return 0;
        }


        void Xml::StartLog()
        {
            (*m_stream) << "<?xml version=\"" << m_xmlVersionStr << "\" encoding=\"UTF-8\"?>" << std::endl;
            (*m_stream) << "<Log>" << std::endl;
        }

        void Xml::EndLog()
        {
            (*m_stream) << "</Log>" << std::endl;
        }


        void Xml::StartBlock(std::string &key)
        {
            std::string _key(key);

            _key.erase( std::remove(_key.begin(), _key.end(), ':'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '<'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '>'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '&'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '"'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '\''), _key.end());

            MessagesParser::Str::trim_whitespaces(_key);

            (*m_stream) << std::string("<" + _key + ">");
            (*m_stream) << std::endl;
        }

        void Xml::EndBlock(std::string &key)
        {
            std::string _key(key);

            _key.erase( std::remove(_key.begin(), _key.end(), ':'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '<'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '>'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '&'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '"'), _key.end());
            _key.erase( std::remove(_key.begin(), _key.end(), '\''), _key.end());


            MessagesParser::Str::trim_whitespaces(_key);

            (*m_stream) << std::string("</" + _key + ">");
            (*m_stream) << std::endl;
        }

        void Xml::WriteBlock(Msg *pMsg)
        {
            Msg::Msg_Part *pParts;
            std::string _name;
            std::string _value;

            (*m_stream) << "<record>" << std::endl;

            pParts = pMsg->Parts();

            for (unsigned short part = 0; part < pMsg->Total_Parts(); part++)
            {
                _name.assign(pParts[part]._name);
                _value.assign(pParts[part]._data);

                (this->*m_pSpecialChars)(_name);
                (this->*m_pSpecialChars)(_value);
                (*m_stream) << "<field name=\"" << _name << "\">" << _value << "</field>" << std::endl;
            }
            (this->*m_pRawData)(pMsg->RawData());
            (*m_stream) << "</record>" << std::endl;

        }


        void Xml::BeforeDump(std::string filename)
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


                if (m_bXMLSpecials)
                    m_pSpecialChars = &Xml::_UseSpecialChars;
                else
                    m_pSpecialChars = &Xml::_NoSpecialChars;

                m_stream = &m_fstream;
        }

        void Xml::AfterDump()
        {
                if (m_fstream.is_open())
                {
                    m_fstream.close();
                }
        }

    }


}
