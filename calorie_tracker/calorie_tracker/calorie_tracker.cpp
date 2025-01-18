#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Structure to store food or workout entries by date
struct LogEntry {
    string date;
    vector<pair<string, double>> entries; // Pairs of name and calories (or burned calories)
    double totalCalories = 0.0;
};

// Structure to store user data
struct User {
    string username;
    string password;
    int age;
    string gender;
    double weight;
    double height;
    string goal; // "lose", "maintain", or "gain"
    double activityLevel; // activity coefficient
    string accountType; // "Standard" or "Premium"
    double bmr; // BMR value
    double calorieIntake; // Daily calorie intake
    double deficitOrSurplus; // Calorie deficit or surplus
    double proteinGrams; // Grams of protein per day
    double fatGrams; // Grams of fat per day
    double carbGrams; // Grams of carbohydrates per day
    vector<LogEntry> foodLog; // Food log entries
    vector<LogEntry> workoutLog;// Workout log entries

    User()
        : age(0), weight(0.0), height(0.0), activityLevel(1.2), bmr(0.0),
        calorieIntake(0.0), deficitOrSurplus(0), proteinGrams(0.0),
        fatGrams(0.0), carbGrams(0.0) {}
};



// Vector to store users
vector<User> users;

// Function to find a log entry by date
vector<pair<string, double>>* findLogByDate(vector<LogEntry>& logs, const string& date) {
    for (LogEntry& log : logs) {
        if (log.date == date) {
            return &log.entries;
        }
    }
    return nullptr; // Return null if no entry is found
}

// Function to load data from file
void loadDataFromFile() {
    ifstream file("users_data.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            User user;
            file >> user.username >> user.password >> user.age >> user.gender
                >> user.weight >> user.height >> user.goal >> user.activityLevel
                >> user.accountType >> user.deficitOrSurplus >> user.proteinGrams
                >> user.fatGrams >> user.carbGrams;
            if (file.fail()) break;

            // Calculation of BMR and calorieIntake after loading
            user.bmr = (user.gender == "male")
                ? (88.362 + (13.397 * user.weight) + (4.799 * user.height) - (5.677 * user.age))
                : (447.593 + (9.247 * user.weight) + (3.098 * user.height) - (4.330 * user.age));

            user.calorieIntake = user.bmr * user.activityLevel + user.deficitOrSurplus;

            size_t foodLogSize;
            file >> foodLogSize;
            for (size_t i = 0; i < foodLogSize; ++i) {
                string date;
                size_t foodCount;
                file >> date >> foodCount;
                vector<pair<string, double>> foodList;
                for (size_t j = 0; j < foodCount; ++j) {
                    string foodName;
                    double calories;
                    file >> foodName >> calories;
                    foodList.push_back(make_pair(foodName, calories));
                }
                user.foodLog.push_back({ date, foodList });
            }

            size_t workoutLogSize;
            file >> workoutLogSize;
            for (size_t i = 0; i < workoutLogSize; ++i) {
                string date;
                size_t workoutCount;
                file >> date >> workoutCount;
                vector<pair<string, double>> workoutList;
                for (size_t j = 0; j < workoutCount; ++j) {
                    string workoutName;
                    double caloriesBurned;
                    file >> workoutName >> caloriesBurned;
                    workoutList.push_back(make_pair(workoutName, caloriesBurned));
                }
                user.workoutLog.push_back({ date, workoutList });
            }

            users.push_back(user);
        }
        file.close();
    }
}


void updateUser(User& user) {
    int choice;
    do {
        cout << "\n--- Update User Data ---" << endl;
        cout << "1. Update Weight" << endl;
        cout << "2. Update Height" << endl;
        cout << "3. Update Goal" << endl;
        cout << "4. Update Activity Level" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter new weight (kg): ";
            cin >> user.weight;
            break;
        }
        case 2: {
            cout << "Enter new height (cm): ";
            cin >> user.height;
            break;
        }
        case 3: {
            cout << "Enter new goal (lose/maintain/gain): ";
            cin >> user.goal;
            break;
        }
        case 4: {
            cout << "Enter number of active days per week: ";
            int activeDays;
            cin >> activeDays;
            if (activeDays <= 0) {
                user.activityLevel = 1.2;
            }
            else if (activeDays <= 3) {
                user.activityLevel = 1.375;
            }
            else if (activeDays <= 5) {
                user.activityLevel = 1.55;
            }
            else if (activeDays <= 6) {
                user.activityLevel = 1.725;
            }
            else {
                user.activityLevel = 1.9;
            }
            break;
        }
        case 5:
            cout << "Returning to main menu...\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        // Recalculate BMR and calorieIntake after the change
        user.bmr = (user.gender == "male")
            ? (88.362 + (13.397 * user.weight) + (4.799 * user.height) - (5.677 * user.age))
            : (447.593 + (9.247 * user.weight) + (3.098 * user.height) - (4.330 * user.age));
        user.calorieIntake = user.bmr * user.activityLevel + user.deficitOrSurplus;

        cout << "Data updated successfully!\n";
    } while (choice != 5);
}



// Function to check username uniqueness
bool isUsernameUnique(const string& username) {
    for (const User& user : users) {
        if (user.username == username) {
            return false;
        }
    }
    return true;
}

void displayUserData(const User& user) {
    cout << "\n--- User Data ---" << endl;
    cout << "Username: " << user.username << endl;
    cout << "Age: " << user.age << endl;
    cout << "Gender: " << user.gender << endl;
    cout << "Weight: " << user.weight << " kg" << endl;
    cout << "Height: " << user.height << " cm" << endl;
    cout << "Goal: " << user.goal << endl;
    cout << "Activity Level: " << user.activityLevel << endl;
    cout << "Calorie Intake: " << user.calorieIntake << " kcal" << endl;
   
    if (user.accountType == "Premium") {
        cout << "Protein: " << user.proteinGrams << " g" << endl;
        cout << "Fat: " << user.fatGrams << " g" << endl;
        cout << "Carbohydrates: " << user.carbGrams << " g" << endl;
    }
    else {
        cout << "Upgrade to Premium to view macronutrient details." << endl;
    }
}

// Function to log in a user
void loginUser() {
    string username, password;
    cout << "\nUser Login" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (User& user : users) {
        if (user.username == username && user.password == password) {
            cout << "\nLogin successful! Welcome, " << username << "!" << endl;
            displayUserData(user);
            return;
        }
    }
    cout << "\nInvalid username or password!" << endl;
}

// Function to save data to file
void saveDataToFile() {
    ofstream file("users_data.txt");
    if (file.is_open()) {
        for (const User& user : users) {
            file << user.username << " " << user.password << " " << user.age << " "
                << user.gender << " " << user.weight << " " << user.height << " "
                << user.goal << " " << user.activityLevel << " " << user.accountType << " "
                << user.deficitOrSurplus << " " << user.proteinGrams << " "
                << user.fatGrams << " " << user.carbGrams << "\n";

            // Recording the food diary
            file << user.foodLog.size() << "\n";
            for (const LogEntry& log : user.foodLog) {
                file << log.date << " " << log.entries.size() << " ";
                for (const std::pair<std::string, double>& entry : log.entries) {
                    file << entry.first << " " << entry.second << " ";
                }
                file << "\n";
            }

            // Recording the training log
            file << user.workoutLog.size() << "\n";
            for (const auto& log : user.workoutLog) {
                file << log.date << " " << log.entries.size() << " ";
                for (const std::pair<std::string, double>& entry : log.entries) {
                    file << entry.first << " " << entry.second << " ";
                }
                file << "\n";
            }
        }
        file.close();
    }
    else {
        cout << "Error: Could not open file for saving data!" << endl;
    }
}


// Function to register a new user
void registerUser() {
    User newUser;
    cout << "\nRegister New User" << endl;
    do {
        cout << "Enter username: ";
        cin >> newUser.username;
        if (!isUsernameUnique(newUser.username)) {
            cout << "Username already exists. Please choose another." << endl;
        }
    } while (!isUsernameUnique(newUser.username));

    cout << "Enter password: ";
    cin >> newUser.password;
    cout << "Enter age: ";
    cin >> newUser.age;
    cout << "Enter gender (male/female): ";
    cin >> newUser.gender;
    cout << "Enter weight (kg): ";
    cin >> newUser.weight;
    cout << "Enter height (cm): ";
    cin >> newUser.height;
    cout << "Enter goal (lose/maintain/gain): ";
    cin >> newUser.goal;

    int activeDays;
    cout << "Enter number of active days per week: ";
    cin >> activeDays;
    if (activeDays <= 0) {
        newUser.activityLevel = 1.2;
    }
    else if (activeDays <= 3) {
        newUser.activityLevel = 1.375;
    }
    else if (activeDays <= 5) {
        newUser.activityLevel = 1.55;
    }
    else if (activeDays <= 6) {
        newUser.activityLevel = 1.725;
    }
    else {
        newUser.activityLevel = 1.9;
    }

    newUser.bmr = (newUser.gender == "male")
        ? (88.362 + (13.397 * newUser.weight) + (4.799 * newUser.height) - (5.677 * newUser.age))
        : (447.593 + (9.247 * newUser.weight) + (3.098 * newUser.height) - (4.330 * newUser.age));

    cout << "\nChoose rate of change (0.00/0.25/0.5/0.75/1 kg per week): ";
    double rate;
    cin >> rate;
    newUser.deficitOrSurplus = (newUser.goal == "maintain") ? rate * 0 : (newUser.goal == "lose") ? rate * -1100 : (newUser.goal == "gain") ? rate * 1100 : 0;

    newUser.calorieIntake = (newUser.bmr * newUser.activityLevel) + newUser.deficitOrSurplus;
    newUser.proteinGrams = (newUser.calorieIntake * 0.25) / 4;
    newUser.fatGrams = (newUser.calorieIntake * 0.30) / 9;
    newUser.carbGrams = (newUser.calorieIntake * 0.45) / 4;

    cout << "\nChoose account type (Standard/Premium): ";
    cin >> newUser.accountType;
    while (newUser.accountType != "Standard" && newUser.accountType != "Premium") {
        cout << "Invalid account type. Please enter 'Standard' or 'Premium': ";
        cin >> newUser.accountType;
    }


    users.push_back(newUser);
    saveDataToFile();
    cout << "\nUser registered successfully!" << endl;

    displayUserData(newUser);

}



void addFood(User& user) {
    string date, foodName;
    double calories;

    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter food name: ";
    cin >> foodName;
    cout << "Enter calories: ";
    cin >> calories;

    bool found = false;
    for (LogEntry& log : user.foodLog) {
        if (log.date == date) {
            log.entries.push_back({ foodName, calories });
            log.totalCalories += calories; // Summary Calorie Update
            found = true;
            break;
        }
    }

    if (!found) {
        LogEntry newLog;
        newLog.date = date;
        newLog.entries.push_back({ foodName, calories });
        newLog.totalCalories = calories;
        user.foodLog.push_back(newLog);
    }

    cout << "Food added successfully!\n";
    saveDataToFile();
}



void addWorkout(User& user) {
    string date, workoutName;
    double caloriesBurned;

    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter workout name: ";
    cin >> workoutName;
    cout << "Enter calories burned: ";
    cin >> caloriesBurned;

    bool found = false;
    for (LogEntry& log : user.workoutLog) {
        if (log.date == date) {
            log.entries.push_back({ workoutName, caloriesBurned });
            log.totalCalories += caloriesBurned; // Summary Calorie Update
            found = true;
            break;
        }
    }

    if (!found) {
        LogEntry newLog;
        newLog.date = date;
        newLog.entries.push_back({ workoutName, caloriesBurned });
        newLog.totalCalories = caloriesBurned;
        user.workoutLog.push_back(newLog);
    }

    cout << "Workout added successfully!\n";
    saveDataToFile();
}



void dailyReport(const User& user) {
    string date;
    double totalFoodCalories = 0.0;
    double totalWorkoutCalories = 0.0;

    cout << "\n--- Daily Report ---" << endl;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;

    // Summing up the calories from food for a given date
    for (const LogEntry& log : user.foodLog) {
        if (log.date == date) {
            for (const std::pair<std::string,double>& entry : log.entries) {
                totalFoodCalories += entry.second;
            }
            break;
        }
    }

    // Summing up the calories from workouts for a given date
    for (const LogEntry& log : user.workoutLog) {
        if (log.date == date) {
            for (const std::pair<std::string, double>& entry : log.entries) {
                totalWorkoutCalories += entry.second;
            }
            break;
        }
    }

    // Calculating the daily calorie balance
    double netCalories = totalFoodCalories - totalWorkoutCalories;

    // Show the report
    cout << "\nReport for " << date << ":" << endl;
    cout << "Total food calories: " << totalFoodCalories << " kcal" << endl;
    cout << "Total workout calories: " << totalWorkoutCalories << " kcal" << endl;
    cout << "Net calorie balance: " << netCalories << " kcal" << endl;

    // Comparison with daily calorie needs
    const double EPSILON = 1e-9;
    double dailyCalorieNeeds = user.bmr * user.activityLevel + user.deficitOrSurplus;
    cout << "Daily calorie needs: " << dailyCalorieNeeds << " kcal" << endl;

    if (fabs(netCalories - dailyCalorieNeeds)>EPSILON) {
        cout << "Status: You are in a calorie surplus of " << (netCalories - dailyCalorieNeeds) << " kcal." << endl;
    }
    else if (fabs(netCalories - dailyCalorieNeeds)<EPSILON) {
        cout << "Status: You are in a calorie deficit of " << (dailyCalorieNeeds - netCalories) << " kcal." << endl;
    }
    else {
        cout << "Status: You have maintained your calories perfectly!" << endl;
    }

    if (user.accountType == "Premium") {
        cout << "\n--- Macronutrient Breakdown ---" << endl;
        cout << "Protein: " << user.proteinGrams << " g" << endl;
        cout << "Fat: " << user.fatGrams << " g" << endl;
        cout << "Carbohydrates: " << user.carbGrams << " g" << endl;
    }
    else {
        cout << "\nUpgrade to Premium to view detailed macronutrient information." << endl;
    }
}

int getValidMenuChoice() {
    int choice;
    do {
        cout << "\nChoose an option: ";
        cin >> choice;

        // Validity check
        if (cin.fail() || choice < 1 || choice > 6) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clearing the buffer
            cout << "Invalid input. Please enter a number between 1 and 6." << endl;
        }
        else {
            break; // Entry is valid
        }
    } while (true);

    return choice;
}


// Main menu
void mainMenu() {
    int choice;
    loadDataFromFile();
    do {
        cout << "\n--- Main Menu ---" << endl;
        cout << "1. Register User" << endl;
        cout << "2. Login User" << endl;
        cout << "3. Add Food" << endl;
        cout << "4. Add Workout" << endl;
        cout << "5. Daily Report" << endl;
        cout << "6. Update User Data" << endl;
        cout << "0. Exit" << endl;
        
        choice = getValidMenuChoice();

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
            if (!users.empty()) {
                string username;
                cout << "Enter username: ";
                cin >> username;
                bool userFound = false;
                for (User& user : users) {
                    if (user.username == username) {
                        addFood(user);
                        userFound = true;
                        break;
                    }
                }
                if (!userFound) {
                    cout << "User not found!" << endl;
                }
            }
            else {
                cout << "No users registered yet!" << endl;
            }
            break;

        case 4:

            if (!users.empty()) {
                string username;
                cout << "Enter username: ";
                cin >> username;
                bool userFound = false;
                for (User& user : users) {
                    if (user.username == username) {
                        addWorkout(user);
                        userFound = true;
                        break;
                    }
                }
                if (!userFound) {
                    cout << "User not found!" << endl;
                }
            }
            else {
                cout << "No users registered yet!" << endl;
            }
            break;

        
        case 5:
            if (!users.empty()) {
                string username;
                cout << "Enter username: ";
                cin >> username;
                bool userFound = false;
                for (const User& user : users) {
                    if (user.username == username) {
                        dailyReport(user);
                        userFound = true;
                        break;
                    }
                }
                if (!userFound) {
                    cout << "User not found!" << endl;
                }
            }
            else {
                cout << "No users registered yet!" << endl;
            }
            break;

        case 6:
            if (!users.empty()) {
                string username;
                cout << "Enter username: ";
                cin >> username;

                bool userFound = false;
                for (User& user : users) {
                    if (user.username == username) {
                        updateUser(user);
                        saveDataToFile(); 
                        userFound = true;
                        break;
                    }
                }

                if (!userFound) {
                    cout << "User not found!" << endl;
                }
            }
            else {
                cout << "No users registered yet!" << endl;
            }
            break;

        case 0:
            cout << "\nExiting the program. Goodbye!" << endl;
            break;

        default:
            cout << "\nInvalid choice! Try again." << endl;
        }
    } while (choice != 0);
}

// Main function
int main() {
    cout << "Welcome to the Calorie Tracking Program!" << endl;
    mainMenu();
    return 0;
}
