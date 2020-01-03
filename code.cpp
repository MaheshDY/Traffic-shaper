#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <unistd.h>

using namespace omnetpp;

class Tic9 : public cSimpleModule
{
  private:
    int seq;  // message sequence number
    cOutVector graph;

    cMessage *message;  // message that has to be re-sent on timeout
    cPar *iaTime;
    cMessage *nextmsg =new cMessage("nextmsg") ;

  public:

    virtual cMessage *generateNewMessage();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic9);

void Tic9::initialize()
{
    // Initialize variables.
    seq = 0;
    int i;
    iaTime = &par("iaTime");
            // Generate and send initial message.
        //EV << "Sending Control message\n";

            //message= generateNewMessage();
            //send(message,"out");
            //cMessage *nextmsg = new cMessage(nextmsg);
        scheduleAt(simTime()+iaTime->doubleValue(), nextmsg);
}

void Tic9::handleMessage(cMessage *msg)
{
    message= generateNewMessage();
    send(message,"out");
    graph.record(seq);
    //cMessage *nextmsg = new cMessage(nextmsg);
scheduleAt(simTime()+iaTime->doubleValue(), nextmsg);

if(seq==15)
{
    for(int i=1;i<15;i++)
    {
        message= generateNewMessage();
            send(message,"out");
            graph.record(seq);
    }
    }

}

cMessage *Tic9::generateNewMessage()
{
    // Generate a message with a different name every time.
    char msgname[20];
        sprintf(msgname, "msg-%d", ++seq);
        cMessage *msg = new cMessage(msgname);

        return msg;
}

class Traffic_shaper : public cSimpleModule
{
  private:
    cQueue token;
    cQueue msgg;
    simtime_t nexttoken;

    cMessage *nextToken =new cMessage("nextToken") ;
    int tokensize;
    int msgsize;


  public:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
Define_Module(Traffic_shaper);

void Traffic_shaper::initialize()
{
    nexttoken = 0.4;
    tokensize = 5;
    msgsize = 10;
    scheduleAt(simTime()+nexttoken, nextToken);

}

void Traffic_shaper::handleMessage(cMessage *msg)
{
    if(msg==nextToken)
    //if(strcmp("nextToken",msg->getName()))
    {
        EV<<"This is IF"<<"\n";

         nextToken =new cMessage("nextToken") ;
                scheduleAt(simTime()+nexttoken, nextToken);

        if(token.getLength()<tokensize)
        token.insert(msg);
          if(msgg.getLength()>0)
          {
              msg = (cMessage *) msgg.pop();
              send(msg,"out");
              token.pop();
          }
    }
    else
    {
        if(msgg.getLength()< msgsize)
         msgg.insert(msg);
        if(token.getLength()>0)
                  {
                       msg = (cMessage *) msgg.pop();
                       send(msg,"out");
                      token.pop();
                  }
    }

}

class Toc9 : public cSimpleModule
{
private:
    int count=0;
  public:
    virtual void handleMessage(cMessage *msg) override;
    cOutVector graph2;
};
Define_Module(Toc9);
void Toc9::handleMessage(cMessage *msg)
{
    count++;
    graph2.record(count);
    delete msg;

}
