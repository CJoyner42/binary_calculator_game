#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <bitset>
#include <random>
#include <iomanip>

// 1define a constant for the history file location using the USERPROFILE environment variable
const std::string HISTORY_FILE = std::getenv("USERPROFILE") + std::string("\\Documents\\My Games\\binary calculator\\answer_history.txt");

// collect integer input from the user
int collectIntegerInput(const std::string &prompt)
{
    int input;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> input)
        {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "\n*numbers only*" << std::endl;
    }
    return input;
}

// collect the user's chosen difficulty
int collectDifficultyInput()
{
    std::cout << "\n-'- { difficulty } -'-\n";
    std::cout << "\n[1] easy\n";
    std::cout << "-\n";
    std::cout << "[2] medium\n";
    std::cout << "-\n";
    std::cout << "[3] ???\n";
    std::cout << "-\n";
    std::cout << "[4] main menu\n";

    int choice;
    while (true)
    {
        choice = collectIntegerInput("\nchoose [1 - 4]: ");
        if (choice >= 1 && choice <= 4)
        {
            break;
        }
        else
        {
            std::cerr << "\n*invalid selection*" << std::endl;
        }
    }
    return choice;
}

// display the history of correct answers
void displayHistory()
{
    std::ifstream historyFile(HISTORY_FILE);
    if (!historyFile)
    {
        std::cout << "*history is empty*" << std::endl;
        return;
    }

    std::string line;
    std::cout << "\n-'- { correct answers } -'-\n";
    std::cout << "\n";
    while (std::getline(historyFile, line))
    {
        std::cout << line << std::endl;
    }
    historyFile.close();
}

// save the answer data to the history file
void saveAnswer(const std::string &answerData)
{
    std::ofstream historyFile(HISTORY_FILE, std::ios::app);
    if (historyFile)
    {
        historyFile << answerData << '\n';
        historyFile.close();
    }
    else
    {
        std::cerr << "*error saving answer*" << std::endl;
    }
}

// start the game
void play()
{
    // collect the user's chosen difficulty
    int selection = collectDifficultyInput();

    // initialize random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist;

    // begin game loop
    while (true)
    {
        int decimalRepresentation;

        // generate a random number based on the selected difficulty
        if (selection == 1)
        {
            decimalRepresentation = dist(gen) % 100 + 1;
            std::cout << "\n'" << std::bitset<7>(decimalRepresentation) << "' is the binary representation of a random number between 1 and 100." << std::endl;
        }
        else if (selection == 2)
        {
            decimalRepresentation = dist(gen) % 10000 + 1;
            std::cout << "\n'" << std::bitset<14>(decimalRepresentation) << "' is the binary representation of a random number between 1 and 10,000." << std::endl;
        }
        else if (selection == 3)
        {
            decimalRepresentation = dist(gen) % 1000000 + 1;
            std::cout << "\n'" << std::bitset<20>(decimalRepresentation) << "' is the binary representation of a random number between 1 and 1,000,000." << std::endl;
        }
        else if (selection == 4)
        {
            // return to main menu
            break;
        }

        // mark the start time for measuring the user's response
        time_t startTime = time(nullptr);

        // prompt the user for their answer
        int answer = collectIntegerInput("\nwhat is the decimal representation?: ");

        // check if the user's answer is correct
        if (answer == decimalRepresentation)
        {
            std::cout << "\n*correct. the answer is " << decimalRepresentation << '*' << std::endl;

            // mark the end time and calculate elapsed time
            time_t endTime = time(nullptr);
            int elapsedSeconds = static_cast<int>(endTime - startTime);
            int minutes = elapsedSeconds / 60;
            int seconds = elapsedSeconds % 60;

            // format the current time and elapsed time
            std::string currentTime = std::asctime(std::localtime(&endTime));
            currentTime.pop_back();

            std::stringstream formattedTime;
            formattedTime << std::put_time(std::localtime(&endTime), "%b %d, %Y {%H:%M:%S}");

            std::stringstream timeTaken;
            timeTaken << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;

            // convert the decimal representation to binary and remove leading zeros
            std::string binaryRep = std::bitset<20>(decimalRepresentation).to_string();
            binaryRep.erase(0, binaryRep.find_first_not_of('0'));

            // construct a string containing answer data and save it to the history file
            std::string answerData = std::to_string(answer) + " (" + binaryRep + ") - " + formattedTime.str() + ", time taken: " + timeTaken.str();
            saveAnswer(answerData);
            return;
        }
        else
        {
            // report correct answer to the user
            std::cout << "\n*incorrect. the answer is " << decimalRepresentation << '*' << std::endl;
            return;
        }
    }
}

// main menu
int main()
{
    while (true)
    {
        std::cout << "\n-'- { main menu } -'-\n";
        std::cout << "\n[1] play game\n";
        std::cout << "-\n";
        std::cout << "[2] history\n";
        std::cout << "-\n";
        std::cout << "[3] exit\n";

        int choice = collectIntegerInput("\nchoose [1 - 3]: ");

        switch (choice)
        {
        case 1:
            play();
            break;
        case 2:
            displayHistory();
            break;
        case 3:
            std::cout << "\n-'- -*-  -*- -'-\n";
            std::cout << "\n";
            return 0;
        default:
            std::cerr << "\n*invalid selection*" << std::endl;
        }
    }
}
