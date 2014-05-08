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


#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace xmppsc;

const unsigned int hex2str(const std::string hex) {
    int i;

    std::stringstream conv;
    conv << std::hex << hex;
    if (! (conv >> i)) {
        //TODO exception?
        i = 0;
    }

    return i;
}

// http://stackoverflow.com/a/5100745
template< typename T > const std::string int2hex( T i ) {
    // get the size
    register int c = 2;
    T _i = i;
    while (_i >>= 8)
        c++;
    c+=c%2;

    // create the hex stream
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(c)
           << std::hex << i;
    return stream.str();
}

I2CReadMethod::I2CReadMethod(): CommandMethod("i2c.read") {}

I2CReadMethod::~I2CReadMethod() {}

void I2CReadMethod::handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink* sink) {
    // get the device
    const std::string _device = sc.param("device");
    const unsigned int device = hex2str(_device);

    // get the address
    const std::string _address = sc.param("address");
    const unsigned int address = hex2str(_address);


    // TODO perfom read
    std::cout << "Perform I2C read on address " << std::hex << address << " of device " << device << "." << std::endl;
    const int result = 30;


    xmppsc::SpaceCommand::space_command_params params;
    params["device"] = int2hex(device);
    params["address"] = int2hex(address);
    params["response"] = int2hex(result);
    xmppsc::SpaceCommand idcmd("i2c.update", params);
    sink->sendSpaceCommand(&idcmd);
}


I2CWriteMethod::I2CWriteMethod(): CommandMethod("i2c.write") {}

I2CWriteMethod::~I2CWriteMethod() {}

void I2CWriteMethod::handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink* sink) {
    // get the device
    const std::string _device = sc.param("device");
    const unsigned int device = hex2str(_device);

    // get the address
    const std::string _address = sc.param("address");
    const unsigned int address = hex2str(_address);

    // get the value
    const std::string _value = sc.param("value");
    const unsigned int value = hex2str(_value);


    // TODO perfom write
    std::cout << "Perform I2C write of value " << std::hex << value << " on address " << address << " of device " << device << "." << std::endl;

    const int result = 0;

    xmppsc::SpaceCommand::space_command_params params;
    params["device"] = int2hex(device);
    params["address"] = int2hex(address);
    params["value"] = int2hex(value);
    params["response"] = int2hex(result);
    xmppsc::SpaceCommand idcmd("i2c.update", params);
    sink->sendSpaceCommand(&idcmd);
}


// End of file
