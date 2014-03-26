#include <iostream>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"

#include "i2chandler.h"

int main(int argc, char **argv) {
    gloox::Client* client=0;
    try {
        xmppsc::ConfiguredClientFactory ccf("spacecontrol.config");
        client = ccf.newClient();
    } catch (xmppsc::ConfiguredClientFactoryException &ccfe) {
        std::cerr << "ConfiguredClientFactoryException: " << ccfe.what() << std::endl;
        return (-1);
    }


    if (client) {      
        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, new xmppsc::I2CHandler(), 
									 new xmppsc::TextSpaceCommandSerializer());

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    return 0;
}