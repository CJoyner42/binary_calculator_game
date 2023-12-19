#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <bitset>
#include <random>
#include <iomanip>

using namespace std;

// define a constant for the history file location using the USERPROFILE environment variable
const string HISTORY_FILE = getenv("USERPROFILE") + string("\\Documents\\My Games\\binary calculator\\answer_history.txt");

// collect integer input from the user
int collect_integer_input(const string &prompt)
{
    int input;
    while (true)
    {
        cout << prompt;
        if (cin >> input)
        {
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cerr << "\n*numbers only*" << endl;
    }
    return input;
}

// collect the user's chosen difficulty
int collect_difficulty_input()
{
    cout << "\n-'- { difficulty } -'-\n";
    cout << "\n[1] easy\n";
    cout << "-\n";
    cout << "[2] medium\n";
    cout << "-\n";
    cout << "[3] ???\n";
    cout << "-\n";
    cout << "[4] main menu\n";

    int choice;
    while (true)
    {
        choice = collect_integer_input("\nchoose [1 - 4]: ");
        if (choice >= 1 && choice <= 4)
        {
            break;
        }
        else
        {
            cerr << "\n*invalid selection*" << endl;
        }
    }
    return choice;
}

// display the history of correct answers
void display_history()
{
    ifstream history_file(HISTORY_FILE);
    if (!history_file)
    {
        cout << "*history is empty*" << endl;
        return;
    }

    string line;
    cout << "\n-'- { correct answers } -'-\n";
    cout << "\n";
    while (getline(history_file, line))
    {
        cout << line << endl;
    }
    history_file.close();
}

// save the answer data to the history file
void save_answer(const string &answer_data)
{
    ofstream history_file(HISTORY_FILE, ios::app);
    if (history_file)
    {
        history_file << answer_data << '\n';
        history_file.close();
    }
    else
    {
        cerr << "*error saving answer*" << endl;
    }
}

// start the game
void play()
{
    // collect the user's chosen difficulty
    int selection = collect_difficulty_input();

    // initialize random number generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist;

    // begin game loop
    while (true)
    {
        int decimal_representation;

        // generate a random number based on the selected difficulty
        if (selection == 1)
        {
            decimal_representation = dist(gen) % 100 + 1;
            cout << "\n'" << bitset<7>(decimal_representation) << "' is the binary representation of a random number between 1 and 100." << endl;
        }
        else if (selection == 2)
        {
            decimal_representation = dist(gen) % 10000 + 1;
            cout << "\n'" << bitset<14>(decimal_representation) << "' is the binary representation of a random number between 1 and 10,000." << endl;
        }
        else if (selection == 3)
        {
            decimal_representation = dist(gen) % 1000000 + 1;
            cout << "\n'" << bitset<20>(decimal_representation) << "' is the binary representation of a random number between 1 and 1,000,000." << endl;
        }
        else if (selection == 4)
        {
            // return to main menu
            break;
        }

        // mark the start time for measuring the user's response
        time_t start_time = time(nullptr);

        // prompt the user for their answer
        int answer = collect_integer_input("\nwhat is the decimal representation?: ");

        // check if the user's answer is correct
        if (answer == decimal_representation)
        {
            cout << "\n*correct. the answer is " << decimal_representation << '*' << endl;

            // mark the end time and calculate elapsed time
            time_t end_time = time(nullptr);
            int elapsed_seconds = static_cast<int>(end_time - start_time);
            int minutes = elapsed_seconds / 60;
            int seconds = elapsed_seconds % 60;

            // format the current time and elapsed time
            string current_time = asctime(localtime(&end_time));
            current_time.pop_back();

            stringstream formatted_time;
            formatted_time << put_time(localtime(&end_time), "%b %d, %Y {%H:%M:%S}");

            stringstream time_taken;
            time_taken << setfill('0') << setw(2) << minutes << ":" << setfill('0') << setw(2) << seconds;

            // convert the decimal representation to binary and remove leading zeros
            string binaryRep = bitset<20>(decimal_representation).to_string();
            binaryRep.erase(0, binaryRep.find_first_not_of('0'));

            // construct a string containing answer data and save it to the history file
            string answer_data = to_string(answer) + " (" + binaryRep + ") - " + formatted_time.str() + ", time taken: " + time_taken.str();
            save_answer(answer_data);
            return;
        }
        else
        {
            // report correct answer to the user
            cout << "\n*incorrect. the answer is " << decimal_representation << '*' << endl;
            return;
        }
    }
}

// main menu
int main()
{
    while (true)
    {
        cout << "\n-'- { main menu } -'-\n";
        cout << "\n[1] play game\n";
        cout << "-\n";
        cout << "[2] history\n";
        cout << "-\n";
        cout << "[3] exit\n";

        int choice = collect_integer_input("\nchoose [1 - 3]: ");

        switch (choice)
        {
        case 1:
            play();
            break;
        case 2:
            display_history();
            break;
        case 3:
            cout << "\n-'- -*-  -*- -'-\n";
            cout << "\n";
            return 0;
        default:
            cerr << "\n*invalid selection*" << endl;
        }
    }
}
