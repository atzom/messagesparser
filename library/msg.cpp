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
#include <sstream>
#include "msg.h"

namespace MessagesParser
{

    std::string Msg::TagGeneric = "generic";
    std::string Msg::TagRawData = "RawData";

    Msg::Msg(std::string data, unsigned short total_parts): FloorFormatPart(0)
    {
        m_part_identifier = 0;
        m_part_notdefined = 0;
        m_reservedParts = 0;
        m_totalParts = 0;
        m_pParts = 0;

        // Make a deep copy and not shallow copy!
        m_rawdata.assign(data);

        Create_Parts(total_parts);
    }


    Msg::~Msg()
    {
    }


    bool Msg::Add_Format(Config::LogFormat *pFormat)
    {

        if ( ! pFormat)
            return false;

        if (pFormat->expected_parts == 0)
            return false;

        if ((m_reservedParts + pFormat->expected_parts) > m_totalParts)
            return false;

        m_reservedParts += pFormat->expected_parts;

        m_format.push_back(pFormat);

        return true;
    }


    void Msg::Clear_Values()
    {
        for (unsigned short i = 0; i < m_totalParts; i++)
            Get_Part(i)->_data.clear();
    }


    void Msg::Extract()
    {
        bool bError = false;
        Msg_Format_t::iterator iter;
        unsigned short _part = 0;
        std::size_t _found, _pos;

        for (iter = m_format.begin(); ((iter != m_format.end()) && (!bError)); iter++)
        {
            unsigned short _cp = 0;
            std::string _token = "";
            std::string *_pData_orig = 0;
            Config::LogFormat *_format = *iter;
            std::string _delim;

            _found = 0;
            _pos = 0;

//          std::cout << "+ --------------------------------------" << std::endl;
//          std::cout << *_format._pData_orig << std::endl;
//          std::cout << "* --------------------------------------" << std::endl;


            for (_cp = 0; _cp < _format->expected_parts - 1 ; _cp++)
            {
                _token.clear();

                _pData_orig=Get_Part_Data(_format->based_on_part, false);

                _delim = _format->str_delimiter;

                _found = _pData_orig->find(_delim, _pos);

                if (_found == std::string::npos)
                {
                    _token.assign(_pData_orig->substr(_pos, _pData_orig->length() - _pos));
                    Set_Part_Value(_cp + _part, _token);

//                  std::cout << "N " << _cp + _part << " " << _token << std::endl;
//
                    bError = true;

                    Clear_Values();
                    Set_Part_Value(m_part_identifier, TagGeneric);
                    Set_Part_Value(m_part_notdefined, _token);

                    break;
                }

                _token.assign(_pData_orig->substr(_pos, _found - _pos));
//              std::cout << "C " << _cp + _part << " " << _token << std::endl;

                Set_Part_Value(_cp + _part, _token);

                _pos = _found + 1;
                
            }

            if (_pData_orig->length() > _pos)
                _token.assign(_pData_orig->substr(_pos, _pData_orig->length() - _pos));
            else
                _token.assign(_pData_orig->substr(0, _pData_orig->length()));

            Set_Part_Value(_cp + _part, _token);
//          std::cout << "Z " << _cp + _part << " " << _token << std::endl;

            _part += _format->expected_parts;
        }

//      std::cout << "====================================================" << std::endl;

    }


    void Msg::Create_Parts(unsigned short parts)
    {
        if ( ! m_pParts)
        {
            m_totalParts = 0;
            m_reservedParts = 0;
            m_pParts = new (std::nothrow) Msg_Part[parts];

            if (m_pParts)
            {
                m_totalParts = parts;
            }
            else
                std::cerr << "Unable to create parts" << std::endl;
        }
    }


    bool Msg::Set_Part_Name(unsigned short part, std::string name)
    {
        Msg_Part *p = Get_Part(part);

        if (p)
        {
            p->_name.assign(name);

            m_hashtbl[name] = p;

            return true;
        }

        return false;
    }


    bool Msg::Set_Part_Value(unsigned short part, std::string value)
    {
        Msg_Part *p = Get_Part(part);

        if (p)
        {
            p->_data.assign(value);

            return true;
        }

        return false;
    }


    Msg::Msg_Part * Msg::Get_Part(unsigned short part)
    {
        if ( (m_pParts) && ( part < m_totalParts ) )
            return &m_pParts[part];

        return 0;
    }


    Msg::Msg_Part * Msg::Get_Part(std::string name)
    {
        if (m_pParts)
        {
            Msg_Part *p;

            Msg_Parts::iterator iter = m_hashtbl.find(name);

            if (iter == m_hashtbl.end())
                return 0;

            p=iter->second;

            return p;
        }

        return 0;
    }   


    std::string * Msg::Get_Part_Data(unsigned short part, bool bReturnNull)
    {
        Msg_Part *_pPart = Get_Part(part);

        if (_pPart)
            return &_pPart->_data;

        return ( bReturnNull ? 0 : &m_rawdata );
    }


    std::string * Msg::Get_Part_Data(std::string name, bool bReturnNull)
    {
        Msg_Part *_pPart = Get_Part(name);

        if (_pPart)
            return &_pPart->_data;

        return ( bReturnNull ? 0 : &m_rawdata );
    }


}
