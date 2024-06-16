#include "server.h"
#include "../../database/src/inc/menuDatabase.h"
#include "recommendationEngine.h"


int main()
{
//     const std::string &dbHost = "tcp://127.0.0.1", const std::string &user = "prashanth",
//                  const std::string &password = "myroot", const std::string &database = "recommendationEngine");

    std::shared_ptr<DatabaseInterface> database = std::make_shared<MenuDatabase>("tcp://127.0.0.1", "prashanth",
    "myroot", "recommendationEngine");
    std::shared_ptr<IRecommendationEngine> recommendationEngine = std::make_shared<RecommendationEngine>(database);
    std::shared_ptr<Server> server = std::make_shared<Server>(8080, recommendationEngine);

    server->start();

    return 0;
}