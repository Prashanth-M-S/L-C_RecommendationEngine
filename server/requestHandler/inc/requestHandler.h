#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "IRequestHandler.h"
#include "IDatabaseController.h"
#include "IRecommendationEngine.h"
#include "dataParser.h"
#include <memory>

class RequestHandler : public IRequestHandler 
{
public:
    RequestHandler(std::shared_ptr<IDatabaseController> database, std::shared_ptr<IRecommendationEngine> recommendationEngine);
    void handleRequest(int new_socket) override;

private:
    std::shared_ptr<IDatabaseController> database;
    std::shared_ptr<IRecommendationEngine> recommendationEngine;
    std::shared_ptr<DataParser> dataParser; 

    std::string readFromSocket(int new_socket);
    std::string processRequest(const GeneralRequest &request);
    std::string handleLoginRequest(const std::string &data);
    std::string handleGetRecommendedFoodRequest();
    std::string handleAddUserRequest(const std::string &data);
    std::string handleDelUserRequest(const std::string &data);
    std::string handleAddMenuRequest(const std::string &data);
    std::string handleDelMenuRequest(const std::string &data);
};

#endif 
