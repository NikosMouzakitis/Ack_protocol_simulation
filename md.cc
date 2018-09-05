
#include <string.h>
#include <omnetpp.h>


using namespace omnetpp;

/*

        Simulation of a central node that is
       bridging - routing messages to appropriate
       receivers depending on the message received.

 */

class Md : public cSimpleModule
{
    private:
        int counter;
        int seq;
        double timeout;
        cMessage *timeoutEvent;
        cMessage *message;
    public:
        Md();
        virtual ~Md();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void sendCopyOf(cMessage * msg);
        virtual cMessage *newMessage();
};

Define_Module(Md);

Md::Md(){
    timeout = 1.0;
    timeoutEvent = NULL;
}
Md::~Md()
{
    cancelAndDelete(timeoutEvent);
    delete timeoutEvent;
}

void Md::sendCopyOf(cMessage *msg)
{
    cMessage *copy = (cMessage *)msg->dup();

    if(strcmp("a",getName()) ==0){
        send(copy, "o2");
        scheduleAt(simTime() + timeout,timeoutEvent);

    } else if(strcmp("c",getName()) ==0){

        send(copy, "o2");
        scheduleAt(simTime() + timeout,timeoutEvent);

    }

}

cMessage * Md::newMessage()
{
    char msgname[10];

    sprintf(msgname,"SeqNo-%d",++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Md::initialize()
{
    seq = 0; //sequence number.
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    if(strcmp("a", getName()) == 0){
        EV << "Sending Initial message...\n";
        message = newMessage();
        sendCopyOf(message);
        EV << "scheduled TOE.\n";
    }
}

void Md::handleMessage(cMessage *msg)
{
    if(strcmp("bridge",getName()) ==0){

        if( msg->arrivedOn("i1")) { // message from Node A
            //forwarding it to Node B.
            send(msg, "o2");
        } else if( msg->arrivedOn("i2"))  { // message from Node B
            // forwarding it to Node A.
            send(msg, "o1");
        }
        return;
    }

    if(msg == timeoutEvent) {
        EV << "Time out expired.Re-transmitting the message.\n";
        sendCopyOf( message );

    } else { // can be any message in A,C.

        if(strcmp("a",getName())==0){
            EV << "ACK ARRIVED.\n";
            delete msg;
            cancelEvent(timeoutEvent);
            delete message;
            message = newMessage();
            sendCopyOf(message);
        } else if(strcmp("c",getName())==0) {
            if(uniform(0,1) < 0.25) {
                EV << "Packet Loss!!\n";
                delete msg;
                return;
            } else {
                seq++;
                EV <<"Reception of packet :" << seq << " .";
                send(msg,"o2");
            }
        }
    }
}
