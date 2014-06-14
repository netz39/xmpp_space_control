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

#ifndef SPACECOMMAND_H__
#define SPACECOMMAND_H__

#include <stdexcept>
#include <string>
#include <map>

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


//! Exception for missing command parameters.
class IllegalCommandParameterException : public std::exception {
public:
    //! Create a new exception
    /*!
     * \param _name        The name of the missing parameter.
     */
    IllegalCommandParameterException(const std::string& _name, const std::string& _reason);

    //! Virtual destructor derrived from std::exception.
    virtual ~IllegalCommandParameterException() throw();

    //! Get the exception message.
    /*!
     * \returns the exception message
     */
    virtual const char* what() const throw();

    virtual const char* name() const throw();

    virtual const char* reason() const throw();
private:
    std::string m_name;
    std::string m_reason;
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
 *
 * This class is immutable.
 */
class SpaceCommand {
public:
    //! Convenience typedef for the parameter map.
    typedef std::map<const std::string, std::string> space_command_params;

    //! Create a new Space Command representation.
    /*!
     * \param _cmd    The command name
     * \param _params The parameter map.
     */
    SpaceCommand(const std::string& _cmd, const space_command_params& _params) throw();

    //! Copy constructor.
    SpaceCommand(const SpaceCommand& other);

    //! Get the command name.
    /*!
     * \returns The command name
     */
    const std::string& cmd() const throw();

    //! Get a parameter value
    /*!
     * \param key The parameter name.
     * \returns The value for the provided key.
     * \throws MissingCommandParameterException if the parameter does not exist.
     */
    const std::string& param(const std::string& key) const
    throw(MissingCommandParameterException);

    //! Check if a parameter is available
    /*!
     * \param key The parameter name
     * \returns true, if the parameter is available, otherwise false
     */
    bool param_available(const std::string& key) const throw();

    //! Direct access to the parameter map.
    /*!
     * Stick to the provided member functions if possible!
     */
    const space_command_params& params() const throw();

private:
    const std::string m_cmd;
    const space_command_params m_params;
};

//! Interface to a Space Command serializer
class SpaceCommandSerializer {
public:
    typedef std::pair<std::string, SpaceCommand> Incoming;

    virtual ~SpaceCommandSerializer() = 0;

    //! Serialize a Space Command for sending
    /*!
     * \param cmd Pointer to the space command, must not be null
     * \returns The serialized message as String
     */
    virtual std::string to_body(const SpaceCommand& cmd, const std::string& threadId) const = 0;

    //! De-Serialize a received Space Command
    /*!
     * \param body The message body containing the command.
     * \returns pointer to the created command
     * \throws SpaceCommandFormatException if the body cannot be de-serialized
     */
    virtual Incoming to_command(const std::string body)
    throw(SpaceCommandFormatException) = 0;
};


//! Serializer for text-based messages
class TextSpaceCommandSerializer : public SpaceCommandSerializer {
public:
    TextSpaceCommandSerializer();

    virtual ~TextSpaceCommandSerializer();

    virtual std::string to_body(const SpaceCommand& cmd, const std::string& threadId) const;

    virtual Incoming to_command(const std::string body)
    throw(SpaceCommandFormatException);
};

} // namespace xmppsc

#endif // SPACECOMMAND_H__

