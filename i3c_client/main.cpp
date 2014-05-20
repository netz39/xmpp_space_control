#include <iostream>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"
#include "../space_control/methodhandler.h"


#include "i2cmethods.h"
#include "i2cendpoint.h"


class TestMethod : public xmppsc::CommandMethod {
public:
    TestMethod();
    virtual ~TestMethod();

    virtual void handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink* sink);
};

TestMethod::TestMethod(): CommandMethod("test") {
}

TestMethod::~TestMethod() {}

void TestMethod::handleSpaceCommand(gloox::JID peer, xmppsc::SpaceCommand sc, xmppsc::SpaceCommandSink* sink) {
    xmppsc::SpaceCommand response("Hallo Welt!",
                                  xmppsc::SpaceCommand::space_command_params());

    sink->sendSpaceCommand(&response);

    try {
        xmppsc::SpaceCommand::space_command_params params;
        params["id"] = sc.param("id");
        xmppsc::SpaceCommand idcmd("id", params);
        sink->sendSpaceCommand(&idcmd);
    } catch (std::out_of_range &oor) {
        std::cerr << "Parameter id is not available!" << std::endl;
    }
}


int main(int argc, char **argv) {
  xmppsc::I2CEndpointBroker broker;
  
  xmppsc::I2CEndpoint ep = broker.endpoint(0x22);
  
  
  // Send a double beep
  std::cout << std::hex << "0x" << ep.read_reg_16(0x95) << std::endl;
  
  
  return 0;
  
  
    gloox::Client* client=0;
    try {
        xmppsc::ConfiguredClientFactory ccf("spacecontrol.config");
        client = ccf.newClient();
    } catch (xmppsc::ConfiguredClientFactoryException &ccfe) {
        std::cerr << "ConfiguredClientFactoryException: " << ccfe.what() << std::endl;
        return (-1);
    }

    xmppsc::MethodHandler* i2ch = new xmppsc::MethodHandler();
    TestMethod* m = new TestMethod();
    i2ch->add_method(m);
    
    i2ch->add_method(new xmppsc::I2CReadMethod());
    i2ch->add_method(new xmppsc::I2CWriteMethod());


    if (client) {
        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer());

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    return 0;
}
