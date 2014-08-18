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

#include "i3cresponsemethod.h"

#include <iostream>

namespace xmppsc {
  
I3CResponseMethod::I3CResponseMethod(gloox::Client* _client) throw()
    : FinalizingCommandMethod(_client, "i3c.response") {}
I3CResponseMethod::~I3CResponseMethod() throw() {}

bool I3CResponseMethod::evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) {
    // evaluate result
    try {
	const std::string response = sc.param("response");
	std::cout << response << std::endl;
    } catch (const xmppsc::MissingCommandParameterException& e) {
	std::cerr << "Missing response!" << std::endl;
    }

    return true;
}
  
} // namespace xmppsc
// End of File