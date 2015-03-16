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

#ifndef _ATZ_MSG_
#define _ATZ_MSG_ 1


#include <string>
#include <sstream>
#include <list>
#include <map>
#include <functional>

#include "config.h"


namespace MessagesParser
{

    class Msg
    {

        public:

            const unsigned short FloorFormatPart;
            static std::string TagGeneric;
            static std::string TagRawData;

            typedef std::list<Config::LogFormat *> Msg_Format_t;


            typedef struct Msg_Part
            {
                Msg_Part()
                {
                    static unsigned long long _id = 0;
                    std::stringstream _ss;

                    _name.clear();
                    _data.clear();

                    _ss << _id;

                    _id++;

                    _name.assign(std::string("PART_" + _ss.str()));
                }

                std::string _name;
                std::string _data;
            } Msg_Part;


            typedef std::map<std::string, Msg_Part *> Msg_Parts;

        protected:
            std::string     m_rawdata;

            unsigned short  m_part_identifier;
            unsigned short  m_part_notdefined;

            Msg_Format_t    m_format;

            Msg_Parts       m_hashtbl;

            Msg_Part        *m_pParts;

            unsigned short  m_totalParts;
            unsigned short  m_reservedParts;

            unsigned short  m_key_part_id;


            void Clear_Values();

        public:
            Msg(std::string data, unsigned short total_parts);
            ~Msg();


            inline std::string & RawData() { return m_rawdata; }

            bool Add_Format(Config::LogFormat *);

            void Create_Parts(unsigned short parts);

            bool Set_Part_Name(unsigned short part, std::string name);
            bool Set_Part_Value(unsigned short part, std::string value);

            inline void Set_Identifier(unsigned short part) { m_part_identifier = part; }
            inline unsigned short Get_Identifier() { return m_part_identifier; }

            inline void Set_NotFoundPartsBucket(unsigned short part) { m_part_notdefined = part; }
            inline unsigned short Get_NotFoundPartsBucket() { return m_part_notdefined; }

            Msg_Part * Get_Part(unsigned short part);
            Msg_Part * Get_Part(std::string name);
            std::string * Get_Part_Data(unsigned short part, bool bReturnNull = false);
            std::string * Get_Part_Data(std::string name, bool bReturnNull = false);

            
            inline unsigned short Total_Parts() { return m_totalParts; }

            Msg_Part * Parts() { return m_pParts; }

            void Extract();
    };

}


#endif
