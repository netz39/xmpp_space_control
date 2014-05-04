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

#ifndef I2CMETHODS_H__
#define I2CMETHODS_H__

#include "../space_control/spacecontrolclient.h"

namespace xmppsc {

class I2CReadMethod : public CommandMethod {
  public:
    I2CReadMethod();
    ~I2CReadMethod();
    
    virtual void handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink);
};

class I2CWriteMethod : public CommandMethod {
  public:
    I2CWriteMethod();
    ~I2CWriteMethod();
    
    virtual void handleSpaceCommand(gloox::JID peer, SpaceCommand sc, SpaceCommandSink* sink);
};
}
#endif // I2CMETHODS_H__