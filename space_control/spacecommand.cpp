/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spacecommand.h"

#include <sstream>

namespace xmppsc {
  
SpaceCommandFormatException::SpaceCommandFormatException(std::string _what,
        std::string _body, int _line_number=0)
    : m_what(_what), m_body(_body), m_line_number(_line_number) {}

SpaceCommandFormatException::~SpaceCommandFormatException() throw() {}

const char* SpaceCommandFormatException::what() const throw() {
    return m_what.c_str();
}

const char* SpaceCommandFormatException::body() const throw() {
    return m_body.c_str();
}

int SpaceCommandFormatException::line_number() const throw() {
    return m_line_number;
}


MissingCommandParameterException::MissingCommandParameterException(
    const std::string _name)
    : m_name(_name)
{
    std::stringstream msg;
    msg << "Missing parameter: " << m_name;
    m_what = msg.str();

}

MissingCommandParameterException::~MissingCommandParameterException() throw() {}

const char* MissingCommandParameterException::what() const throw()
{
    return m_what.c_str();
}

const char* MissingCommandParameterException::name() const throw()
{
    return m_name.c_str();
}


IllegalCommandParameterException::IllegalCommandParameterException(
    const std::string& _name, const std::string& _reason)
    : m_name(_name), m_reason(_reason)
{
    std::stringstream msg;
    msg << "Illegal parameter (" << m_name << "): " << _reason;
    m_what = msg.str();

}

IllegalCommandParameterException::~IllegalCommandParameterException() throw() {}

const char* IllegalCommandParameterException::what() const throw()
{
    return m_what.c_str();
}

const char* IllegalCommandParameterException::name() const throw()
{
    return m_name.c_str();
}

const char* IllegalCommandParameterException::reason() const throw()
{
    return m_reason.c_str();
}


SpaceCommand::SpaceCommand(const std::string& _cmd,
                           const space_command_params& _params) throw()
    : m_cmd(_cmd), m_params(_params) {}

SpaceCommand::SpaceCommand(const SpaceCommand& other)
    : m_cmd(other.m_cmd), m_params(other.m_params) {}

const std::string& SpaceCommand::cmd() const throw() {
    return this->m_cmd;
}

const std::string& SpaceCommand::param(const std::string& key) const
throw(MissingCommandParameterException) {
    space_command_params::const_iterator it = m_params.find(key);

    if (it == m_params.end())
        throw MissingCommandParameterException(key);

    return it->second;
}

bool SpaceCommand::param_available(const std::string& key) const throw() {
    return m_params.find(key) != m_params.end();
}


const SpaceCommand::space_command_params& SpaceCommand::params() const throw() {
    return m_params;
}


SpaceCommandSerializer::~SpaceCommandSerializer() {}



TextSpaceCommandSerializer::TextSpaceCommandSerializer() {}

TextSpaceCommandSerializer::~TextSpaceCommandSerializer() {}

std::string TextSpaceCommandSerializer::to_body(const SpaceCommand& cmd, const std::string& threadId) const {

    // build parameter list
    std::stringstream pars("");

    SpaceCommand::space_command_params params = cmd.params();
    SpaceCommand::space_command_params::const_iterator iter;

    for (iter =  params.begin(); iter != params.end(); iter++) {
        // count lines in parameter value
        int paramlines = 0;

        if (iter->second.empty())
            paramlines = 0;
        else {
            paramlines = 1;
            const char* val = iter->second.c_str();
            int i;
            for (i = iter->second.size(); i; i--)
                if (val[i] == '\n')
                    paramlines++;
        }

        if (!pars.str().empty())
            pars << std::endl;
        pars << paramlines << " " << iter->first << std::endl;
        pars << iter->second;
    }


    // build message body
    std::stringstream body;
    body << cmd.cmd() << std::endl;
    body << threadId;
    if (!pars.str().empty())
        body << std::endl << pars.str();

    return body.str();
}

TextSpaceCommandSerializer::Incoming TextSpaceCommandSerializer::to_command(const std::string body)
throw(SpaceCommandFormatException) {
    // the command
    std::string command;
    // the thread Id
    std::string threadId;
    // the parameter map
    SpaceCommand::space_command_params params;

    //parse command and params from msg body
    int line_number = 0;
    std::stringstream ss(body);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        line_number++;

        // first line is command
        if (line_number == 1)
            command = line;
        // second line is the thread Id
        else if (line_number == 2)
            threadId = line;
        else {
            /* Note: std::stoi is not available yet :(
             *
             *            // split the parameter line
             *            const size_t idx = 0;
             *            // first is the number of lines containing the value
             *            const int parlines = std::stoi(line, &idx);
             *            // after the space is the parameter name
             *            const std::string key = line.substr(idx+1);
             */

            // find the space character
            const size_t idx = line.find(' ');
            if (idx == std::string::npos)
                throw SpaceCommandFormatException("Missing space character in parameter key line.", body, line_number);

            // get the number of parameter lines
            const std::string s_parlines = line.substr(0, idx);
            int parlines = strtol(s_parlines.c_str(), 0, 10);
            if (parlines == 0)
                throw SpaceCommandFormatException("Invalid integer for parameter line count.", body, line_number);

            // get the parameter key
            const std::string key = line.substr(idx+1);

            // get the parameter lines
            std::string value;
            while (parlines--) {
                std::string parline;
                if (!std::getline(ss, parline, '\n'))
                    throw SpaceCommandFormatException("There are less lines that stated in the parameter line count!", body, line_number);
                line_number++;

                if (!value.empty())
                    value.append("\n");
                value.append(parline);
            }

            // add to parameter map
            params[key] = value;
        }
    }

    // create the command
    return Incoming(threadId, SpaceCommand(command, params));
}

} // namespace xmppsc

// End of File
