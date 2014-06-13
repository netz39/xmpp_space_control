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


#include "i3cmethods.h"

#include <iostream>
#include <sstream>

namespace {
union I2C_result {
    unsigned char c[2];
    unsigned short r;
};
} // anon namespace

namespace xmppsc {

I3CCallMethod::I3CCallMethod(I2CEndpointBroker* broker): I2CMethodBase("i3c.call", broker) { }

I3CCallMethod::~I3CCallMethod() throw() {}

void I3CCallMethod::handleSpaceCommand(gloox::JID peer, const SpaceCommand& sc, SpaceCommandSink* sink)
{
    // get parameters
    const unsigned int device = retrieveHexParameter("device", sc);
    const unsigned int command = retrieveHexParameter("command", sc);
    const unsigned int data = retrieveHexParameter("data", sc, false, 0);

    // render the register value
    unsigned char send = (command << 4) + data;

    // calculate the parity
    char v = send;
    char c;
    for (c = 0; v; c++)
        v &= v-1;
    c &= 1;

    // set parity bit
    send += (c << 7);

    try {
        // get the device endpoint
        I2CEndpoint* ep = broker()->endpoint(device);

        I2C_result result;
        result.r = 0;

        // maximal number of tries
        int hops=20;

        // try for hops times until the result is not zero
        while (!result.c[0] && --hops) {

            // perfom read
#ifdef DEBUG
            std::cout << "I3C call with command 0x" << std::hex << command << ", data 0x" << data
                      << " on device 0x" << device << " (TTL " << std::dec << hops << ")." << std::endl;
            std::cout << "Perform I2C read on 16-bit register 0x" << std::hex << (int)send << " of device 0x" << device << "." << std::endl;
#endif
	    result.r = ep->read_reg_16(send);

            // check for transmission errors: 2nd byte is inverted 1st byte
            const unsigned char c = ~result.c[0];
            if (result.c[1] != c)
                // if no match, reset the result
                result.r = 0;
        }

        if (hops) {
            // send result
            xmppsc::SpaceCommand::space_command_params params;
            params["device"] = int2hex(device);
            params["command"] = int2hex(command);
            params["data"] = int2hex(data);
            params["response"] =  int2hex<int>(result.c[0]);
            params["i2c.register"] = int2hex<int>(send);
            params["i2c.response"] = int2hex<int>(result.r);
            const xmppsc::SpaceCommand idcmd("i3c.response", params);
            sink->sendSpaceCommand(idcmd);
        } else {
            // send error
            xmppsc::SpaceCommand::space_command_params params;
            params["device"] = int2hex(device);
            params["command"] = int2hex(command);
            params["data"] = int2hex(data);
            params["i2c.register"] = int2hex<int>(send);
            const xmppsc::SpaceCommand idcmd("i3c.timeout", params);
            sink->sendSpaceCommand(idcmd);
        }
    } catch (const I2CEndpointException& e) {
        // send exception
        I2C_EX_MSG
    }
}

} // namespace xmppsc

// End of File

