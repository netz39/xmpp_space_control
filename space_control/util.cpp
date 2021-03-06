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

#include "util.h"

#include <sstream>
#include <iostream>
#include <iomanip>

namespace {
  
//! Calculate the (even) hex width of a number
/*!
 * i.e. the hex digit count rounded up to an even number
 */
inline int width(int i) {
    int c = 2;
    unsigned int _i = i;
    while(_i >>= 8)
        c++;
    c += c % 2;
    
    return c;
}

} //anonymous namespace

namespace xmppsc {
  
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
const std::string int2hex(unsigned int i)
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(width(i))
           << std::hex << i;
    return stream.str();
}

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
 
} // namespace xmppsc

// End of File