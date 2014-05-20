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

#include "i2cendpoint.h"
#include "../space_control/spacecontrolclient.h"

namespace xmppsc {
  
class I2CMethodBase : public CommandMethod {
public:
  I2CMethodBase(const std::string& command, I2CEndpointBroker* broker) 
      throw(std::invalid_argument);
      
  virtual ~I2CMethodBase() throw();
  
protected:
  I2CEndpointBroker* broker() const throw();
  
private:
  I2CEndpointBroker* m_broker;
};

class I2CReadMethod : public I2CMethodBase {
  public:
    I2CReadMethod(I2CEndpointBroker* broker);
    virtual ~I2CReadMethod() throw();
    
    virtual void handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink);
};

class I2CWriteMethod : public I2CMethodBase {
  public:
    I2CWriteMethod(I2CEndpointBroker* broker);
    virtual ~I2CWriteMethod() throw();
    
    virtual void handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink);
};
}
#endif // I2CMETHODS_H__