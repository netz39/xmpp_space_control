#include <stdlib.h>

#include <iostream>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"
#include "../space_control/methodhandler.h"
#include "../space_control/daemon.h"


#include "i2cmethods.h"
#include "i3cmethods.h"
#include "i2cendpoint.h"


int main(int argc, char **argv) {
    xmppsc::Daemon daemon("I3Cclient");
    if (!daemon.seed()) {
      std::cerr << "Daemon already running!" << std::endl;
      exit(EXIT_FAILURE);
    }
  
    xmppsc::I2CEndpointBroker*  broker = new xmppsc::I2CEndpointBroker();

    //xmppsc::I2CEndpoint* ep = broker->endpoint(0x22);
    // Send a double beep
    //std::cout << std::hex << "0x" << ep->read_reg_16(0x95) << std::endl;

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

    i2ch->add_method(new xmppsc::I2CReadMethod(broker));
    i2ch->add_method(new xmppsc::I2CRead8Method(broker));
    i2ch->add_method(new xmppsc::I2CRead16Method(broker));
    i2ch->add_method(new xmppsc::I2CWriteMethod(broker));
    i2ch->add_method(new xmppsc::I2CWrite8Method(broker));
    i2ch->add_method(new xmppsc::I2CWrite16Method(broker));
    i2ch->add_method(new xmppsc::I3CCallMethod(broker));


    if (client) {
        // Use the "eco" variant
        xmppsc::set_eco_tcp_client(client);

        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer(), af);

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    if (af)
        delete af;

    delete broker;

    return 0;
}
