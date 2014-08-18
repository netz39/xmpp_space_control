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

#include "finalizingcommandmethod.h"


#include <thread>
#include <chrono>

namespace {

void wait_for_state(const gloox::Client* client, const gloox::ConnectionState& state) {
  while (client->state() != state)
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

}

namespace xmppsc {
  
FinalizingCommandMethod::FinalizingCommandMethod(gloox::Client* _client, const std::string _command)
    : CommandMethod(_command), m_client(_client) {}

FinalizingCommandMethod::FinalizingCommandMethod(gloox::Client* _client, const t_command_set _commands)
    : CommandMethod(_commands), m_client(_client) {}

void FinalizingCommandMethod::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, 
							 xmppsc::SpaceCommandSink* sink) {
    if (evaluate_result(peer, sc, sink)) {
	client()->disconnect();
	wait_for_state(client(), gloox::ConnectionState::StateDisconnected);
    }
}

gloox::Client* FinalizingCommandMethod::client() const throw() {
    return m_client;
}
  
} // namespace xmppsc

// End of file