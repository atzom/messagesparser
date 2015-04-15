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

#include "config.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "strutil.h"

namespace MessagesParser
{

    namespace Config
    {

        Config::Config()
        {
			m_identifier = 0;
			m_strIdentifier.clear();
        }

        Config::~Config()
		{
		}

		bool Config::_Read_Block(std::string block, void (Config::*callback)(std::string str) )
		{
			std::ifstream _fin;
			std::string _token = "";
			std::string _line = "";
			
			bool _bBegin = false;
			bool _bBlock = false;

			_fin.open(m_cfg.c_str(), std::ifstream::in);

			if ( ! _fin.is_open())
			{
				std::cerr << "Couldn't open configuration file... weird" << std::endl;
				return false;
			}

			while ( std::getline(_fin, _line, '\n') )
			{

				// The following characters are ignored into the configuration file
				_line.erase( std::remove(_line.begin(), _line.end(), ' '), _line.end() );
				_line.erase( std::remove(_line.begin(), _line.end(), '\r'), _line.end() );
				_line.erase( std::remove(_line.begin(), _line.end(), '\t'), _line.end() );
				//////////////////////////////////////////////////////////////////////////

				if (_line.length() == 0)
					continue;

				// if a line is starting with '#' then we count it as a comment
				if (_line.at(0) == '#')
					continue;

				if (_line.find(_CFG_BEGIN_STR, 0) == 0)
				{
					// Check if already inited
					if ( (_bBegin) || (_bBlock) )
						continue;

					_bBegin = true;

					if (_line.find(block, 0) == 0)
						_bBlock = true;
					else
						_bBegin = false;

					continue;
				}
				else if (_line.find(_CFG_END_STR,0) == 0)
				{
					if ( (_bBegin) && (_bBlock) )
					{
						_bBegin = false;
						_bBlock = false;
					}

					continue;
				}

				if (_bBlock)
				{
					(this->*callback)(_line);
				}
					
			}

			_fin.close();

			return true;
		}


		bool Config::Read_Config(std::string filename)
		{
			m_cfg = filename;

			if ( ! _Read_Block(_CFG_IDENTIFIER_STR, &Config::_Read_Identifier))
			{
				std::cerr << "Couldn't get identifier" << std::endl;
				return false;
			}

			if ( ! _Read_Block(_CFG_FORMATS_STR, &Config::_Read_Formats))
			{
				std::cerr << "Couldn't get formats" << std::endl;
				return false;
			}

			if ( ! _Read_Block(_CFG_PARTS_STR, &Config::_Read_Parts))
			{
				std::cerr << "Couldn't get parts" << std::endl;
				return false;
			}


			if ( ! _Read_Block(_CFG_RULES_STR, &Config::_Read_Rules))
				return false;

			// Check the identifier

			if ( ! Str::str2number(m_strIdentifier, m_identifier))
			{
				m_identifier = 0;

				// not a number thus try to get it via its name
				for (Parts_t::iterator iter = m_parts_names.begin(); iter != m_parts_names.end(); iter++)
				{
					if ( m_strIdentifier.compare(*iter) == 0)
					{
						break;
					} 

					m_identifier++;
				}
			}

			if ( m_identifier >= m_parts_names.size() )
				m_identifier = 0;
			///////////////////////////////////////////////////////////////

			
			// Check the configuration formats
			for (Formats_t::iterator iter = m_formats.begin(); iter != m_formats.end(); iter++)
			{
				LogFormat *_format = *iter;

				if (_format->expected_parts == 0)
				{
                    LogFormat *_format = *iter;

                    delete _format;

					iter = m_formats.erase(iter);

					continue;
				}

				if ( (_format->based_on_part >= 0) && (_format->based_on_part >= m_parts_names.size()) )
				{
                    LogFormat *_format = *iter;

                    delete _format;

					iter = m_formats.erase(iter);

					continue;
				}
			}

			return true;

		}

		void Config::_Read_Identifier(std::string str)
		{
			m_strIdentifier.assign(str);
		}

		void Config::_Read_Formats(std::string str)
		{
			std::stringstream ss(str);
			std::string _token = "";
			unsigned short _part = 0;
			LogFormat *_format = new LogFormat;
            size_t _length = 0;
			
			while( (std::getline(ss, _token, ',')) && (_part < 3) )
			{
				if (_part == 0)
				{
                    _length = _token.length();
					if (_length == 1)
						_format->str_delimiter.assign( std::string(1, _token.at(0)) );
					else if (_length >= 4)
					{
                        std::string _cfgType(_token.substr(0,3));

                        _token.erase(0, 3);

                        if (_cfgType == _CFG_STR_SIG)
                        {
                            _format->str_delimiter.assign(_token);
                        }
                        else if (_cfgType == _CFG_CHR_SIG)
                        {
                            if (_token.compare(_CFG_STR_SPACE) == 0)
                                _format->str_delimiter.assign( std::string(1, _CFG_CHR_SPACE) );
                            else if (_token.compare(_CFG_STR_TAB) == 0)
                                _format->str_delimiter.assign( std::string(1, _CFG_CHR_TAB) );
                            else if (_token.compare(_CFG_STR_SLASH_R) == 0)
                                _format->str_delimiter.assign( std::string(1, _CFG_CHR_SLASH_R) );
                            else if (_token.compare(_CFG_STR_SLASH_N) == 0)
                                _format->str_delimiter.assign( std::string(1, _CFG_CHR_SLASH_N) );
                            else if (_token.compare(_CFG_STR_CRLF) == 0)
                                _format->str_delimiter.assign( std::string(_CFG_CHR_CRLF) );
                            else if (_token.compare(_CFG_STR_COMMA) == 0)
                                _format->str_delimiter.assign( std::string(1, _CFG_CHR_COMMA) );
                        }
                        else if (_cfgType == _CFG_ASCII_SIG)
                        {
                            unsigned int _ascii_code = 0;

                            if (Str::str2number(_token, _ascii_code))
                            {
                                if ((_ascii_code > 0) && (_ascii_code < 255))
                                {
                                    unsigned char _c = _ascii_code;
                                    _format->str_delimiter.assign( std::string(1, _c));
                                }
                            }
                        }
					}
                    else
                    {
                        // Nothing for now
                    }
				}
				else if (_part == 1)
				{
					if ( ! Str::str2number(_token, _format->expected_parts))
						_format->expected_parts = 0;
				}
				else if (_part == 2)
				{
					if ((Str::isNumber<unsigned int>(_token)) && ( ! Str::str2number(_token, _format->based_on_part)))
						_format->based_on_part = -1;
				}

				_part++;
			}

			if (_format->expected_parts > 0)
				m_formats.push_back(_format);
            else
                delete _format;

		}

		void Config::_Read_Parts(std::string str)
		{
			m_parts_names.push_back(str);
		}

		void Config::_Read_Rules(std::string str)
		{
		}


    }

}
