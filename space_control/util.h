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

#ifndef UTIL_H__
#define UTIL_H__

#include <stdexcept>
#include <string>

namespace xmppsc {

//! Convert a hex string to integer value
/**
 * @param hex The hex value as string
 * @returns the integer value
 * @throws std::invalid_argument if the hex value cannot be converted.
 */
unsigned int hex2int(const std::string& hex) throw(std::invalid_argument);

//! Convert an integer value to hex string
/**
 * @param i the integer value
 * @returns the hex string
 */
const std::string int2hex(unsigned int  i);

} // namespace xmppsc

#endif // UTIL_H
