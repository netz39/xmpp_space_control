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


#include "methodhandler.h"

#include <iostream>
#include <sstream>
#include <cassert>

namespace xmppsc {


MethodHandler::~MethodHandler() {}

void MethodHandler::handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink) {
    const std::string cmd = sc.cmd();
    try {
#ifdef DEBUG
        std::cout << "Got command " << cmd << " from " << peer.full() << std::endl;
#endif

        CommandMethod* method = m_methods.at(cmd);

        if (method) {
            try {
                method->handleSpaceCommand(peer, sc, sink);
            } catch (MissingCommandParameterException &mcp) {
                SpaceCommand::space_command_params par;
                par["what"] = mcp.what();
		par["parameter"] = mcp.name();
                SpaceCommand ex("exception", par);
                sink->sendSpaceCommand(ex);
            } catch (IllegalCommandParameterException &mcp) {
                SpaceCommand::space_command_params par;
                par["what"] = mcp.what();
		par["parameter"] = mcp.name();
                SpaceCommand ex("exception", par);
                sink->sendSpaceCommand(ex);
            }
        } else {
            std::cerr << "Assertion error: Method not found, but did not throw an exception!" << std::endl;
	    // fail in debug mode
	    assert(0);
	}
    } catch (std::out_of_range &oor) {
        SpaceCommand::space_command_params par;
        par["what"] = oor.what();
        std::stringstream s;
        s << "Unknown command: " << cmd;
        par["text"] = s.str();
        SpaceCommand ex("exception", par);
        sink->sendSpaceCommand(ex);
    }
}

void MethodHandler::add_method(CommandMethod* method) {
    CommandMethod::t_command_set::iterator iter;

    for (iter =  method->command_set().begin();
            iter != method->command_set().end(); iter++) {
    	// TODO delete old methods, if already set
        m_methods[*iter] = method;
    }
}



} // namespace xmppsc

// End of file
