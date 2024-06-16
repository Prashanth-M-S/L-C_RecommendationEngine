#include "../inc/menuDatabase.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

MenuDatabase::MenuDatabase(const std::string &dbHost, const std::string &user, const std::string &password, const std::string &database)
    : dbHost(dbHost), user(user), password(password), database(database)
{
    connect();
}

void MenuDatabase::connect()
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    connection.reset(driver->connect(dbHost, user, password));
    connection->setSchema(database);
}

std::vector<Menu> MenuDatabase::fetchMenus()
{
    std::vector<Menu> menus;

    std::unique_ptr<sql::Statement> stmt(connection->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT menuId, menuName, price FROM Menu"));

    while (res->next())
    {
        Menu menu;
        menu.menuId = res->getInt("menuId");
        menu.menuName = res->getString("menuName");
        menu.price = res->getDouble("price");
        menus.push_back(menu);
    }

    return menus;
}

std::vector<Menu> MenuDatabase::fetchMenusWithFeedback()
{
    std::vector<Menu> menus = fetchMenus();

    for (Menu &menu : menus)
    {
        fetchFeedbacks(menu);
    }

    return menus;
}

void MenuDatabase::fetchFeedbacks(Menu &menu)
{
    std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement("SELECT feedbackId, userId, rating, comment, date FROM Feedback WHERE menuId = ?"));
    preparedStatement->setInt(1, menu.menuId);
    std::unique_ptr<sql::ResultSet> res(preparedStatement->executeQuery());

    while (res->next())
    {
        Feedback feedback;
        feedback.feedbackId = res->getInt("feedbackId");
        feedback.userId = res->getInt("userId");
        feedback.rating = res->getDouble("rating");
        feedback.comment = res->getString("comment");
        feedback.date = res->getString("date");
        menu.feedbacks.push_back(feedback);
    }
}
