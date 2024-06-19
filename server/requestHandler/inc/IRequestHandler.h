#ifndef IREQUESTHANDLER_H
#define IREQUESTHANDLER_H

class IRequestHandler 
{
public:
    virtual ~IRequestHandler() {}
    virtual void handleRequest(int newSocket) = 0;
};

#endif