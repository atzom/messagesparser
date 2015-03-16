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
#include <cstring>
#include <set>

#include "library/options.h"
#include "library/parser.h"
#include "library/output.h"
#include "library/xml.h"
#include "library/text.h"
#include "library/stdout.h"
#include "library/msg.h"


void Syntax(std::string process_name)
{
    std::cout << "Syntax is :" << process_name << " <options>" << std::endl;
    std::cout << "\t -f <filename>\t\t\t-\tInput log file" << std::endl;
    std::cout << "\t -t [text/xml/stdout]\t\t-\tTo output to Text file, Xml file or Stdout" << std::endl;
    std::cout << "\t -w <filename>\t\t\t-\tOutput file name" << std::endl;
    std::cout << "\t -c <config file>\t\t-\tConfig file name" << std::endl;
    std::cout << std::endl;
    std::cout << "\t Common Input options:" << std::endl;
    std::cout << "\t\t -notab\t\t\t\t-\tRemove tabs during log file parsing" << std::endl;
    std::cout << "\t\t -nocr\t\t\t\t-\tRemove CR during log file parsing" << std::endl;
    std::cout << "\t\t -nonumbers\t\t\t-\tRemove Numbers during log file parsing" << std::endl;
    std::cout << "\t\t -delim <delimiter>\t\t-\tLog file, message / line delimiter [default is LF (\\n)]" << std::endl;
    std::cout << "\t Common Output options:" << std::endl;
    std::cout << "\t\t -rawdata\t\t\t-\tTo include raw data of each record" << std::endl;
    std::cout << "\t Output XML options:" << std::endl;
    std::cout << "\t\t -xmlversion [1.0/1.1]\t\t-\tXML Version output (default is 1.0)" << std::endl;
    std::cout << "\t\t -noxmlspecials\t\t\t-\tTo output without converting the special XML characters (default is with xml specials)" << std::endl;
}

int main(int argc, char **argv)
{
    MessagesParser::Output::Type _eType = MessagesParser::Output::eStdout;
    MessagesParser::Options _options(argc, argv);
    MessagesParser::Parser _parser;
    MessagesParser::Output::Output *_output = 0;
    MessagesParser::Output::XmlVersion _eXmlVersion;
    std::string _input_log;
    std::string _output_file;
    std::string _output_type;
    std::string _delim;
    std::string _xmlversion;
    std::string _cfg;

    if (
            ( ! _options.Value("-f", _input_log)) ||
            ( ! _options.Value("-t", _output_type))
        )
            
    {
        Syntax(argv[0]);

        return 1;
    }


    if ((_output_type != "text") && (_output_type != "xml") && (_output_type != "stdout"))
    {
        Syntax(argv[0]);

        return 1;
    }

    if ( ((_output_type == "text") || (_output_type=="xml")) && ( ! _options.Value("-w", _output_file)) )
    {
        Syntax(argv[0]);

        return 1;
    }

    if (_options.Value("-delim",_delim))
    {
        _parser.SetLogFileDelimiter(_delim.at(0));
    }


    if (_options.Exist("-notab"))
        _parser.KeepTabs(false);

    if (_options.Exist("-nocr"))
        _parser.KeepCR(false);

    if (_options.Exist("-nonumbers"))
        _parser.KeepNumbers(false);

    if ( ! _parser.Open(_input_log))
    {
        std::cerr << "Unable to open the input file" << std::endl;
        return 1;
    }


    if (_output_type == "stdout" )
    {
        _output = new MessagesParser::Output::Stdout;
        _eType = MessagesParser::Output::eStdout;
    }
    else if (_output_type == "text" )
    {
        _output = new MessagesParser::Output::Text;
        _eType = MessagesParser::Output::eTxtFile;
    }
    else if (_output_type == "xml" )
    {
        
        _output = new MessagesParser::Output::Xml;
        _eType = MessagesParser::Output::eXmlFile;

        if (_options.Exist("-noxmlspecials"))
            ((MessagesParser::Output::Xml *)_output)->SetXMLSpecials(false);
        else
            ((MessagesParser::Output::Xml *)_output)->SetXMLSpecials(true);


        _eXmlVersion = MessagesParser::Output::eXml_V_1_0;

        if (_options.Value("-xmlversion", _xmlversion))
        {
            if (_xmlversion == "1.0")
                _eXmlVersion  = MessagesParser::Output::eXml_V_1_0;
            else
                _eXmlVersion  = MessagesParser::Output::eXml_V_1_1;
        }

        ((MessagesParser::Output::Xml *)_output)->SetXmlVersion(_eXmlVersion);

    }

    if (_options.Value("-c", _cfg))
    {
        if ( ! _parser.Config(_cfg))
        {
            std::cerr << "Couldn't open config file. Aborting..." << std::endl;
            return 1;
        }
    }

    if (_options.Exist("-rawdata"))
        _output->ShowRawData(true);
    else
        _output->ShowRawData(false);

    _parser.ParseFile();
    _output->Dump(_parser.GetLog(), _output_file);

    _parser.Close();


    return 0;
}
