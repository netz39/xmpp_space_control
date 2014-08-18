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

#ifndef FINALIZINGCOMMANDMETHOD_H__
#define FINALIZINGCOMMANDMETHOD_H__

#include <xmppsc/methodhandler.h>

namespace xmppsc {

class FinalizingCommandMethod : public xmppsc::CommandMethod {
public:
    FinalizingCommandMethod(gloox::Client* _client, const std::string _command);

    FinalizingCommandMethod(gloox::Client* _client, const t_command_set _commands);

    //! Evaluate the result and decide if the client may be terminated.
    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) = 0;

    virtual void handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink);
protected:
    gloox::Client* client() const throw();

private:
    gloox::Client* m_client;
};

} // namespace xmppsc
#endif // FINALIZINGCOMMANDMETHOD_H__
// End of File
