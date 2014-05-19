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

#include "i2cendpoint.h"


using namespace xmppsc;


I2CEndpointException::I2CEndpointException(const int address, const int error, const std::string msg)
    : m_address(address), m_error(error), m_what(msg) { }

I2CEndpointException::~I2CEndpointException() throw() {}

const int I2CEndpointException::address() throw() {
    return m_address;
}

const int I2CEndpointException::error() throw()
{
    return m_error;
}

const char* I2CEndpointException::what() const throw()
{
    return m_what.c_str();
}


// End of file
