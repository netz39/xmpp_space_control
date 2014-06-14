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

//TODO extrn?

#ifndef SPACECONTROLCLIENT_H__
#define SPACECONTROLCLIENT_H__

#include <string>
#include <stdexcept>
#include <set>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/message.h>
#include <gloox/messagesession.h>
#include <gloox/messagehandler.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionlistener.h>

#include "accessfilter.h"
#include "spacecommand.h"

namespace xmppsc {

//! Interface to send out Space Commands
/*!
/* A sink that has been provided to a command handler, must not be disposed.
 * However, if the sink has been obtained explicitly, the caller is
 * responsible for disposing and deleting the sink object.
 */
class SpaceCommandSink {
public:
    //! Send a Space Command.
    /*!
     * \param sc The space command to be sent.
     */
    virtual void sendSpaceCommand(const SpaceCommand& sc) = 0;

    //! Return the communication thread ID.
    /*!
     *  Ideally we would use the XMPP thread ID which, however, as proven to be unreliable.
     * @return The thread ID
     */
    virtual const std::string& threadId() const throw() = 0;
};


//! Interface for space control handlers to receive and process messages.
/*!
 * This class must be derrived in order to handle message sent to the
 * Space Control client, i.e. this is the entry point for implementing
 * applications.
 */
class SpaceControlHandler {
public:
    //! Handle an incoming Space Command
    /*!
     * \param peer The communication peer as provided by GLOOX
     * \param sc   The incoming Space Command instance.
     * \param sink The sink for response commands.
     */
    virtual void handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink) = 0;
};

//! Handler for a set of commands.
class CommandMethod : public xmppsc::SpaceControlHandler {
public:
    //! A set for command names
    /*!
     * A convenience declaration to ease code interpretation.
     */
    typedef std::set<std::string> t_command_set;

    //! Create a command method for a set of commands.
    /*!
     * \param _commands the set of commands understood by this method.
     */
    CommandMethod(t_command_set _commands);

    //! Create a command method for a single command.
    /*!
     * \param _command the command understood by this method
     */
    CommandMethod(const std::string _command);

    virtual ~CommandMethod() throw();

    //! Get the command set.
    /*!
     * \returns the set of commands available from this method.
     */
    t_command_set& command_set();

    //! Override to actually do something with the command.
    /*!
     * \sa xmppsc::SpaceControlHandler
     */
    virtual void handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink) = 0;

private:
    t_command_set m_commands;
};


//! The space control client.
/*!
 * An XMPP client, that listens for XMPP messages, converts them to
 * Space Commands and deals with the Space Command handler.
 */
class SpaceControlClient : public gloox::MessageHandler, gloox::ConnectionListener {
public:
    //! Create the space control client
    /*!
     * \param _client An already configured XMPP client.
     * \param _hnd the space control handler to process the received Space Commands
     * \param _ser the space command serializer
     */
    SpaceControlClient(gloox::Client* _client, SpaceControlHandler* _hnd, SpaceCommandSerializer* _ser, AccessFilter* _access);
    
    virtual ~SpaceControlClient();

    //! message handler
    /*!
     * Converts an XMPP message into a Space Command and calls the
     * Space Command handler.
     * \sa gloox::MessageHandler::handleMessage
     */
    virtual void handleMessage(const gloox::Message& msg, gloox::MessageSession* session = 0);
    
    virtual void onConnect();
    
    //! Disconnect handler
    /*!
     * Store the disconnection reason
     */
    virtual void onDisconnect(gloox::ConnectionError e);
    
    virtual bool onTLSConnect(const gloox::CertInfo& info);

    //! Get the XMPP client.
    /*!
     * \returns the XMPP client used by this space control client.
     */
    gloox::Client* client();
    
    gloox::ConnectionError conn_error() const throw();

    //! Get the space control handler.
    /*!
     * \returns the space control handler used by this space control client.
     */
    SpaceControlHandler* handler();

    //! Create a space command sink with a new session.
    /*!
     * \param peer The communication peer for the session.
     * \returns a space command sink with a new session.
     */
    SpaceCommandSink* create_sink(const gloox::JID& peer, const std::string& threadId);

    const AccessFilter* access() const throw();

protected:
    //! Get the space command serializer
    /*!
     * \returns the space command serializer
     */
    SpaceCommandSerializer* serializer();

private:
    gloox::Client* m_client;
    gloox::ConnectionError m_conn_error;
    SpaceControlHandler* m_hnd;
    SpaceCommandSerializer* m_ser;
    AccessFilter* m_access;
};


void set_eco_tcp_client(gloox::Client* client);

class EcoConnectionTCPClient : public gloox::ConnectionTCPClient {
public:
    EcoConnectionTCPClient(gloox::ConnectionDataHandler* cdh, const gloox::LogSink& logInstance,
                           const std::string& server, int port);
    
    virtual gloox::ConnectionError receive();
};

}

#endif // SPACECONTROLCLIENT_H__



