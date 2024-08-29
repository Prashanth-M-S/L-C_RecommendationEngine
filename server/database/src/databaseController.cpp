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

bool DatabaseController::addMenu(const MenuAttributes &menuData)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "INSERT INTO menu (menu_name, price, diet_type, spice_level, cuisine_type, sweet_type) VALUES (?, ?, ?, ?, ?, ?)"));

        preparedStatement->setString(1, menuData.menuName);
        preparedStatement->setDouble(2, menuData.price);
        preparedStatement->setString(3, menuData.dietType);
        preparedStatement->setString(4, menuData.spiceLevel);
        preparedStatement->setString(5, menuData.cuisineType);
        preparedStatement->setString(6, menuData.sweetType);

        int rowsAffected = preparedStatement->executeUpdate();
        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::addMenu() SQLException: " << e.what() << "\n";
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

        for (const auto &entry : dailyMenuEntry)
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

std::vector<DailyMenuAttributes> DatabaseController::getDailyMenu()
{
    std::vector<DailyMenuAttributes> dailyMenu;

    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "SELECT dm.dailyMenuId, m.menu_name AS itemName, dm.availability, dm.mealCategory, m.price "
                "FROM dailyMenu dm "
                "JOIN menu m ON dm.menuId = m.menuId "
                "WHERE dm.menuDate = CURDATE()"));

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            DailyMenuAttributes entry;
            entry.dailyMenuId = resultSet->getInt("dailyMenuId");
            entry.itemName = resultSet->getString("itemName");
            entry.availability = resultSet->getInt("availability");
            entry.mealCategory = resultSet->getString("mealCategory");
            entry.price = resultSet->getDouble("price");
            dailyMenu.push_back(entry);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::getDailyMenu() SQLException: " << e.what() << "\n";
    }

    return dailyMenu;
}

bool DatabaseController::insertUserOrderEntries(const std::vector<UserOrderEntry> &userOrderEntries)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("INSERT INTO userOrder (userId, dailyMenuId) VALUES (?, ?)"));

        for (const auto &entry : userOrderEntries)
        {
            preparedStatement->setInt(1, entry.userId);
            preparedStatement->setInt(2, entry.dailyMenuId);
            preparedStatement->executeUpdate();
        }

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::insertUserOrderEntries() SQLException: " << e.what() << "\n";
    }

    return false;
}

int DatabaseController::getMenuIdFromDailyMenuId(int dailyMenuId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("SELECT menuId FROM dailyMenu WHERE dailyMenuId = ?"));
        preparedStatement->setInt(1, dailyMenuId);

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        if (resultSet->next())
        {
            return resultSet->getInt("menuId");
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::getMenuIdFromDailyMenuId() SQLException: " << e.what() << "\n";
    }

    return -1;
}

bool DatabaseController::insertUserFeedback(const Feedback &feedback)
{
    int menuId = getMenuIdFromDailyMenuId(feedback.menuId);
    if (menuId == -1)
    {
        std::cerr << "DatabaseController::insertUserFeedback() Error: Invalid dailyMenuId\n";
        return false;
    }

    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "INSERT INTO feedback (menuId, userId, rating, comment, feedback_date) VALUES (?, ?, ?, ?, CURDATE())"));

        preparedStatement->setInt(1, menuId);
        preparedStatement->setInt(2, feedback.userId);
        preparedStatement->setDouble(3, feedback.rating);
        preparedStatement->setString(4, feedback.comment);

        int rowsAffected = preparedStatement->executeUpdate();
        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::insertUserFeedback() SQLException (Insert feedback): " << e.what() << "\n";
    }

    return false;
}

bool DatabaseController::insertNotification(const std::string &message)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("INSERT INTO notification (message) VALUES (?)"));
        preparedStatement->setString(1, message);
        int rowsAffected = preparedStatement->executeUpdate();
        return rowsAffected == 1;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::insertNotification() SQLException: " << e.what() << "\n";
        return false;
    }
}

bool DatabaseController::addNotification(const std::string &message)
{
    try
    {
        connection->setAutoCommit(false);

        std::unique_ptr<sql::PreparedStatement> insertNotificationStmt(
            connection->prepareStatement("INSERT INTO notification (message) VALUES (?)"));
        insertNotificationStmt->setString(1, message);
        insertNotificationStmt->executeUpdate();

        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID() AS notificationId"));
        int notificationId;
        if (res->next())
        {
            notificationId = res->getInt("notificationId");
        }

        std::unique_ptr<sql::PreparedStatement> insertUserNotificationStatusStmt(
            connection->prepareStatement("INSERT INTO userNotificationStatus (userId, notificationId, viewedStatus) SELECT userId, ?, 'not_viewed' FROM users"));
        insertUserNotificationStatusStmt->setInt(1, notificationId);
        insertUserNotificationStatusStmt->executeUpdate();

        connection->commit();
        connection->setAutoCommit(true);

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::addNotification() SQLException: " << e.what() << "\n";
        connection->rollback();
        connection->setAutoCommit(true);
    }

    return false;
}

std::vector<Notification> DatabaseController::getNonViewedNotificationsForUser(int userId)
{
    std::vector<Notification> notifications;

    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "SELECT n.notificationId, n.message, n.dateUpdated "
                "FROM notification n "
                "JOIN userNotificationStatus uvs ON n.notificationId = uvs.notificationId "
                "WHERE uvs.userId = ? AND uvs.viewedStatus = 'not_viewed'"));
        preparedStatement->setInt(1, userId);

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            Notification notification;
            notification.notificationId = resultSet->getInt("notificationId");
            notification.message = resultSet->getString("message");
            notification.dateUpdated = resultSet->getString("dateUpdated");
            notifications.push_back(notification);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::getNonViewedNotificationsForUser() SQLException: " << e.what() << "\n";
    }

    return notifications;
}

bool DatabaseController::markNotificationsAsViewed(int userId, const std::vector<int> &notificationIds)
{
    printf("DatabaseController::markNotificationsAsViewed entry ");
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "UPDATE userNotificationStatus SET viewedStatus = 'viewed' WHERE userId = ? AND notificationId = ?"));

        for (const int &notificationId : notificationIds)
        {
            preparedStatement->setInt(1, userId);
            preparedStatement->setInt(2, notificationId);
            preparedStatement->executeUpdate();
        }

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::markNotificationsAsViewed() SQLException: " << e.what() << "\n";
    }

    return false;
}

UserProfile DatabaseController::fetchUserProfile(int userId)
{
    UserProfile profile;
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(connection->prepareStatement(
            "SELECT preferenceType, spiceLevel, cuisinePreference, sweetTooth FROM users WHERE userId = ?"));
        preparedStatement->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        if (resultSet->next())
        {
            profile.userId = userId;
            profile.preferenceType = resultSet->getString("preferenceType");
            profile.spiceLevel = resultSet->getString("spiceLevel");
            profile.cuisinePreference = resultSet->getString("cuisinePreference");
            profile.sweetTooth = resultSet->getString("sweetTooth");
        }
    }
    catch (sql::SQLException &e)
    {
        printf("DatabaseController::fetchUserProfile() SQLException: %s\n", e.what());
    }
    return profile;
}

std::vector<DailyMenuAttributes> DatabaseController::getDailyMenuWithAttributes()
{
    std::vector<DailyMenuAttributes> dailyMenu;

    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "SELECT dm.dailyMenuId, m.menu_name, dm.availability, dm.mealCategory, m.price, "
                "m.diet_type, m.spice_level, m.cuisine_type, m.sweet_type "
                "FROM dailyMenu dm "
                "JOIN menu m ON dm.menuId = m.menuId "
                "WHERE dm.menuDate = CURDATE()"));

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            DailyMenuAttributes entry;
            entry.dailyMenuId = resultSet->getInt("dailyMenuId");
            entry.menuName = resultSet->getString("menu_name");
            entry.availability = resultSet->getInt("availability");
            entry.mealCategory = resultSet->getString("mealCategory");
            entry.price = resultSet->getDouble("price");
            entry.dietType = resultSet->getString("diet_type");
            entry.spiceLevel = resultSet->getString("spice_level");
            entry.cuisineType = resultSet->getString("cuisine_type");
            entry.sweetType = resultSet->getString("sweet_type");

            dailyMenu.push_back(entry);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::getDailyMenuWithAttributes() SQLException: " << e.what() << "\n";
    }

    return dailyMenu;
}

bool DatabaseController::updateUserProfile(const UserProfile &profile)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "UPDATE users SET preferenceType = ?, spiceLevel = ?, cuisinePreference = ?, sweetTooth = ? WHERE userId = ?"));

        preparedStatement->setString(1, profile.preferenceType);
        preparedStatement->setString(2, profile.spiceLevel);
        preparedStatement->setString(3, profile.cuisinePreference);
        preparedStatement->setString(4, profile.sweetTooth);
        preparedStatement->setInt(5, profile.userId);

        int updatedRows = preparedStatement->executeUpdate();
        return updatedRows > 0;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::updateUserProfile() SQLException: " << e.what() << "\n";
        return false;
    }
}

UserProfile DatabaseController::getUserProfile(int userId)
{
    UserProfile userProfile;

    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement(
                "SELECT userId, preferenceType, spiceLevel, cuisinePreference, sweetTooth FROM users WHERE userId = ?"));

        preparedStatement->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        if (resultSet->next())
        {
            userProfile.userId = resultSet->getInt("userId");
            userProfile.preferenceType = resultSet->getString("preferenceType");
            userProfile.spiceLevel = resultSet->getString("spiceLevel");
            userProfile.cuisinePreference = resultSet->getString("cuisinePreference");
            userProfile.sweetTooth = resultSet->getString("sweetTooth");
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::getUserProfile() SQLException: " << e.what() << "\n";
    }

    return userProfile;
}

bool DatabaseController::setDailyMenuAvailabilityToZero(int dailyMenuId)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("UPDATE dailyMenu SET availability = 0 WHERE dailyMenuId = ?"));

        preparedStatement->setInt(1, dailyMenuId);
        preparedStatement->executeUpdate();

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::setDailyMenuAvailabilityToZero() SQLException: " << e.what() << "\n";
    }

    return false;
}

bool DatabaseController::addFeedbackQuestion(const std::string &questionText)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("INSERT INTO FeedbackQuestion (question_text) VALUES (?)"));

        preparedStatement->setString(1, questionText);
        preparedStatement->executeUpdate();

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::addFeedbackQuestion() SQLException: " << e.what() << "\n";
    }

    return false;
}

std::vector<std::pair<int, std::string>> DatabaseController::fetchFeedbackQuestions()
{
    std::vector<std::pair<int, std::string>> questions;
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("SELECT question_id, question_text FROM FeedbackQuestion"));

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            int questionId = resultSet->getInt("question_id");
            std::string questionText = resultSet->getString("question_text");
            questions.emplace_back(questionId, questionText);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::fetchFeedbackQuestions() SQLException: " << e.what() << "\n";
    }

    return questions;
}

bool DatabaseController::storeFeedbackAnswers(const std::vector<FeedbackAnswer> &feedbackAnswers)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> getMenuIdStatement(
            connection->prepareStatement("SELECT menuId FROM dailyMenu WHERE dailyMenuId = ?"));

        std::unique_ptr<sql::PreparedStatement> insertFeedbackStatement(
            connection->prepareStatement("INSERT INTO FeedbackAnswer (question_id, menuId, userId, answer_text) VALUES (?, ?, ?, ?)"));

        for (const auto &answer : feedbackAnswers)
        {
            getMenuIdStatement->setInt(1, answer.foodId);
            std::unique_ptr<sql::ResultSet> resultSet(getMenuIdStatement->executeQuery());

            if (resultSet->next())
            {
                int menuId = resultSet->getInt("menuId");

                insertFeedbackStatement->setInt(1, answer.questionId);
                insertFeedbackStatement->setInt(2, menuId);
                insertFeedbackStatement->setInt(3, answer.employeeId);
                insertFeedbackStatement->setString(4, answer.answerText);
                insertFeedbackStatement->executeUpdate();
            }
            else
            {
                std::cerr << "DatabaseController::storeFeedbackAnswers() Error: Invalid dailyMenuId " << answer.foodId << "\n";
                return false;
            }
        }

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::storeFeedbackAnswers() SQLException: " << e.what() << "\n";
    }

    return false;
}

std::vector<FeedbackAnswer> DatabaseController::fetchSuggestionsForMenu(int menuId)
{
    std::vector<FeedbackAnswer> feedbackAnswers;
    try
    {
        std::unique_ptr<sql::PreparedStatement> preparedStatement(
            connection->prepareStatement("SELECT question_id, answer_text, userId FROM FeedbackAnswer WHERE menuId = ?"));
        preparedStatement->setInt(1, menuId);

        std::unique_ptr<sql::ResultSet> resultSet(preparedStatement->executeQuery());

        while (resultSet->next())
        {
            FeedbackAnswer answer;
            answer.questionId = resultSet->getInt("question_id");
            answer.answerText = resultSet->getString("answer_text");
            answer.employeeId = resultSet->getInt("userId");

            feedbackAnswers.push_back(answer);
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "DatabaseController::fetchFeedbackAnswersForMenu() SQLException: " << e.what() << "\n";
    }

    return feedbackAnswers;
}
