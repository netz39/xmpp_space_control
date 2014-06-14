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

#ifndef I3CMETHODS_H__
#define I3CMETHODS_H__

#include "i2cendpoint.h"
#include "i2cmethods.h"

#include <xmppsc/spacecontrolclient.h>

namespace xmppsc {

class I3CCallMethod : public I2CMethodBase {
public:
    I3CCallMethod(I2CEndpointBroker* broker);
    virtual ~I3CCallMethod() throw();

    virtual void handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink);
};


} // namespace xmppsc
#endif // I2CMETHODS_H__
