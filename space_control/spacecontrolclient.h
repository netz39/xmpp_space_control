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
#include <map>
#include <stdexcept>
#include <set>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/message.h>
#include <gloox/messagesession.h>
#include <gloox/messagehandler.h>
#include <gloox/messagesessionhandler.h>


namespace xmppsc {


//! Exception on parsing errors in space command messages.
/*!
 * This exception is thrown when parsing a message body for SpaceCommandS
 * fails. A description of the failure and the message body are provided.
 */
class SpaceCommandFormatException : public std::exception {
public:
    //! Create a new exception
    /*!
     * \param _what        The exception message, goes to std::exception::what()
     * \param _body        The body of the original space command message.
     * \param _line_number The number of the errornous line, defaults to 0.
     */
    SpaceCommandFormatException(std::string _what, std::string _body, int _line_number);

    //! Virtual destructor derrived from std::exception.
    virtual ~SpaceCommandFormatException() throw();

    //! Get the exception message.
    /*!
     * \returns the exception message
     */
    virtual const char* what() const throw();

    //! Get the space command body
    /*!
     * The body is unaltered and can be used for further inspection.
     *
     * \returns the space command body in raw format
     */
    virtual const char* body() const throw();

    //! Get the line number.
    /*!
     * \returns the number of the errornous line.
     */
    virtual int line_number() const throw();

private:
    std::string m_what;
    std::string m_body;
    int m_line_number;
};


//! Exception for missing command parameters.
class MissingCommandParameterException : public std::exception {
public:
    //! Create a new exception
    /*!
     * \param _name        The name of the missing parameter.
     */
    MissingCommandParameterException(const std::string _name);

    //! Virtual destructor derrived from std::exception.
    virtual ~MissingCommandParameterException() throw();

    //! Get the exception message.
    /*!
     * \returns the exception message
     */
    virtual const char* what() const throw();

    virtual const char* name() const throw();

private:
    std::string m_name;
    /* 
     * The message for what() is functional dependent on m_name, 
     * but must be stored here due to scope issues. Assembly is
     * done in the constructor.
     */
    std::string m_what;
};


//! Space Command representation.
/*!
 * This class represents a Space Command, consisting of the issuing/receiving
 * peer, the command name and the parameter map.
 */
//TODO copy constructor
class SpaceCommand {
public:
    //! Convenience typedef for the parameter map.
    typedef std::map<const std::string, std::string> space_command_params;

    //! Create a new Space Command representation.
    /*!
     * \param _cmd    The command name
     * \param _params The parameter map.
     */
    SpaceCommand(const std::string _cmd, space_command_params _params);

    //! Get the command name.
    /*!
     * \returns The command name
     */
    std::string cmd();

    //! Get a parameter value
    /*!
     * \param key The parameter name.
     * \returns The value for the provided key.
     * \throws std::out_of_range if the parameter does not exist.
     */
    const std::string param(const std::string key)
      throw(MissingCommandParameterException);

    space_command_params params();

private:
    std::string m_cmd;
    space_command_params m_params;
};

//! Interface to a Space Command serializer
class SpaceCommandSerializer {
public:
    virtual ~SpaceCommandSerializer() = 0;

    //! Serialize a Space Command for sending
    /*!
     * \param cmd Pointer to the space command, must not be null
     * \returns The serialized message as String
     */
    virtual std::string to_body(SpaceCommand* cmd) = 0;

    //! De-Serialize a received Space Command
    /*!
     * \param body The message body containing the command.
     * \returns pointer to the created command
     * \throws SpaceCommandFormatException if the body cannot be de-serialized
     */
    virtual SpaceCommand to_command(const std::string body)
    throw(SpaceCommandFormatException) = 0;
};

//! Serializer for text-based messages
class TextSpaceCommandSerializer : public SpaceCommandSerializer {
public:
    TextSpaceCommandSerializer();

    virtual ~TextSpaceCommandSerializer();

    virtual std::string to_body(SpaceCommand* cmd);

    virtual SpaceCommand to_command(const std::string body)
    throw(SpaceCommandFormatException);
};

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
    virtual void sendSpaceCommand(SpaceCommand* sc) = 0;
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
    virtual void handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink) = 0;
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
    virtual void handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink) = 0;

private:
    t_command_set m_commands;
};


//! The space control client.
/*!
 * An XMPP client, that listens for XMPP messages, converts them to
 * Space Commands and deals with the Space Command handler.
 */
class SpaceControlClient : public gloox::MessageSessionHandler, gloox::MessageHandler {
public:
    //! Create the space control client
    /*!
     * \param _client An already configured XMPP client.
     * \param _hnd the space control handler to process the received Space Commands
     * \param _ser the space command serializer
     */
    SpaceControlClient(gloox::Client* _client, SpaceControlHandler* _hnd, SpaceCommandSerializer* _ser);

    //! message session handler
    /*!
     * \sa gloox::MessageSessionHandler::handleMessageSession
     */
    virtual void handleMessageSession(gloox::MessageSession* session);

    //! message handler
    /*!
     * Converts an XMPP message into a Space Command and calls the
     * Space Command handler.
     * \sa gloox::MessageHandler::handleMessage
     */
    virtual void handleMessage(const gloox::Message& msg, gloox::MessageSession* session = 0);

    //! Get the XMPP client.
    /*!
     * \returns the XMPP client used by this space control client.
     */
    gloox::Client* client();

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
    SpaceCommandSink* create_sink(gloox::JID peer);

protected:
    //! Get the space command serializer
    /*!
     * \returns the space command serializer
     */
    SpaceCommandSerializer* serializer();

private:
    gloox::Client* m_client;
    SpaceControlHandler* m_hnd;
    SpaceCommandSerializer* m_ser;
};

}

#endif // SPACECONTROLCLIENT_H__
