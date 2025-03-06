/*
Muhammad Usman
22i - 2618
Assignment#4
*/

// This line disables certain compiler warnings related to the use of standard C functions.
#define _CRT_SECURE_NO_WARNINGS

// Header files for various C++ library components and operating system functions
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <fstream>
#include <thread>
#include <cstdlib>

using namespace std;

// Declare a class called Catalog
class Catalog
{
private:
	// Private member variables for item name, item price, and quantity
	string itemName;
	int itemPrice;
	int quantity;

public:
	// Default constructor for Catalog class
	Catalog()
	{
	}

	// Constructor for Catalog class that takes arguments for item name, price, and quantity
	Catalog(string item, int itemPrice, int quantity)
	{
		// Set the values of the private member variables based on the arguments passed in
		this->itemName = item;
		this->itemPrice = itemPrice;
		this->quantity = quantity;
	}

	// Setters and getters for the private member variables
	void setItem(string item)
	{
		this->itemName = item;
	}

	void setPriceItem(int itemPrice)
	{
		this->itemPrice = itemPrice;
	}

	string getItem()
	{
		return itemName;
	}

	double getPriceItem()
	{
		return itemPrice;
	}

	void setQuantity(int quantity)
	{
		this->quantity = quantity;
	}

	int getQuantity()
	{
		return quantity;
	}
};

// Declare a class called Inventory that inherits from Catalog
class Inventory : public Catalog
{
public:
	// Default constructor for Inventory class
	Inventory()
	{
	}

	// Constructor for Inventory class that takes arguments for item name, price, and quantity
	Inventory(string item, int itemPrice, int quantity)
	{
		// Use setters from the Catalog class to set the values of the private member variables
		setItem(item);
		setPriceItem(itemPrice);
		setQuantity(quantity);
	}

	// Method for searching the inventory for a specific product
	bool searchInventory(const string &input_file, const string &product)
	{
		// Open the input file for reading
		ifstream infile(input_file);

		// Declare a string variable to hold each line of the file as it's read in
		string line;

		// Declare a boolean variable to keep track of whether or not the product was found in the inventory
		bool product_found = false;

		// Loop through each line in the file
		while (getline(infile, line))
		{
			// Declare an array of strings to hold the individual words in the line
			string words[3];

			// Declare a string variable to hold each word as it's read in
			string word;

			// Declare an integer variable to keep track of which index of the words array we're currently populating
			int index = 0;

			// Use an istringstream object to split the line into individual words
			istringstream iss(line);

			// Loop through each word in the line and add it to the words array
			while (iss >> word)
			{
				words[index] = word;
				index += 1;
			}

			// If the first word in the line (which should be the product name) matches the search query, set the product_found flag to true and break out of the loop
			if (words[0] == product)
			{
				product_found = true;
				break;
			}
		}

		// Close the input file
		infile.close();

		// Display error message if product not found
		if (!product_found)
		{
			cout << "No such item found in the Inventory    :(  " << endl;
			return false;
		}
		else
		{
			return true;
		}
	}

	// This function updates the product quantity and price in the inventory file
	void updateProduct(const string &input_file, const string &product, int new_quant, int new_price = 1)
	{
		// open the input and output files
		ifstream infile(input_file);
		ofstream outfile("temp.txt");
		string line;
		bool product_found = false;

		// read each line in the input file
		while (getline(infile, line))
		{
			// split the line into three words: name, price, quantity
			string words[3];
			string word;
			int index = 0;
			istringstream iss(line);

			while (iss >> word)
			{
				words[index] = word;
				index += 1;
			}

			// if the product is found, update the price and quantity
			if (words[0] == product)
			{
				product_found = true;
				words[1] = to_string(new_price);
				words[2] = to_string(new_quant);
			}

			// write the updated line to the output file
			for (int i = 0; i < index; i++)
			{
				outfile << words[i] << (i < index - 1 ? "\t" : "\n");
			}
		}

		// close the input and output files
		infile.close();
		outfile.close();

		// if the product was not found, display an error message
		if (!product_found)
		{
			cout << "No such item found in the Inventory    :(  " << endl;
		}
		else
		{
			// replace the original file with the updated file
			remove(input_file.c_str());
			rename("temp.txt", input_file.c_str());
		}
	}

	// This function displays the current inventory items and their prices and quantities
	void DisplayItemInventory()
	{
		// open the inventory file and print each line
		ifstream f("Inventory.txt", ios::in | ios::binary);
		string s;
		cout << "Name\tprice  quantity\n";
		while (!f.eof())
		{
			getline(f, s);
			cout << s << endl;
		}
		f.close();
	}

	// This function adds a new item to the inventory
	void addItem(Inventory &item)
	{
		// open the inventory file in append mode and add the new item
		ofstream o("Inventory.txt", ios::app | ios::binary);
		o << '\n'
		  << item.getItem() << "\t" << item.getPriceItem() << "\t" << item.getQuantity();
		cout << "Item successfully added to the Inventory!" << endl;
		o.close();
	}

	bool checkAvail(string filename, string name, int amount)
	{
		// Check if the item exists in the inventory
		if (searchInventory(filename, name) == true)
		{
			ifstream infile(filename);
			string line;
			while (getline(infile, line))
			{
				string words[3];
				string word;
				int index = 0;
				istringstream iss(line);

				// Split the line into individual words
				while (iss >> word)
				{
					words[index] = word;
					index += 1;
				}

				// Check if the current line corresponds to the item
				if (words[0] == name)
				{
					// Check if there are enough items in the inventory
					if (stoi(words[2]) > amount)
					{
						return true;
					}
					else
					{
						cout << "Not enough items in the inventory" << endl;
						return false;
					}
				}
			}
		}
	}

	void WriteReport(string S)
	{
		// Open the InventoryReport file in append mode
		ofstream o("InventoryReport.txt", ios::app | ios::binary);
		// Write the string to the file
		o << S << '\n';
		o.close();
	}

	void DisplayInventoryReport()
	{
		// Open the InventoryReport file in input mode
		ifstream f("InventoryReport.txt", ios::in | ios::binary);
		string s;
		// Print the header for the report
		cout << "Name\tquantity\tDate\t\tTime\t\t\t status\n";
		while (!f.eof())
		{
			// Read a line from the file
			getline(f, s);
			// Print the line
			cout << s << endl;
		}
		f.close();
		// Pause the program for 30 seconds using the Sleep function
		Sleep(30000);
	}
};

class User
{
protected:
	string Name;
	string phone;

public:
	void virtual xyz() const = 0;
	void setName(string s)
	{
		this->Name = s;
	}
	string getName()
	{
		return Name;
	}
	void setPhone(string P)
	{
		this->phone = P;
	}
	string getPhone()
	{
		return phone;
	}
};

class Customer : public User
{
protected:
	int customerID;
	int bill = 0;
	// Shopping *S = new Shopping;
	Inventory *I = new Inventory;

public:
	void xyz() const
	{
		cout << "Customer" << endl;
	}
	void setID(int i)
	{
		customerID = i;
	}
	int getID()
	{
		return customerID;
	}
	void setBill(int b)
	{
		bill = b;
	}
	int getBill()
	{
		return bill;
	}
	ostream &display(ostream &out) const;
	friend ostream &operator<<(ostream &out, Customer &B);

	void DisplayItems()
	{
		cout << "*********************************************************************************" << endl;
		cout << "The entire Inventory is Displayed below:                     " << endl;
		I->DisplayItemInventory();
		cout << endl
			 << endl;
		cout << "*********************************************************************************" << endl;
	}

	// This function takes in a product name and a quantity as inputs and returns the total cost of the items.
	int calculateBill(string name, int quantity)
	{
		// Open the "Inventory.txt" file for reading in binary mode.
		ifstream f("Inventory.txt", ios::in | ios::binary);

		// Declare a string variable to hold each line of the file.
		string s;

		// Loop through each line of the file until the end of file is reached or a match for the product name is found.
		while (!f.eof() && getline(f, s))
		{
			// Read the next line of the file.
			getline(f, s);

			// Declare an array of strings to hold the words in the line.
			string words[3];

			// Declare a string variable to hold each word in the line.
			string word;

			// Declare an integer variable to keep track of the index of the current word in the line.
			int index = 0;

			// Use an istringstream object to split the line into words and store them in the words array.
			istringstream iss(s);

			// Loop through each word in the line and store it in the words array.
			while (iss >> word)
			{
				words[index] = word;
				index += 1;
			}

			// If the first word in the line matches the product name, close the file and return the total cost of the items.
			if (words[0] == name)
			{
				f.close();
				return stoi(words[1]) * quantity;
			}
		}

		// If the end of file is reached without finding a match for the product name, close the file and return nothing.
		f.close();
	}

	// This function initiates the shopping process
	void shopping()
	{
		// Get the current local time
		std::time_t t = std::time(0);
		// Convert the time to a structure representing a broken-down time
		std::tm *now = std::localtime(&t);
		// Get the current system time as a time point
		auto current_time_point = std::chrono::system_clock::now();
		// Convert the time point to a C-style time value
		auto time_t_value = std::chrono::system_clock::to_time_t(current_time_point);
		// Convert the C-style time value to a string
		std::string time_string(ctime(&time_t_value));
		// Extract the current year and month from the time structure
		int year = now->tm_year + 1900;
		int month = (now->tm_mon + 1);

		// Initialize variables for the name and quantity of the item, and a random number for the lucky draw
		string name = " ";
		int quantity = 0;
		int r = rand() % 40 + 1;
		bool status = false;

		// Loop until the user enters "bye" to exit the shopping process
		while (name != "bye")
		{
			string detail = "";
			// Clear the console screen and display the list of available items
			system("cls");
			DisplayItems();

			// Prompt the user to enter the name of the item they want to buy, or enter "bye" to exit
			cout << "Enter the name of the item you want to buy" << endl;
			cout << "Enter bye to exit" << endl;
			cin >> name;

			// If the user enters "bye", display the total bill and prompt for a discount code
			if (name == "bye")
			{
				string promocode;
				string promo1 = "eid2023";
				string promo2 = "ramzan2023";
				string promo3 = "christmas2023";
				string special = "OOP4";
				cout << "IF YOU HAVE ANY DISCOUNT CODE PLS ENTER\n IF YOU DONT HAVE ANY PRESS KEY AND ENTER\n";
				cin >> promocode;

				// If the user enters a valid discount code, apply the discount to the total bill
				if (promocode == promo1 || promocode == promo2 || promocode == promo3)
				{
					cout << "Congrats you have availed 20% discount\n";
					bill *= 0.8;
				}
				// If the user enters a special discount code, apply a larger discount to the total bill
				else if (promocode == special)
				{
					cout << "Congrats you have special 50% discount\n";
					bill *= 0.5;
				}
				// If the user doesn't enter a valid discount code, perform a lucky draw for a chance to win a discount
				else
				{
					cout << "Pls wait while the lucky draw takes place\n";
					Sleep(2500);
					int x = rand() % 10 + 1;
					if (x == r)
					{
						cout << "CONGRATULATIONS YOU WON THE LUCKY DRAW!!!!!\n";
						bill *= 0.9;
					}
				}
				// Display the final total bill
				cout << "Your total bill is:" << bill << endl;
			}

			// If the user enters the name of an item, prompt for the quantity they want to buy
			cout << "Enter the quantity of the item you want to buy" << endl;
			cin >> quantity;

			status = I->checkAvail("Inventory.txt", name, quantity); // Check if the item is available in the inventory
			if (status == true)
			{
				cout << "Item is available in the inventory" << endl;
				int invoice = calculateBill(name, quantity); // Calculate the bill for the purchased item(s)
				bill += invoice;							 // Add the bill to the total bill
				// Check if the purchased quantity is eligible for any discounts and apply them
				if (quantity > 5)
				{
					cout << "For this item you get 50 rs off as a token of gratitude\n";
					bill -= 50;
				}
				if (quantity > 10)
				{
					cout << "For this item you get 100 rs off as a token of gratitude\n";
					bill -= 100;
				}
				if (quantity > 15)
				{
					bill -= 150;
					cout << "For this item you get 150 rs off as a token of gratitude\n";
				}
				if (quantity > 20)
				{
					bill -= 200;
					cout << "For this item you get 200 rs off as a token of gratitude\n";
				}

				// Record the details of the purchase in the sales report
				string detail = "";
				detail.append(name);
				detail.append("\t");
				detail.append(to_string(quantity));
				detail.append("\t");
				detail.append(to_string(now->tm_mday));
				detail.append("-");
				detail.append(to_string(month));
				detail.append("-");
				detail.append(to_string(year));
				detail.append("\t");
				time_string.pop_back(); // Remove the newline character from the time string
				detail.append(time_string);
				detail.append("\t");
				detail.append("sold");
				detail.append("\n");
				I->WriteReport(detail); // Write the sales report to the file

				cout << "Your bill currently is " << bill << endl;
				PurchaseUpdate(name, quantity); // Update the inventory with the purchased quantity
				Sleep(3000);
			}
			else
			{
				cout << "Item is not available in the inventory" << endl;
				Sleep(3000);
				shopping(); // recursive call to purchase a different item
			}
		}
	}

	void PurchaseUpdate(string name, int q)
	{
		const string update = "Inventory.txt"; // set the name of the file to update
		ifstream updateStream(update);		   // open the file for reading
		ofstream outfile("temp.txt");		   // create a temporary file for writing
		string line;
		while (getline(updateStream, line)) // loop through each line in the file
		{
			string words[3]; // create an array to store the three fields in each line
			string word;
			int index = 0;
			istringstream iss(line); // create a string stream for parsing the line
			while (iss >> word)		 // loop through each word in the line
			{
				words[index] = word; // add the word to the array
				index += 1;			 // increment the index
			}
			if (words[0] == name) // if the first field matches the item name
			{
				int quantity = stoi(words[2]);										 // convert the third field to an integer
				quantity -= q;														 // subtract the purchase quantity from the current quantity
				outfile << words[0] << "\t" << words[1] << "\t" << quantity << endl; // write the updated line to the temporary file
			}
			else
			{
				outfile << line << endl; // write the original line to the temporary file
			}
		}

		updateStream.close();				// close the input file
		outfile.close();					// close the output file
		remove(update.c_str());				// delete the original file
		rename("temp.txt", update.c_str()); // rename the temporary file to the original filename
	}

	void RefundUpdate(string name, int q)
	{
		std::time_t t = std::time(0);												  // get the current time
		std::tm *now = std::localtime(&t);											  // convert to local time
		auto current_time_point = std::chrono::system_clock::now();					  // get the current time point
		auto time_t_value = std::chrono::system_clock::to_time_t(current_time_point); // convert to time_t
		std::string time_string(ctime(&time_t_value));								  // convert to a string
		int year = now->tm_year + 1900;												  // get the current year
		int month = (now->tm_mon + 1);												  // get the current month
		if (I->checkAvail("Inventory.txt", name, q))								  // check if the item is available in the inventory
		{
			string detail = "";					   // create a string to store the refund details
			const string update = "Inventory.txt"; // set the name of the file to update
			ifstream updateStream(update);		   // open the file for reading
			ofstream outfile("temp.txt");		   // create a temporary file for writing
			string line;
			while (getline(updateStream, line)) // loop through each line in the file
			{
				string words[3]; // create an array to store the three fields in each line
				string word;
				int index = 0;
				istringstream iss(line); // create a string stream for parsing the line
				while (iss >> word)		 // loop through each word in the line
				{
					words[index] = word; // add the word to the array
					index += 1;			 // increment the index
				}
				if (words[0] == name) // if the first field matches the item name
				{
					int quantity = stoi(words[2]);
					quantity += q;
					outfile << words[0] << "\t" << words[1] << "\t" << quantity << endl;
				}
				else
				{
					outfile << line << endl;
				}
			}
			updateStream.close();
			outfile.close();
			remove(update.c_str());
			rename("temp.txt", update.c_str());
			cout << "Your refund has been processed" << endl;
			detail.append(name);
			detail.append("\t");
			detail.append(to_string(q));
			detail.append("\t");
			detail.append(to_string(now->tm_mday));
			detail.append("-");
			detail.append(to_string(month));
			detail.append("-");
			detail.append(to_string(year));
			detail.append("\t");
			time_string.pop_back(); // Remove the newline character
			detail.append(time_string);
			detail.append("\t");
			detail.append("Refunded");
			detail.append("\n");
			I->WriteReport(detail);
		}
		else
		{
			cout << "Item is not available in the inventory" << endl;
		}
	}
};

class Supplier
{
protected:
	Inventory *I = new Inventory;

public:
	void restock(string name, int q)
	{
		std::time_t t = std::time(0);												  // get time now - get current time as a time_t object
		std::tm *now = std::localtime(&t);											  // convert the time_t object to a tm structure representing the local time
		auto current_time_point = std::chrono::system_clock::now();					  // get the current time as a time_point object
		auto time_t_value = std::chrono::system_clock::to_time_t(current_time_point); // convert the time_point object to a time_t object
		std::string time_string(ctime(&time_t_value));								  // convert the time_t object to a string representation of the time
		int year = now->tm_year + 1900;												  // extract the year from the tm structure and add 1900 to get the actual year
		int month = (now->tm_mon + 1);												  // extract the month from the tm structure and add 1 to get the actual month

		string detail = "";				 // initialize an empty string to hold the inventory update details
		string update = "Inventory.txt"; // the name of the file containing the inventory information
		ifstream updateStream(update);	 // open the file for reading
		ofstream outfile("temp.txt");	 // create a temporary file for writing
		string line;
		while (getline(updateStream, line)) // read each line of the file
		{
			string words[3]; // initialize an array to hold the words in each line
			string word;
			int index = 0;
			istringstream iss(line); // create a string stream for the current line
			while (iss >> word)		 // read each word from the string stream
			{
				words[index] = word; // store the word in the array
				index += 1;
			}
			if (words[0] == name) // if the current line corresponds to the item being restocked
			{
				int quantity = stoi(words[2]);													// get the current quantity of the item
				quantity += q;																	// add the new quantity to the current quantity
				outfile << words[0] << "\t" << words[1] << "\t" << to_string(quantity) << endl; // write the updated line to the temporary file
			}
			else // if the current line corresponds to a different item
			{
				outfile << line << endl; // write the line to the temporary file unchanged
			}
		}
		updateStream.close();				// close the input file
		outfile.close();					// close the temporary file
		remove(update.c_str());				// delete the original file
		rename("temp.txt", update.c_str()); // rename the temporary file to the original file name

		cout << "Your order is in process and will be delivered soon" << endl;
		detail.append(name);

		detail.append("\t");
		detail.append(to_string(q));
		detail.append("\t");
		detail.append(to_string(now->tm_mday));
		detail.append("-");
		detail.append(to_string(month));
		detail.append("-");
		detail.append(to_string(year));
		detail.append("\t");
		time_string.pop_back(); // Remove the newline character
		detail.append(time_string);
		detail.append("\t");
		detail.append("Stocked");
		detail.append("\n");
		I->WriteReport(detail);
	}
};

class Manager : public User
{
protected:
	Inventory *I = new Inventory;
	Supplier *S = new Supplier;

public:
	void xyz() const
	{
		cout << "Manager" << endl;
	}
	/*Function used by the admin to view the entire Catalog*/
	void DisplayInventory()
	{
		cout << "*********************************************************************************" << endl;
		cout << "The entire Inventory is Displayed below:                     " << endl;
		I->DisplayItemInventory();
		cout << endl
			 << endl;
		cout << "*********************************************************************************" << endl;
	}

	/*Function used by the admin to update an item*/
	void updateInventory()
	{
		string name;
		int price, quantity;
		cout << "*********************************************************************************" << endl;
		cout << "Enter the Name of the item to want to search     " << endl;
		cout << endl;
		cout << "Item name: ";
		cin >> name;
		cout << endl;
		cout << "What would you like the new price to be?" << endl;
		cin >> price;
		cout << "What is the new quantity available" << endl;
		cin >> quantity;
		cout << endl;
		string input_file = "Inventory.txt";
		I->updateProduct(input_file, name, quantity, price);
		cout << "*********************************************************************************" << endl;
	}

	/*This function will be used to add a new items in catalog*/
	void addInventory()
	{
		string name;
		int price;
		int quantity;
		cout << "*********************************************************************************" << endl;
		cout << "               Enter the Name of the item to want to add in the Inventory    " << endl;
		cout << endl;
		cout << "Item name: ";
		cin >> name;
		cout << endl;
		cout << "What would you like the new price to be?" << endl;
		cin >> price;
		cout << endl;
		cout << "What is the total quantity of this item?" << endl;
		cin >> quantity;
		cout << endl;
		Inventory C(name, price, quantity);
		I->addItem(C);
		cout << "*********************************************************************************" << endl;
	}

	/*This function will be used to delete an item based on its name*/
	void deleteInventory()
	{
		string name;
		cout << "*********************************************************************************" << endl;
		cout << "Enter the Name of the item to want to add in the Inventory    " << endl;
		cout << endl;
		cout << "Item name: ";
		cin >> name;
		cout << endl;
		// I->deleteitemInventory(name);
		cout << "*********************************************************************************" << endl;
	}

	void DisplaySales()
	{
		cout << "*********************************************************************************" << endl;
		cout << endl;
		cout << endl;
		I->DisplayInventoryReport();
		cout << "*********************************************************************************" << endl;
	}
	void restockinventory(string name, int q)
	{
		cout << "*********************************************************************************" << endl;
		S->restock(name, q);
	}
};

ostream &Customer::display(ostream &out) const
{
	out << "Welcome Mr. " << Name << endl;
	out << "Your customer ID for this session  is: " << customerID << endl;
	return out;
}

ostream &operator<<(ostream &out, Customer &C)
{
	C.display(out);
	return out;
}

int main()
{
	// Initialize the random seed using the current time
	srand(time(NULL));

	// Initialize variables
	int choice = -1; // User input variable
	int c = -1;		 // Counter variable

	// Initialize string variables for text formatting
	string divider = "*********************************************************************************";
	string reset = "\033[0m";
	string bold = "\033[1m";
	string green = "\033[32m";
	string yellow = "\033[33m";
	string cyan = "\033[36m";
	string Red = "\033[31m";
	string Blue = "\033[34m";
	string Magenta = "\033[35m";
	string Grey = "\033[90m";
	string LightRed = "\033[91m";
	string LightGreen = "\033[92m";
	string LightYellow = "\033[93m";
	string LightBlue = "\033[94m";
	string LightMagenta = "\033[95m";
	string LightCyan = "\033[96m";
	string White = "\033[37m";
	string light_gray_bg = "\033[47m";

	// While loop to validate user input for menu choice
	while (!(choice == 0 || choice == 1 || choice == 2))
	{
		// Clear the console
		system("cls");

		// Print formatted text
		cout << green << divider << reset << endl;
		cout << endl
			 << endl
			 << endl;
		cout << bold << cyan << "                     Welcome to the One Stop mart!                                  " << reset << endl;
		cout << endl
			 << endl
			 << endl;
		cout << yellow << "                     Press 1 if you are a customer                              " << endl;
		cout << "                     Press 2 if you are a manager                               " << endl;
		cout << "                     Press 0 to exit                                            " << endl;
		cout << reset << endl
			 << endl
			 << endl;
		cout << green << divider << reset << endl;

		// Get user input for menu choice
		cin >> choice;
	}

	switch (choice)
	{
	case 0:
		exit(0);
		break;
	case 1:
	{
		Customer C1;
		string nAme;
		string selection;
		int id;
		int x = 0;
		int bill = 0;
		system("cls");
		cout << cyan << "Pls enter your name\n";
		cin >> nAme;
		C1.setName(nAme);
		system("cls");
		id = (rand() % 100000) + 1;
		C1.setID(id);
		cout << C1;
		Sleep(3000);
		system("cls");
		cout << "Now Mr. " << yellow << C1.getName() << cyan << " what would you like to do: \n\n";
		// while (1) {
		while (!(c == 11 || c == 12))
		{
			cout << green << "Press 11 if you want to go view our inventory \n"
				 << Magenta << "Press 12 if you want to claim a refund for any previous purchase\n";
			cin >> c;
			cout << cyan;
		}
		if (c == 11)
		{

			C1.shopping();
			cout << cyan << "Thank you Mr. " << yellow << C1.getName() << cyan << " for shopping with us. We hope to see you again soon.\n";
			break;
			// c = -1;
			system("cls");
		}
		if (c == 12)
		{
			bool out = true;
			while (out)
			{
				string name;
				int quantity;
				C1.DisplayItems();
				cout << yellow << "Enter the name of the item you want to return\n";
				cin >> name;
				cout << yellow << "Enter the quantity of the item you want to return\n";
				cin >> quantity;
				C1.RefundUpdate(name, quantity);
				cout << green << "Enter 1 if you want to return another item\n";
				cin >> x;
				if (x != 1)
				{
					out = false;
				}
				// c = -1;
				system("cls");
			}
			break;
		}
		//}
		break;
	}
	case 2:
	{
		Manager M1;
		string Managername;
		system("cls");
		cout << green << "Pls enter your name\n";
		cin >> Managername;
		system("cls");
		while (1)
		{
			while (!(c == 21 || c == 22 || c == 23 || c == 24 || c == 25))
			{
				cout << yellow << "Press 21 if you want add new item \n"
					 << Magenta << "Press 22 to update an existing item \n"
					 << Blue << "Press 23 to view inventory report\n"
					 << Red << "Press 24 to view sales report\n"
					 << cyan << "Press 25 to restock inventory\n";
				cin >> c;
			}
			if (c == 21)
			{
				M1.addInventory();
				c = -1;
				system("cls");
			}
			if (c == 22)
			{
				M1.updateInventory();
				c = -1;
				system("cls");
			}
			if (c == 23)
			{
				M1.DisplayInventory();
				Sleep(10000);
				c = -1;
				system("cls");
			}
			if (c == 24)
			{
				M1.DisplaySales();
				c = -1;
				system("cls");
			}
			if (c == 25)
			{
				int decision = 1;
				while (decision == 1)
				{
					string name;
					int q;
					M1.DisplayInventory();
					cout << green << "enter name of item u want to retock\n";
					cin >> name;
					cout << "enter quantity\n";
					cin >> q;
					M1.restockinventory(name, q);
					cout << Red << "Enter 0 if you are done\nPress 1 to continue" << endl;
					cin >> decision;
					system("cls");
					c = -1;
					system("cls");
				}
			}
		}
		break;
	}
	default:
		break;
	}

	cout << "bye bye bye\n";
	return 0;
}
