/* CS230 Course Project
*  
*  This program will offer a 5 option menu that reads data from
*  the csv file given. 
*  
*  Open source code, released under the GNU Public License 
*  Pavlos Constantinou, 20210366@student.act.edu 21/02/2023
*/

#include <iostream>  	// Required for cin, cout, cerr
#include <windows.h> 	// Required for PAUSE (in some C++ compiler versions)
#include <limits> 		// Required for error trapping: numeric_limits<streamsize>::max()
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdio.h> // to delete files
#include <iomanip>
#include <cmath> //to manipulate columns in the 3rd menu option
#include <vector> //for the 4th menu option

using namespace std;

int main()
{

    int option=0;  //menu 
    
    //option 1 variables
	fstream file("top_scorers_Qatar2023.csv");
	string data; //data in each line of the csv
	
	//option 2 variables
	stringstream linestream(data);
  	double threshold; 
  	int numPlayers = 0;
  	
  	
  	remove("constantinou_qatar2023_stats.csv"); //removes file made from menu option 3 
  	
  	
  do		   				
 { 	
 	 cout << "=================================================================" << endl;
	 cout << "USER MENU: TOP SCORER STATISTICS FROM THE FOOTBALL WORLD CUP 2023" << endl;
	 cout << "=================================================================" << endl << endl;
 	 
 	 
     cout << "You may choose from the following options." << endl << endl;	
     cout << "1. Read & display stats for the top goal scorers (from file top_scorers_Qatar2023.csv)" << endl;
     cout << "2. Display shooters with an average distance past a threshold, in alphabetical order" << endl;
     cout << "3. Calculate G/S and NPG/S, save into new file along with the original stats, display" << endl;
     cout << "4. Sort by a field indicated by the user" << endl;
     cout << "5. Exit the program" << endl << endl;
     
     cout << "Please select an option: ";
	 cin >> option;  
     
	 cout << endl;
     
	 cin.sync(); 				// Clears the cin buffer
    
	while(cin.fail()) 			// Error trapping begins
	{
		cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(),'\n'); // helps survive text char input
        cout << "Invalid entry. Please enter a number of the option you want to select: ";
        cin >> option;
        cin.sync(); 			// Clears the cin buffer
    } 
      
      
      
      
    if(option == 1) 
	{
	file.close();
    cout << "You selected option 1!" << endl << "Displaying the content of the csv file..." << endl << endl;
	file.open("top_scorers_Qatar2023.csv", ios::in); //opens csv file
	if (!file) {
        cerr << "Failed to open file" << endl;
        return 1;
    }
	for(int i=0; i<=11; i++) //reads each line (12 in total)
	{
		getline(file,data);
		cout<<data<<"\n";
	}
		cout << endl;
	
		system("pause");
	

    }
    else if(option == 2) 
    {
    file.close();
	cout << "You selected option 2!" << endl << endl;
    file.open("top_scorers_Qatar2023.csv", ios::in);
    if (!file) {
        cerr << "Failed to open file" << endl;
        return 1;
    }
    
    struct Player { //creates structure to put the players name and shot distance
    	string name;
    	double avgShotDistance;
	};
	
    getline(file, data); // ignore header row

    Player players[100]; // assuming no more than 100 players
    while (getline(file, data)) {
        stringstream ss(data);
        string name;
        double avgShotDistance;
        getline(ss, name, ',');
        for (int i = 1; i <= 7; i++) {
            getline(ss, data, ','); // ignore columns 2-7
    }
        getline(ss, data, ','); // read the eighth column
        stringstream(data) >> avgShotDistance;
        players[numPlayers].name = name;
        players[numPlayers].avgShotDistance = avgShotDistance;
        numPlayers++;
        
        cout << "Name: " << name << ", Shot Distance: " << avgShotDistance << endl; //prints the player names and their shot distances user prompt
    }

	cout << endl;
	
    cout << "Enter the average shot distance: "; //ask for threshold
    cin >> threshold;
	
	cout << endl;
	
    sort(players, players + numPlayers, [](const Player& a, const Player& b) { //sorts names in alphabetical order
        return a.name < b.name;
    });

    cout << "Players who tend to shoot from " << threshold << " meters or higher:" << endl;
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].avgShotDistance >= threshold) {
            cout << players[i].name << " (" << players[i].avgShotDistance << ")" << endl;
        }
    }
    memset(players, 0, sizeof(players)); //clears the array for repeated use of the menu option
    cout << endl;
    system("pause");
    
}


    else if(option == 3)
    {
        cout << "You selected option 3!" << endl << endl;
        ifstream file("top_scorers_Qatar2023.csv");
        ofstream file2("constantinou_qatar2023_stats.csv"); //added here so the file won't be created before the menu option is selected
		
		
    	bool isFirstLine = true;
    	while (getline(file, data)) {
        // Skip the first line (header row) so string to integer conversion can be made
        if (isFirstLine) {
            isFirstLine = false;
            file2 << data << ",Goals to Shots,Penalty to Shots" << endl; // Add the new column headers to the output CSV file
            continue;
        }
        
        const int MAX_COLUMNS = 11; // maximum number of columns in CSV file
        string columns[MAX_COLUMNS];
        int numColumns = 0;

        // Split the CSV row into individual columns
        stringstream ss(data);
        string column;
        while (getline(ss, column, ',')) {
            columns[numColumns++] = column;
    }

        // Append the new column to the end of the row
        int goals = stoi(columns[3]);
        int penaltyGoals = stoi(columns[4]);
        int shots = stoi(columns[5]);
        double goalsToShots = 0.0;
        double penaltyToShots = 0.0;
        if (shots != 0) {
            goalsToShots = (double)goals / shots; 
            penaltyToShots = (double)penaltyGoals / shots;
        }
		double goalsToShotsRounded = round(goalsToShots * 100.0) / 100.0; // Round the goalsToShots value to the nearest 1st decimal
        double penaltyToShotsRounded = round(penaltyToShots * 100.0) / 100.0; // Round the penaltyToShots value to the nearest 1st decimal

        // Write the updated row to the output CSV file
        file2 << data << "," << fixed << setprecision(2) << goalsToShotsRounded << "," << penaltyToShotsRounded << endl;
        
}

cout << "New CSV file created." << endl;
system("pause");

 
} 
    else if(option == 4) //ran out of time to finalize this option
    {
    	cout << "You selected option 4!" << endl << endl;
    	
    	string filename = "constantinou_qatar2023_stats.csv";
    	ifstream file(filename);
    
	if (!file) {
        cerr << "Failed to open file " << filename << ". Please execute menu option 3." << endl;
        return 1;
    }

	string header;
    if (getline(file, header)) {
        // Ignore the header row
    }

    vector<vector<string>> data;
    string line;
    while (getline(file, line)) {
        vector<string> row;
        istringstream iss(line);
        string val;

        // Read all values and store only columns 3-10
        int colCount = 0;
        while (getline(iss, val, ',') && colCount < 10) {
            colCount++;
            if (colCount >= 3) {
                row.push_back(val);
            }
        }

        data.push_back(row);
    }

    // Ask the user which column to print
    int numCols = data[0].size();
    cout << "Choose a column to print (3-" << numCols << "): ";
    int colIndex;
    cin >> colIndex;
    if (colIndex < 3 || colIndex > numCols) {
        cerr << "Invalid column index" << endl;
        return 1;
    }

     // Ask the user whether to sort in ascending or descending order
    cout << "Sort in ascending (0) or descending (1) order? ";
    int sortOrder;
    cin >> sortOrder;
    if (sortOrder != 0 && sortOrder != 1) {
        cerr << "Invalid sort order" << endl;
        return 1;
    }

    // Sort the data by the chosen column
    sort(data.begin(), data.end(), [=](vector<string> row1, vector<string> row2) {
        string token1 = row1[colIndex - 3];
        string token2 = row2[colIndex - 3];
        if (sortOrder == 0) {
            return token1 < token2;
        } else {
            return token1 > token2;
        }
    });

    // Print the sorted data
    for (vector<string> row : data) {
        for (string val : row) {
            cout << val << ",";
        }
        cout << endl;
    }
    
    } 
    else if(option == 5)
    {
        cout << endl << "Exiting..." << endl << endl;
    } 
    else 		
    {
       cout << "Invalid entry. Please enter \"1\" , \"2\" or \"3\" " << endl << endl;
    }
} while(option != 5);  // Condition of do-while loop
   
   system("PAUSE");
   return 0;
}