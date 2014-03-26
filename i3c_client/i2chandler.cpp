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

#include "i2chandler.h"

#include <iostream>

using namespace xmppsc;

void I2CHandler::handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink) {
    std::cout << "Got command " << sc.cmd() << " from " << peer.full() << std::endl;

    xmppsc::SpaceCommand response("Hallo Welt!",
                                  xmppsc::SpaceCommand::space_command_params());

    sink->sendSpaceCommand(&response);

    try {
        xmppsc::SpaceCommand::space_command_params params;
        params["id"] = sc.param("id");
        xmppsc::SpaceCommand idcmd("id", params);
        sink->sendSpaceCommand(&idcmd);
    } catch (std::out_of_range &oor) {
        std::cerr << "Parameter id is not available!" << std::endl;
    }

}