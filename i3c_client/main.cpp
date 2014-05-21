#include <iostream>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"
#include "../space_control/methodhandler.h"


#include "i2cmethods.h"
#include "i2cendpoint.h"


int main(int argc, char **argv) {
    xmppsc::I2CEndpointBroker*  broker = new xmppsc::I2CEndpointBroker();

    //xmppsc::I2CEndpoint* ep = broker->endpoint(0x22);
    // Send a double beep
    //std::cout << std::hex << "0x" << ep->read_reg_16(0x95) << std::endl;

    gloox::Client* client=0;
    try {
        xmppsc::ConfiguredClientFactory ccf("spacecontrol.config");
        client = ccf.newClient();
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


    if (client) {
        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer());

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    delete broker;

    return 0;
}
