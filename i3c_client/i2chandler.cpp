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
#include <sstream>

using namespace xmppsc;




I2CHandler::~I2CHandler() {}

void I2CHandler::handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink) {
    const std::string cmd = sc.cmd();
    try {
        std::cout << "Got command " << cmd << " from " << peer.full() << std::endl;

        CommandMethod* method = m_methods.at(cmd);

        if (method)
            method->handleSpaceCommand(peer, sc, sink);
        else
            std::cerr << "Assertion error: Method not found, but did not throw an exception!" << std::endl;
    } catch (std::out_of_range &oor) {
        SpaceCommand::space_command_params par;
        par["what"] = oor.what();
        std::stringstream s;
        s << "Unknown command: " << cmd;
        par["text"] = s.str();
        SpaceCommand ex("exception", par);
        sink->sendSpaceCommand(&ex);
    }
}

void I2CHandler::add_method(CommandMethod* method) {
    CommandMethod::t_command_set::iterator iter;

    for (iter =  method->command_set().begin();
            iter != method->command_set().end(); iter++) {
        std::cout << "Adding command " << *iter << " from method." << std::endl;
        m_methods[*iter] = method;
    }
}



