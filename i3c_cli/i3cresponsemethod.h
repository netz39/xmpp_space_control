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

#ifndef I3CRESPONSEMETHOD_H__
#define I3CRESPONSEMETHOD_H__

#include "finalizingcommandmethod.h"

namespace xmppsc {

class I3CResponseMethod : public FinalizingCommandMethod {
public:
    I3CResponseMethod(gloox::Client* _client) throw();
    virtual ~I3CResponseMethod() throw();

    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink);
};
  
} // namespace xmppsc
#endif # I3CRESPONSEMETHOD_H__
