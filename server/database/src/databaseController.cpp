#include "databaseController.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

DatabaseController::DatabaseController(const std::string &dbHost, const std::string &user, const std::string &password, const std::string &database)
    : dbHost(dbHost), user(user), password(password), database(database)
{
    connect();
}

void DatabaseController::connect()
{
    try
    {

        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        connection.reset(driver->connect(dbHost, user, password));
        connection->setSchema(database);
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::connect() SQLException: %s\n", e.what());
        exit(1);
    }
}

std::vector<Menu> DatabaseController::fetchMenus()
{
    std::vector<Menu> menus;
    try
    {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT menuId, menu_name, price FROM menu"));

        while (res->next())
        {
            Menu menu;
            menu.menuId = res->getInt("menuId");
            menu.menuName = res->getString("menu_name");
            menu.price = res->getDouble("price");
            menus.push_back(menu);
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::fetchMenus() SQLException: %s\n", e.what());
    }

    return menus;
}

std::vector<Menu> DatabaseController::fetchMenusWithFeedback()
{
    std::vector<Menu> menus = fetchMenus();

    for (Menu &menu : menus)
    {
        fetchFeedbacks(menu);
    }

    return menus;
}

void DatabaseController::fetchFeedbacks(Menu &menu)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement("SELECT feedback_Id, menuId, userId, rating, comment, feedback_date FROM feedback WHERE menuId = ?"));
        preparedStatement->setInt(1, menu.menuId);
        std::unique_ptr<sql::ResultSet> res(preparedStatement->executeQuery());

        while (res->next())
        {
            Feedback feedback;
            feedback.feedbackId = res->getInt("feedback_Id");
            feedback.menuId = res->getInt("menuId");
            feedback.userId = res->getInt("userId");
            feedback.rating = res->getDouble("rating");
            feedback.comment = res->getString("comment");
            feedback.feedbackDate = res->getString("feedback_date");
            menu.feedbacks.push_back(feedback);
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::fetchFeedbacks() SQLException: %s\n", e.what());
    }
}

bool DatabaseController::authenticateUser(int userId, const std::string &password)
{
    try
    {
        printf("DatabaseController::authenticateUser entry with userId %d, password %s\n", userId, password.c_str());
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement("SELECT COUNT(*) AS count FROM users WHERE userId = ? AND password = ?"));
        preparedStatement->setInt(1, userId);
        preparedStatement->setString(2, password);
        std::unique_ptr<sql::ResultSet> res(preparedStatement->executeQuery());

        if (res->next())
        {
            int count = res->getInt("count");
            return (count == 1);
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::authenticateUser() SQLException: %s\n", e.what());
    }

    return false;
}

bool DatabaseController::addUser(const User &user)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement("INSERT INTO users (userId, name, role, password) VALUES (?, ?, ?, ?)"));
        preparedStatement->setInt(1, user.userId);
        preparedStatement->setString(2, user.username);
        preparedStatement->setString(3, user.role);
        preparedStatement->setString(4, user.password);

        int rowsAffected = preparedStatement->executeUpdate();
        if (rowsAffected == 1)
        {
            return true;
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::addUser() SQLException: %s\n", e.what());
    }

    return false;
}

bool DatabaseController::deleteUser(int userId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement("DELETE FROM users WHERE userId = ?"));
        preparedStatement->setInt(1, userId);

        int rowsAffected = preparedStatement->executeUpdate();
        if (rowsAffected == 1)
        {
            return true;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::deleteUser() SQLException: " << e.what() << "\n";
    }

    return false;
}