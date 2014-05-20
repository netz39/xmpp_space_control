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

/*!
 * This is the dummy implementation of the plattform-specific I2C endpoint part.
 */


#include "i2cendpoint.h"

#include <iostream>
#include <string>
#include <sstream>

namespace {

void __dummy_message(const std::string msg) {
    std::cout << msg << std::endl;
}

int __dummy_input(const std::string msg) {
    std::cout << msg;

    int in;
    std::cin >> std::hex >> in;

    return in;
}

} // anon namespace


namespace xmppsc {

I2CEndpoint::I2CEndpoint(const int address) throw (I2CEndpointException, std::out_of_range)
    : m_address(address), m_fd(0)
{
    if (address < 0 || address > 0xff) {
        std::stringstream msg("");
        msg << "I2C address " << address << " is out of range, must be between 0 and 0xFF!";
        throw std::out_of_range(msg.str());
    }
    
    m_fd = 1;
    std::stringstream msg("");
    msg << "I2C dummy device " << m_address << " has been set up with fd " << m_fd;

    ::__dummy_message(msg.str());    
}

I2CEndpoint::~I2CEndpoint() throw()
{
    m_fd = 0;
    std::stringstream msg("");
    msg << "I2C dummy device " << m_address << " has been closed.";

    ::__dummy_message(msg.str());
}


const int I2CEndpoint::address() const throw()
{
    return m_address;
}

int I2CEndpoint::_fd() const throw() {
    return m_fd;
}


int I2CEndpoint::read() throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input simple read result (hex) for device 0x" << std::hex << m_address << ": ";
    return ::__dummy_input(msg.str());
}


int I2CEndpoint::write(const int data) throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input simple write result (hex) for device 0x" << std::hex << m_address
        << ", written value 0x"  << data << ": ";
    return ::__dummy_input(msg.str());

}

int I2CEndpoint::read_reg_8(const int reg) throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input 8-bit read result (hex) for device 0x" << std::hex << m_address
        << " on register 0x" << reg << ": ";
    return ::__dummy_input(msg.str());
}

int I2CEndpoint::read_reg_16(const int reg) throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input 16-bit read result (hex) for device 0x" << std::hex << m_address
        << " on register 0x" << reg << ": ";
    return ::__dummy_input(msg.str());
}

int I2CEndpoint::write_reg_8(const int reg, const int data) throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input 8-bit write result (hex) for device 0x" << std::hex << m_address
        << " on register 0x"  << reg << ", written value 0x" << data << ": ";
    return ::__dummy_input(msg.str());
}

int I2CEndpoint::write_reg_16(const int reg, const int data) throw(I2CEndpointException)
{
    std::stringstream msg("");
    msg << "Please input 16-bit write result (hex) for device 0x" << std::hex << m_address
        << " on register 0x" << reg << ", written value 0x" << data << ": ";
    return ::__dummy_input(msg.str());
}

} // namespace xmppsc

// End of File
