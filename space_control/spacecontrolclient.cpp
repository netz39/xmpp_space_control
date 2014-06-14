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

#include <gloox/messagesession.h>


namespace xmppsc {

// local helper class
class Sink : public SpaceCommandSink {
public:
    //TODO null ptr exception
    Sink(const std::string& threadId, const gloox::JID peer, gloox::Client* client, const SpaceCommandSerializer* ser)
        : m_threadId(threadId), m_peer(peer), m_client(client), m_ser(ser) {}
    virtual ~Sink() {}

    virtual void sendSpaceCommand(const SpaceCommand& sc);

    virtual const std::string& threadId() const throw();

private:
    const std::string m_threadId;
    const gloox::JID m_peer;
    gloox::Client* m_client;
    const SpaceCommandSerializer* m_ser;
};

void Sink::sendSpaceCommand(const SpaceCommand& sc) {
    std::string body(m_ser->to_body(sc, m_threadId));
    gloox::Message m(gloox::Message::Chat, m_peer, body);

    m_client->send(m);
}

const std::string& Sink::threadId() const throw() {
    return m_threadId;
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


SpaceControlClient::SpaceControlClient(gloox::Client* _client,
                                       SpaceControlHandler* _hnd,
                                       SpaceCommandSerializer* _ser,
                                       AccessFilter* _access
                                      )
    : m_client(_client), m_conn_error(gloox::ConnNotConnected), 
      m_hnd(_hnd), m_ser(_ser), m_access(_access) {
    if (_client) {
        m_client->registerMessageHandler(this);
	m_client->registerConnectionListener(this);
    }
}

SpaceControlClient::~SpaceControlClient()
{
  if (m_client) {
    m_client->removeMessageHandler(this);
    m_client->removeConnectionListener(this);
  }
}

void SpaceControlClient::handleMessage(const gloox::Message& msg, gloox::MessageSession* session) {
    try {
        // create the command
        // may throw a SpaceCommandFormatException
        const SpaceCommandSerializer::Incoming in = serializer()->to_command(msg.body());
        const std::string threadId(in.first);
        const SpaceCommand cmd = in.second;

        // create shared sink
        Sink sink(threadId, msg.from(), m_client, m_ser);

        // check for access
        if (m_access ? m_access->accepted(msg.from()) : true) {

            // call handler
            if (m_hnd)
                m_hnd->handleSpaceCommand(msg.from(), cmd, &sink);
        } else {
            // send access denied message
            SpaceCommand::space_command_params par;
            par["reason"] = "Denied by access filter!";
            SpaceCommand ex("denied", par);
            sink.sendSpaceCommand(ex);
        }
    } catch (const SpaceCommandFormatException& scfe) {
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

        m_client->send(gloox::Message(gloox::Message::Normal, msg.from(),
                                      serializer()->to_body(ex, "")));
    }
}


void SpaceControlClient::onConnect()
{
  m_conn_error = gloox::ConnNoError;
}

void SpaceControlClient::onDisconnect(gloox::ConnectionError e)
{
  // store the connection error
  m_conn_error = e;
}

bool SpaceControlClient::onTLSConnect(const gloox::CertInfo& info)
{
  // just say yes
  return true;
}


gloox::Client* SpaceControlClient::client() {
    return this->m_client;
}

gloox::ConnectionError SpaceControlClient::conn_error() const throw()
{
  return m_conn_error;
}


SpaceControlHandler* SpaceControlClient::handler() {
    return this->m_hnd;
}

SpaceCommandSerializer* SpaceControlClient::serializer() {
    return this->m_ser;
}

SpaceCommandSink* SpaceControlClient::create_sink(const gloox::JID& peer, const std::string& threadId) {
    // return sink
    return new Sink(threadId, peer, m_client, m_ser);

}

const AccessFilter* SpaceControlClient::access() const throw()
{
    return m_access;
}



void set_eco_tcp_client(gloox::Client* client) {
    EcoConnectionTCPClient* ecocon =
        new EcoConnectionTCPClient(client, client->logInstance(), client->server(), client->port());

    client->setConnectionImpl(ecocon);
}

EcoConnectionTCPClient::EcoConnectionTCPClient(gloox::ConnectionDataHandler* cdh, const gloox::LogSink& logInstance,
        const std::string& server, int port)
    : ConnectionTCPClient(cdh, logInstance, server, port)
{}

gloox::ConnectionError EcoConnectionTCPClient::receive()
{
    if( m_socket < 0 )
        return gloox::ConnNotConnected;

    gloox::ConnectionError err = gloox::ConnNoError;
    while( !m_cancel && ( err = recv( 1000000 ) ) == gloox::ConnNoError )
        ;
    return err == gloox::ConnNoError ? gloox::ConnNotConnected : err;
}


} // namespace xmppsc

// End of File


