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


#include "i2cmethods.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace
{

unsigned int hex2int(const std::string& hex) throw(std::invalid_argument)
{
    std::stringstream conv;
    conv << std::hex << hex;

    int i;

    if (! (conv >> i)) {
        std::ostringstream msg("");
        msg << "Error on hex value conversion (value " << hex << ")!";
        throw std::invalid_argument(msg.str());
    }

    return i;
}

// http://stackoverflow.com/a/5100745
template< typename T > const std::string int2hex(T i)
{
    // get the size
    register int c = 2;
    T _i = i;
    while(_i >>= 8)
        c++;
    c += c % 2;

    // create the hex stream
    std::stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(c)
           << std::hex << i;
    return stream.str();
}
} // anon namespace


namespace xmppsc {


I2CMethodBase::I2CMethodBase(const std::string& command, I2CEndpointBroker* broker) throw(std::invalid_argument)
    : CommandMethod(command), m_broker(broker)
{
    if (!m_broker)
        throw std::invalid_argument("Broker ptr must not be null!");
}

I2CEndpointBroker* I2CMethodBase::broker() const throw()
{
  return m_broker;
}

I2CMethodBase::~I2CMethodBase() throw () {}



I2CReadMethod::I2CReadMethod(I2CEndpointBroker* broker): I2CMethodBase("i2c.read", broker) {}


I2CReadMethod::~I2CReadMethod() throw () {}

void I2CReadMethod::handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink *sink)
{
    //TODO check the parameter conversions and add exceptions accordingly

    // get the device
    const std::string _device = sc.param("device");
    const unsigned int device = hex2int(_device);

    // get the address
    const std::string _address = sc.param("address");
    const unsigned int address = hex2int(_address);


    // get the device endpoint
    I2CEndpoint* ep = broker()->endpoint(device);
    
    // TODO perfom read
    std::cout << "Perform I2C read on address " << std::hex << address << " of device " << device << "." << std::endl;
    const int result = ep->read_reg_16(address);


    xmppsc::SpaceCommand::space_command_params params;
    params["device"] = int2hex(device);
    params["address"] = int2hex(address);
    params["response"] = int2hex(result);
    xmppsc::SpaceCommand idcmd("i2c.update", params);
    sink->sendSpaceCommand(&idcmd);
}


I2CWriteMethod::I2CWriteMethod(I2CEndpointBroker* broker): I2CMethodBase("i2c.write", broker) {}

I2CWriteMethod::~I2CWriteMethod() throw () {}

void I2CWriteMethod::handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink *sink)
{
    //TODO check the parameter conversions and add exceptions accordingly

    // get the device
    const std::string _device = sc.param("device");
    const unsigned int device = hex2int(_device);

    // get the address
    const std::string _address = sc.param("address");
    const unsigned int address = hex2int(_address);

    // get the value
    const std::string _value = sc.param("value");
    const unsigned int value = hex2int(_value);


    // TODO perfom write
    std::cout << "Perform I2C write of value " << std::hex << value << " on address " << address << " of device " << device << "." << std::endl;

    const int result = 0;

    xmppsc::SpaceCommand::space_command_params params;
    params["device"] = ::int2hex(device);
    params["address"] = ::int2hex(address);
    params["value"] = ::int2hex(value);
    params["response"] = ::int2hex(result);
    xmppsc::SpaceCommand idcmd("i2c.update", params);
    sink->sendSpaceCommand(&idcmd);
}

} // namespace xmppsc

// End of file




