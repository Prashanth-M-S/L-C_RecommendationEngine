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
        std::unique_ptr<sql::Statement> statement(connection->createStatement());
        std::unique_ptr<sql::ResultSet> resultSet(statement->executeQuery("SELECT menuId, menu_name, price FROM menu"));

        while (resultSet->next())
        {
            Menu menu;
            menu.menuId = resultSet->getInt("menuId");
            menu.menuName = resultSet->getString("menu_name");
            menu.price = resultSet->getDouble("price");
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
        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            Feedback feedback;
            feedback.feedbackId = resultSet->getInt("feedback_Id");
            feedback.menuId = resultSet->getInt("menuId");
            feedback.userId = resultSet->getInt("userId");
            feedback.rating = resultSet->getDouble("rating");
            feedback.comment = resultSet->getString("comment");
            feedback.feedbackDate = resultSet->getString("feedback_date");
            menu.feedbacks.push_back(feedback);
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::fetchFeedbacks() SQLException: %s\n", e.what());
    }
}

std::string DatabaseController::authenticateUser(int userId, const std::string &password)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement(
            "SELECT role FROM users WHERE userId = ? AND password = ?"));
        preparedStatement->setInt(1, userId);
        preparedStatement->setString(2, password);
        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        if (resultSet->next())
        {
            std::string role = resultSet->getString("role");
            return "STATUS_OK," + role;
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::authenticateUser() SQLException: %s\n", e.what());
    }
    return "ATHENTICATION_FAILURE";
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

        return rowsAffected == 1;
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

        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::deleteUser() SQLException: " << e.what() << "\n";
    }

    return false;
}

bool DatabaseController::addMenu(const MenuData &menu)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("INSERT INTO menu (menu_name, price) VALUES (?, ?)"));
        preparedStatement->setString(1, menu.menuName);
        preparedStatement->setDouble(2, menu.price);
        int rowsAffected = preparedStatement->executeUpdate();

        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::addMenu() SQLException: %s\n", e.what());
    }

    return false;
}

bool DatabaseController::deleteMenu(int menuId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("DELETE FROM menu WHERE menuId = ?"));
        preparedStatement->setInt(1, menuId);

        int rowsAffected = preparedStatement->executeUpdate();
        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::deleteMenu() SQLException: %s\n", e.what());
    }

    return false;
}

bool DatabaseController::insertDailyMenuEntries(const std::vector<DailyMenuEntry> &dailyMenuEntry) 
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("INSERT INTO dailyMenu (menuId, availability, mealCategory, menuDate) VALUES (?, ?, ?, CURDATE())"));

        for (const auto& entry : dailyMenuEntry)
        {
            preparedStatement->setInt(1, entry.menuId);
            preparedStatement->setInt(2, entry.availability);
            preparedStatement->setString(3, entry.mealCategory);
            preparedStatement->executeUpdate();
        }

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::insertDailyMenuEntries() SQLException: " << e.what() << "\n";
    }

    return false;
}
