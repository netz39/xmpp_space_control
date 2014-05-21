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

//TODO comments
//TODO fix the newline specification

#include "spacecontrolclient.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>


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

const SpaceCommand::space_command_params& SpaceCommand::params() const throw() {
    return m_params;
}


SpaceCommandSerializer::~SpaceCommandSerializer() {}


TextSpaceCommandSerializer::TextSpaceCommandSerializer() {}

TextSpaceCommandSerializer::~TextSpaceCommandSerializer() {}

std::string TextSpaceCommandSerializer::to_body(const SpaceCommand& cmd) {

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
    body << cmd.cmd();
    if (!pars.str().empty())
        body << std::endl << pars.str();

    return body.str();
}

SpaceCommand TextSpaceCommandSerializer::to_command(const std::string body)
throw(SpaceCommandFormatException) {
    // the command
    std::string command;
    // the parameter map
    SpaceCommand::space_command_params params;

    //parse command and params from msg body
    int line_number = 0;
    std::stringstream ss(body);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        line_number++;

        // first line is command
        if (command.empty())
            command = line;
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
    return SpaceCommand(command, params);
}


// local helper class
class Sink : public SpaceCommandSink {
public:
    Sink(gloox::MessageSession* _session, SpaceCommandSerializer* _ser, bool _shared=false)
        : m_session(_session), m_ser(_ser), m_shared(_shared) {}
    virtual ~Sink();

    virtual void sendSpaceCommand(const SpaceCommand& sc);

private:
    SpaceCommandSerializer* m_ser;
    gloox::MessageSession* m_session;
    bool m_shared;
};

Sink::~Sink() {
    if (!m_shared && m_session)
        delete m_session;
}

void Sink::sendSpaceCommand(const SpaceCommand& sc) {
    if (m_session) {
        m_session->send(m_ser->to_body(sc));
    }
    //TODO else
}


CommandMethod::CommandMethod(CommandMethod::t_command_set _commands)
    : m_commands(_commands) {}

CommandMethod::CommandMethod(const std::string _command) {
    m_commands.insert(_command);
}

CommandMethod::~CommandMethod() throw() { }


CommandMethod::t_command_set& CommandMethod::command_set() {
    return m_commands;
}


SpaceControlClient::SpaceControlClient(gloox::Client* _client, SpaceControlHandler* _hnd, SpaceCommandSerializer* _ser)
    : m_client(_client), m_hnd(_hnd), m_ser(_ser) {
    if (_client)
        _client->registerMessageSessionHandler(this);
}

void SpaceControlClient::handleMessageSession(gloox::MessageSession* session) {
    std::cout << "Message session from " << session->target().full() << std::endl;
    session->registerMessageHandler(this);
}

void SpaceControlClient::handleMessage(const gloox::Message& msg, gloox::MessageSession* session) {
    if (session)
        try {
            // create the command
            // may throw a SpaceCommandFormatException
            const SpaceCommand cmd = serializer()->to_command(msg.body());

            // create shared sink
            Sink sink(session, m_ser, true);

            // call handler
            if (m_hnd)
                m_hnd->handleSpaceCommand(session->target(), cmd, &sink);

        } catch (SpaceCommandFormatException& scfe) {
            SpaceCommand::space_command_params par;
            par["what"] = scfe.what();
            par["body"] = scfe.body();

            // add line number if available
            if (scfe.line_number()) {
// use std::to_string if available
#ifdef COMPILER_SUPPORTS_CXX11
                par["line number"] = std::to_string(scfe.line_number());
#else // COMPILER_SUPPORTS_CXX11
                std::stringstream s;
                s << scfe.line_number();
                par["line number"] = s.str();
#endif // COMPILER_SUPPORTS_CXX11
            }

            SpaceCommand ex("exception", par);

            session->send(serializer()->to_body(ex));
        }
    //TODO else warn
}

gloox::Client* SpaceControlClient::client() {
    return this->m_client;
}

SpaceControlHandler* SpaceControlClient::handler() {
    return this->m_hnd;
}

SpaceCommandSerializer* SpaceControlClient::serializer() {
    return this->m_ser;
}


SpaceCommandSink* SpaceControlClient::create_sink(gloox::JID peer) {
    gloox::MessageSession* session = new gloox::MessageSession(this->m_client, peer);

    // return sink that is not shared
    return new Sink(session, m_ser, false);
}


} // namespace xmppsc

// End of File


