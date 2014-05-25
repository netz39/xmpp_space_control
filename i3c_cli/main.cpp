#include <iostream>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"
#include "../space_control/methodhandler.h"

#include "../i3c_client/i2cmethods.h"


int main(int argc, char **argv) {
    gloox::Client* client=0;
    xmppsc::AccessFilter* af=0;
    try {
        xmppsc::ConfiguredClientFactory ccf("spacecontrol.config");
        client = ccf.newClient();
        af = ccf.newAccessFilter();
    } catch (xmppsc::ConfiguredClientFactoryException &ccfe) {
        std::cerr << "ConfiguredClientFactoryException: " << ccfe.what() << std::endl;
        return (-1);
    }

    xmppsc::MethodHandler* i2ch = new xmppsc::MethodHandler();

    if (client) {
        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer(), af);

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    if (af)
        delete af;

    return 0;
}
