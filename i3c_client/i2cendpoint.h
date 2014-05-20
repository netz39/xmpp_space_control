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

#ifndef I2CENDPOINT_H__
#define I2CENDPOINT_H__


#include <stdexcept>
#include <string>
#include <map>

namespace xmppsc {

//! Exception during I2C communication via an endpoint.
class I2CEndpointException : public std::exception {
public:
    //! Create a new exception.
    /*!
     * @param address Device address of the I2C peer.
     * @param errno The errno from the backing I2C library
     * @param const msg an error message if available
     */
    I2CEndpointException(const int address, const int error, const std::string& msg);

    virtual ~I2CEndpointException() throw();

    //! Get the device address corresponding to this exception.
    virtual const int address() const throw();

    //! Get the error (errno) corresponding to this exception.
    virtual const int error() const throw();

    //! Get the exception message.
    /*!
     * \returns the exception message
     */
    virtual const char* what() const throw();
private:
    const int m_address;
    const int m_error;
    const std::string m_what;
};

//! I2C communication endpoint
/*!
 * I2C endpoint representing a single device on the bus.
 *
 * An endpoint must be setup in order to be used. Afterwards read and write
 * operations can be arbitrarily used. On errors an I2CEndpointException is thrown.
 *
 * This design is strongly related to the wiringPi I2C library, but can be used for
 * other implementations, too.
 *
 * See http://wiringpi.com/reference/i2c-library/ for details. Most documentation
 * is just copied from there.
 */
class I2CEndpoint {
public:
    //! Create an endpoint instance for a specific address.
    /*!
     * @param address The I2C address of the target device.
     * @throws std::out_of_range if the address is not a valid I2C address
     * @throw I2CEndpointException if the endpoint cannot be initialized
     */
    // TODO check parameter type, 8 bits are sufficient
    I2CEndpoint(const int address) throw (I2CEndpointException, std::out_of_range);

    ~I2CEndpoint() throw();

    //! Return the address for this endpoint.
    /*!
     * @returns The endpoint device address.
     */
    const int address() const throw();

    //! Simple device read_reg_16
    /*!
     * Some devices present data when you read them without having to do any register transactions.
     *
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int read() throw(I2CEndpointException);
    //! Simple device write.
    /*!
     * Some devices accept data this way without needing to access any internal registers.
     *
     * @param data The data to be written.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int write(const int data) throw(I2CEndpointException);

    //! Read 8 bits of data from a device register.
    /*!
     * @param reg The device register.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int read_reg_8(const int reg) throw(I2CEndpointException);
    //! Read 16 bits of data from a device register.
    /*!
     * @param reg The device register.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int read_reg_16(const int reg) throw(I2CEndpointException);

    //! Write 8 bits of data to a device register.
    /*!
     * @param reg The device register.
     * @param data The data.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int write_reg_8(const int reg, const int data) throw(I2CEndpointException);
    //! Write 16 bits of data to a device register.
    /*!
     * @param reg The device register.
     * @param data The data.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    int write_reg_16(const int reg, const int data) throw(I2CEndpointException);

protected:
    int _fd() const throw();

private:
    // No Copies of this instance!
    // (Not implemented and never to be called.)
    I2CEndpoint(const I2CEndpoint& other);

    const int m_address;
    int m_fd;
};

//! Store and manage a cache of already established I2C endpoints
class I2CEndpointBroker {
public:
    //! Create an I2C broker instance.
    I2CEndpointBroker();

    //! Clean-up the instance and clean-up/remove all existing I2C endpoints.
    ~I2CEndpointBroker() throw();

    //! Create (if necessary) and return an I2C endpoint for the specified address.
    I2CEndpoint* endpoint(const int address) throw(I2CEndpointException, std::out_of_range);

private:
    typedef std::map<int, I2CEndpoint*> endpoint_map;
    endpoint_map endpoints;

    void free_all_endpoints() throw();
};


} // namespace xmppsc


#endif // I2CENDPOINT_H__
