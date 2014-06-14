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
#include <iomanip>

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
  
  
} // namespace xmppsc

// End of File