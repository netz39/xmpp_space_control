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

#ifndef ACCESSFILTER_H__
#define ACCESSFILTER_H__

#include <vector>

#include <gloox/jid.h>


namespace xmppsc {

//! General access filter interface.
/*!
 * The access filter decides whether a specific JID is alled for the target service.
 */
class AccessFilter {
public:
    AccessFilter() throw();

    virtual ~AccessFilter() throw();

    //! Check if JID is accepted.
    /*!
     * @param jid the JID to be checked
     * @returns true if the JID can be accepted, otherwise false
     */
    virtual bool accepted(const gloox::JID& jid) const throw() = 0;
};


class ListAccessFilter : public AccessFilter {
public:
    // TODO compare method
    typedef std::vector<gloox::JID> jid_list;

    ListAccessFilter(const jid_list& jids) throw();
    
    virtual ~ListAccessFilter() throw();

    const jid_list& jids() const throw();
    
    virtual bool accepted(const gloox::JID& jid) const throw();
private:
  const jid_list m_jids;
};

} // namespace xmppsc

#endif //ACCESSFILTER_H__

// End of File
