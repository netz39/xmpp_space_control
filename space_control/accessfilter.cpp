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


#include "accessfilter.h"


namespace xmppsc {

AccessFilter::AccessFilter() throw() {}

AccessFilter::~AccessFilter() throw() {}



ListAccessFilter::ListAccessFilter(const ListAccessFilter::jid_list& jids) throw()
    : m_jids(jids) { }

ListAccessFilter::~ListAccessFilter() throw() { }

const ListAccessFilter::jid_list& ListAccessFilter::jids() const throw()
{
    return m_jids;
}

bool ListAccessFilter::accepted(const gloox::JID& jid) const throw()
{
    // go through the list
    for (jid_list::const_iterator it = m_jids.begin(); it != m_jids.end(); ++it) {
        const gloox::JID j(*it);

        // if a ressource is not specified, match the user@host part only
        if (j.resource().empty() && !j.bare().compare(jid.bare()))
            return true;

        // otherwise, match completely
        // (the == operator is overloaded to compare the full JID
        if (j == jid)
            return true;

        // Hint: We suspect that jid will always have a resource set,
        //       as this is the case with actual (live) JIDs.
    }

    // nothing matched
    return false;
}


} // namespace xmppsc

// End of File
