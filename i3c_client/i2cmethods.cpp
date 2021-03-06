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

#include <xmppsc/util.h>

namespace xmppsc {

int retrieveHexParameter(const std::string& parameter, const xmppsc::SpaceCommand& sc, bool required, int def)
throw (xmppsc::IllegalCommandParameterException, xmppsc::MissingCommandParameterException) {
    try {
        const std::string _val = sc.param(parameter);

        // special handling for empty values
        // This code seems a bit redundant, but we safe throwing an exception if the value is optional.
        if (_val.empty()) {
            if (required) {
                std::ostringstream msg("");
                msg << "Missing parameter value for " << parameter << "!";
                throw xmppsc::MissingCommandParameterException(msg.str());
            } else
                return def;
        }

        return  hex2int(_val);
    } catch (std::invalid_argument& ia) {
        throw xmppsc::IllegalCommandParameterException(parameter, ia.what());
    } catch (xmppsc::MissingCommandParameterException& e) {
        if (required)
            throw e;
        return def;
    }
}


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

void I2CReadMethod::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);

    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom read
#ifdef DEBUG
        std::cout << "Perform I2C read on device 0x" << device << "." << std::endl;
#endif
	const int result = ep->read();

        // send result
        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}


I2CRead8Method::I2CRead8Method(I2CEndpointBroker* broker): I2CMethodBase("i2c.read8", broker) {}

I2CRead8Method::~I2CRead8Method() throw () {}

void I2CRead8Method::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int reg = retrieveHexParameter("register", sc);


    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom read
#ifdef DEBUG
        std::cout << "Perform I2C read on 8-bit register 0x" << std::hex << reg << " of device 0x" << device << "." << std::endl;
#endif
        const int result = ep->read_reg_8(reg);

        // send result
        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["register"] = int2hex(reg);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}


I2CRead16Method::I2CRead16Method(I2CEndpointBroker* broker): I2CMethodBase("i2c.read16", broker) {}

I2CRead16Method::~I2CRead16Method() throw () {}

void I2CRead16Method::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int reg = retrieveHexParameter("register", sc);


    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom read
#ifdef DEBUG	
        std::cout << "Perform I2C read on 16-bit register 0x" << std::hex << reg << " of device 0x" << device << "." << std::endl;
#endif
        const int result = ep->read_reg_16(reg);

        // send result
        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["register"] = int2hex(reg);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}


I2CWriteMethod::I2CWriteMethod(I2CEndpointBroker* broker): I2CMethodBase("i2c.write", broker) {}

I2CWriteMethod::~I2CWriteMethod() throw () {}

void I2CWriteMethod::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int data = retrieveHexParameter("data", sc);


    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom write
#ifdef DEBUG
        std::cout << "Perform I2C write of value 0x" << std::hex << data << " to device 0x" << device << "." << std::endl;
#endif
        const int result = ep->write(data);

        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["value"] = int2hex(data);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}


I2CWrite8Method::I2CWrite8Method(I2CEndpointBroker* broker): I2CMethodBase("i2c.write8", broker) {}

I2CWrite8Method::~I2CWrite8Method() throw () {}

void I2CWrite8Method::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int reg = retrieveHexParameter("register", sc);
    const unsigned int data = retrieveHexParameter("data", sc);


    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom write
#ifdef DEBUG
        std::cout << "Perform I2C 8-bit write of value 0x" << std::hex << data << " to register 0x" << reg << " of device 0x" << device << "." << std::endl;
#endif
        const int result = ep->write_reg_8(reg, data);

        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["register"] = int2hex(reg);
        params["value"] = int2hex(data);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}


I2CWrite16Method::I2CWrite16Method(I2CEndpointBroker* broker): I2CMethodBase("i2c.write16", broker) {}

I2CWrite16Method::~I2CWrite16Method() throw () {}

void I2CWrite16Method::handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink *sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int reg = retrieveHexParameter("register", sc);
    const unsigned int data = retrieveHexParameter("data", sc);


    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        // perfom write
#ifdef DEBUG
        std::cout << "Perform I2C 16-bit write of value 0x" << std::hex << data << " to register 0x" << reg << " of device 0x" << device << "." << std::endl;
#endif
        const int result = ep->write_reg_16(reg, data);

        xmppsc::SpaceCommand::space_command_params params;
        params["device"] = int2hex(device);
        params["register"] = int2hex(reg);
        params["value"] = int2hex(data);
        params["response"] = int2hex(result);
        const xmppsc::SpaceCommand idcmd("i2c.update", params);
        sink->sendSpaceCommand(idcmd);
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}

} // namespace xmppsc

// End of file




