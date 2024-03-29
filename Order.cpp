#include "Order.h"
#include "UIElems.h"
#include "customer.h"
#include "InventoryItem.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <conio.h>

Order::Order()
{
	itemName = "";
	itemPrice = 0.0f;
	quantity = 0.0f;
	orders = "";
}

bool Order::idDoneOrdering()
{
	bool isDone;
	std::cout << "Done with order? (Y/N): ";
	char doneInput;
	std::cin >> doneInput;
	// refactor this to ternary operator
	if (std::toupper(doneInput) == 'Y')
	{
		isDone = true;
	}
	else
	{
		isDone = false;
	}
	return isDone;
}

bool Order::placeOrder(Customer* customer)
{
	bool success{ true };
	std::cout << "\n\nPLACE AN ORDER HERE...\n";
	//std::cout << "Hello, " << customer.getUsername() << "! Place an order here.\n";
	MenuItem menu;
	menu.showMenu();

	std::cout << "\nPlace your order now!\n";
	bool done{ false };
	while (!done)
	{
		MenuItem currItem;
		InventoryItem inItem;

		std::cout << "Item ID: ";
		int id;
		std::cin >> id;

		currItem = currItem.getItem(id);
		inItem = inItem.getItem(id);
		Order currOrderItem;

		currOrderItem.itemName = currItem.getName();
		currOrderItem.itemPrice = currItem.getPrice();
		currOrderItem.quantity = 0.0f;
		currOrderItem.orderComplete = false;

		std::cout << "Item: " << currOrderItem.itemName << "\tPrice: " << currOrderItem.itemPrice << '\n';
		std::cout << "Quantity: ";
		float currItemQuantity;
		std::cin >> currItemQuantity;

		if (inItem.getStock() == 0 || inItem.getStock() < currItemQuantity)
		{
			system("cls");
			menu.showMenu();
			std::cout << "\nItem out of stock for today!!\n";
		}
		else
		{
			currOrderItem.quantity += currItemQuantity;
			currOrderItem.itemPrice *= currOrderItem.quantity;

			std::cout << "\Order placed!\n";
			std::cout << currOrderItem.itemName << '\t' << currOrderItem.quantity << '\t' << currOrderItem.itemPrice << '\n';

			if (customer->getUsername() == "")
			{
				if (!customer->createOrderAccount())
				{
					done = true;
					success = false;
					customer->resetAccount();
					break;
				}
				// make a data file with record of order items for this customer
				std::string orderFile = "RestaurantData/Orders/" + customer->getUsername() + ".txt";
				std::ofstream outf(orderFile, std::ios::app);
				outf << currOrderItem.itemName << ',' << currOrderItem.quantity << ',' << currOrderItem.itemPrice << ',' << (currOrderItem.orderComplete ? "true" : "false") << '\n';
				outf.close();
			}
			else
			{
				std::string customerOrderFile = "RestaurantData/Orders/" + customer->getUsername() + ".txt";
				std::ofstream outf(customerOrderFile, std::ios::app);
				outf << currOrderItem.itemName << ',' << currOrderItem.quantity << ',' << currOrderItem.itemPrice << ',' << (currOrderItem.orderComplete ? "true" : "false") << '\n';
				outf.close();
			}
			inItem.updateStock(inItem, currItemQuantity);
		}
		done = idDoneOrdering();
	}
	return success;
}

void Order::createOrderFile(Order currOrderItem, std::string path)
{
	std::ofstream outf(path, std::ios::app);

	outf << currOrderItem.itemName << ',' <<  currOrderItem.quantity << ',' << currOrderItem.itemPrice << ',' << (currOrderItem.orderComplete ? "true" : "false") << '\n';

	outf.close();
}

bool Order::displayCustomerOrder(Customer customer) {

	SetWindowSizeAndCentre(); // Center the console window
	system("cls");

	bool fileExists{ true };
	std::string filePath = "./RestaurantData/Orders/" + customer.getUsername() + ".txt";

	std::ifstream inf(filePath);
	int itemTotal = 0;
	float priceTotal = 0.0f;
	float price;

	if (!inf) {
		Title("No order file for this customer: " + customer.getUsername(), centerY - 15); // Center the title
		std::cout << "File could not be open! FILE_NAME: " << filePath << '\n';
		fileExists = false;
	}
	else {
		Title("Customer Order for " + customer.getUsername(), centerY - 15); // Center the title
		std::cout << "\n\n";

		std::vector<std::string> itemNames; // Store item names to find the longest
		std::string line;
		int sn = 0;
		bool complete = false;
		int lineCount = 0;

		// Find the maximum item name length to set the column width
		size_t maxItemNameLength = 0;
		int consoleWidth = GetWindowSize().X;

		std::stringstream tableHeadingStream;
		tableHeadingStream << std::setw(10) << std::left << "SN"
			<< std::setw(20) << std::left << "Item"
			<< std::setw(10) << std::left << "Quantity"
			<< std::setw(11) << std::left << "Price"
			<< std::setw(10) << std::left << "Status"
			<< '\n';
		std::string tableHeading = tableHeadingStream.str();
		int padding = (consoleWidth - tableHeading.length()) / 2;
		std::cout << std::setw(padding) << ' ';
		std::cout << tableHeading;
		// for line
		std::stringstream seperatorStream;
		seperatorStream << '-' << std::setfill('-') << std::setw(tableHeading.length() + 4) << '-' << std::endl;
		std::string seperator = seperatorStream.str();
		int separatorPadding = (consoleWidth - seperator.length()) / 2;

		std::cout << std::setfill(' ') << std::setw(padding - 2) << ' ' << seperator;
		std::cout << std::setfill(' ');


		while (std::getline(inf, line)) {
			if (lineCount == 0) {
				++lineCount;
				continue;
			}
			std::string name;
			float quantity;
			//float price;

			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			name = line.substr(0, commaIndex1);
			quantity = std::stof(line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1));
			price = std::stof(line.substr(commaIndex2 + 1, commaIndex3 - commaIndex2 - 1));
			complete = line.substr(commaIndex3 + 1).compare("true") == 0 ? true : false;

			// Printing the items
			std::cout << std::setw(padding) << ' ';
			std::cout << std::setw(10) << std::left << ++sn
				<< std::setw(20) << std::left << name
				<< std::setw(10) << std::left << quantity
				<< std::setw(11) << std::left << price
				<< std::setw(10) << std::left << (complete ? "Complete" : "Pending")
				<< '\n';
			
			itemTotal += quantity;
			priceTotal +=  price;
			itemNames.push_back(name); // Store item names
		}

		std::cout << std::setfill(' ') << std::setw(padding - 2) << ' ' << seperator;
		std::cout << std::setfill(' ');

		// table total information
		std::cout << std::setw(padding) << ' ';
		std::cout << std::setw(30) << std::left << "Total"
			 << std::setw(10) << std::left << itemTotal
			 << std::setw(11) << std::left << priceTotal
			<< ""
			<< std::setw(11) <<(isAllOrderComplete(customer) ? "Complete" : "Not Complete")
			<< '\n';
	}

	inf.close();
	return fileExists;
}

/*void Order::displayOrderFromFile(std::string path)
{
	std::ifstream inf(path);
	if (!inf)
	{
		std::cout << "No order file for this path: " << path << "!\n";
	}
	else
	{
		
		// === YINIHARU CHANGE GARNE === 
		std::cout << std::setfill(' ') << std::setw(5) << ' '
			<< std::setw(20) << ' '
			<< std::setw(10) << ' ' << std::setfill(' ') << std::endl;

		std::cout << std::setw(3) << "SN" << std::setw(20) << "Item" << std::setw(15) << "Quantity" << std::setw(15) << "Status" << '\n';
		std::cout << "--------------------------------------------------------------\n";
		std::string line;
		int sn = 0;
		bool complete = false;
		int lineCount = 0;
		while (std::getline(inf, line))
		{
			if (lineCount == 0)
			{
				++lineCount;
				continue;
			}
			std::string name;
			float quantity;
			float price;

			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			name = line.substr(0, commaIndex1);
			quantity = std::stof(line.substr(commaIndex1 + 1, commaIndex2));
			price = std::stof(line.substr(commaIndex2 + 1, commaIndex3));
			complete = line.substr(commaIndex3 + 1).compare("true") == 0 ? true : false;

			std::cout << std::setw(3) << ++sn << std::setw(20) << name << std::setw(15) << quantity << std::setw(15) << (complete ? "Complete" : "Pending") << '\n';
		}
		std::cout << "--------------------------------------------------------------\n";
	}
}*/

void Order::displayOrderFromFile(std::string path)
{
	std::ifstream inf(path);
	if (!inf)
	{
		//std::cout << "No order file for this path: " << path << "!\n";
		Title("No order file for this path ", centerY - 15); // Center the title
	}
	else
	{
		std::string line;
		int sn = 0;
		bool complete = false;
		int lineCount = 0;

		// Getting console window size
		COORD consoleSize = GetWindowSize();
		int consoleWidth = consoleSize.X;
		int consoleHeight = consoleSize.Y;

		Title("Details and Status of Orders", centerY - 15); // Center the title

		// Calculating padding for centering
		int tableWidth = 50;
		int paddingX = (consoleWidth - tableWidth) / 2;

		// Printing empty lines for vertical centering
		for (int i = 0; i < (consoleHeight - 27) / 2; ++i)
		{
			std::cout << std::endl;
		}

		std::cout << std::setw(paddingX) << std::setfill(' ') << ' '
			<< std::setw(5) << ' '
			<< std::setw(20) << ' '
			<< std::setw(10) << ' ' << std::setfill(' ') << std::endl;

		std::cout << std::setw(paddingX) << ' ' << std::setw(5) << "SN" << std::setw(20) << "Item" << std::setw(15) << "Quantity" << std::setw(15) << "Status" << '\n';
		std::cout << std::setw(paddingX - 2) << ' ' << "----------------------------------------------------\n";

		while (std::getline(inf, line))
		{
			if (lineCount == 0)
			{
				++lineCount;
				continue;
			}
			std::string name;
			float quantity;
			float price;

			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			name = line.substr(0, commaIndex1);
			quantity = std::stof(line.substr(commaIndex1 + 1, commaIndex2));
			price = std::stof(line.substr(commaIndex2 + 1, commaIndex3));
			complete = line.substr(commaIndex3 + 1).compare("true") == 0 ? true : false;

			std::cout << std::setw(paddingX) << ' ' << std::setw(5) << ++sn << std::setw(20) << name << std::setw(15) << quantity << std::setw(15) << (complete ? "Complete" : "Pending") << '\n';
		}
		std::cout << std::setw(paddingX - 2) << ' ' << "----------------------------------------------------\n";
		std::cout << "\n";
	}
}

void Order::markItemOrderComplete(std::string path, int id)
{
	std::ifstream inf(path);
	std::ofstream outf("temp.txt", std::ios::app);
	int count = 0;
	if (!inf)
	{
		std::cout << "No order file for this path: " << path << "!\n";
	}
	else
	{
		std::string line;
		int sn = 0;
		bool complete = false;
		int lineCount = 0;
		while (std::getline(inf, line))
		{
			if (lineCount == 0)
			{
				++lineCount;
				outf << line << '\n';
				continue;
			}
			std::string name;
			float quantity;
			float price;

			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			name = line.substr(0, commaIndex1);
			quantity = std::stof(line.substr(commaIndex1 + 1, commaIndex2));
			price = std::stof(line.substr(commaIndex2 + 1, commaIndex3));
			complete = line.substr(commaIndex3 + 1).compare("true") == 0 ? true : false;

			if (++count == id)
			{
				complete = !complete; // toggle complete
			}

			outf << name << ',' << quantity << ',' << price << ',' << (complete ? "true" : "false") << '\n';
		}
	}
	outf.close();
	inf.close();

	std::remove(path.c_str());
	std::rename("temp.txt", path.c_str());
}

bool Order::isAllOrderComplete(Customer customer)
{
	bool completed{ true };
	std::string orderFilePath{ "./RestaurantData/Orders/" + customer.getUsername() + ".txt" };

	std::ifstream inf(orderFilePath);
	int orderCount = 0;
	
	int lineCount = 0;
	while (!inf.eof())
	{
		std::string line;
		if (std::getline(inf, line))
		{
			if (lineCount == 0)
			{
				++lineCount;
				continue;
			}
			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			bool orderComplete = (line.substr(commaIndex3 + 1)).compare("true") == 0 ? true : false;

			if (!orderComplete)
			{
				completed = false;
			}
		}
	}

	inf.close();

	return completed;
}

void Order::cancelOrder(Customer customer)
{
	std::cout << "\nId: ";
	int id = 0;
	std::cin >> id;
	int count = 0;
	std::string orderFilePath{ "./RestaurantData/Orders/" + customer.getUsername() + ".txt" };
	std::ifstream inf(orderFilePath);
	std::ofstream outf("temp.txt", std::ios::app);
	int orderCount = 0;
	InventoryItem inventory;
	while (!inf.eof())
	{
		std::string line;
		if (std::getline(inf, line))
		{
			if (orderCount == 0)
			{
				outf << line << '\n';
				++orderCount;
				continue;
			}
			int commaIndex1 = line.find(',');
			int commaIndex2 = line.find(',', commaIndex1 + 1);
			int commaIndex3 = line.find(',', commaIndex2 + 1);

			bool orderComplete = (line.substr(commaIndex3 + 1)).compare("true") == 0 ? true : false;
			count++;
			if (orderComplete && id == count)
			{
				std::cout << "Your order is complete! Cannot cancel completed order!\n";
				outf << line << '\n';
				orderCount++;
			}
			else if (!orderComplete && id == count)
			{
				std::cout << "Order Canceled!\n";
				std::string itemName = line.substr(0, commaIndex1);
				int quantity = std::stoi(line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1));

				inventory.cancelledOrderUpdate(itemName, quantity);
				continue;
			}
			else
			{
				outf << line << '\n';
				orderCount++;
			}
		}
	}

	inf.close();
	outf.close();

	std::remove(orderFilePath.c_str());
	std::rename("temp.txt", orderFilePath.c_str());

	if (count <= 1)
	{
		system("CLS");
		std::cout << "You have not placed an order yet!\n";
		std::remove(orderFilePath.c_str());
	}
}

void Order::addOrdersToReport(Customer customer, std::string finance_report_filename)
{
	std::string orderFilePath{ "RestaurantData/Orders/" + customer.getUsername() + ".txt" };

	// yeha nira tyo finance report ma add garne
	// paila check if tyo report ma aile ko order xa ki nai
	std::ifstream report_inf;
	std::ifstream order_inf;
	order_inf.open(orderFilePath);

	int orderLineNum = 0;
	while (!order_inf.eof())
	{
		std::string orderLine;
		if (std::getline(order_inf, orderLine))
		{
			if (orderLineNum == 0)
			{
				++orderLineNum;
				continue;
			}
			int orderCommaIndex1 = orderLine.find(',');
			int orderCommaIndex2 = orderLine.find(',', orderCommaIndex1 + 1);
			int orderCommaIndex3 = orderLine.find(',', orderCommaIndex2 + 1);
			// reading items from the order file first
			std::string orderItemName = orderLine.substr(0, orderCommaIndex1);
			int orderQantity = std::stoi(orderLine.substr(orderCommaIndex1 + 1, orderCommaIndex2 - orderCommaIndex1 - 1));
			int orderTotalPrice = std::stoi(orderLine.substr(orderCommaIndex2 + 1, orderCommaIndex3 - orderCommaIndex2 - 1));

			int reportLineNum = 0;
			// check if order item exists in report file
			bool itemExistsInReport{ false };
			report_inf.open(finance_report_filename);
			while (!report_inf.eof())
			{
				std::string reportLine;
				if (std::getline(report_inf, reportLine))
				{
					if (reportLineNum == 0)
					{
						++reportLineNum;
						continue;
					}
					int reportCommaIndex1 = reportLine.find(',');
					int reportCommaIndex2 = reportLine.find(',', reportCommaIndex1 + 1);
					std::string reportItemName = reportLine.substr(reportCommaIndex1 + 1, reportCommaIndex2 - reportCommaIndex1 - 1);

					// item exists if names match
					if (reportItemName == orderItemName)
					{
						itemExistsInReport = true;
						break;
					}
				}
			}
			// file ko end ma pugesi yesto garnuparne rahexa
			report_inf.close();
			report_inf.open(finance_report_filename);

			// temp file to copy report
			std::ofstream temp_outf;
			temp_outf.open("temp.txt", std::ios::app);
			if (itemExistsInReport)
			{
				reportLineNum = 0;
				// jun name milxa tesko quantity ra total badhaune
				while (!report_inf.eof())
				{
					std::string reportLine;
					if (std::getline(report_inf, reportLine))
					{
						if (reportLineNum == 0)
						{
							++reportLineNum;
							temp_outf << reportLine << '\n';
							continue;
						}
						int reportCommaIndex1 = reportLine.find(',');
						int reportCommaIndex2 = reportLine.find(',', reportCommaIndex1 + 1);
						int reportCommaIndex3 = reportLine.find(',', reportCommaIndex2 + 1);
						int reportCommaIndex4 = reportLine.find(',', reportCommaIndex3 + 1);

						int reportSN = std::stoi(reportLine.substr(0, reportCommaIndex1));
						std::string reportItemName = reportLine.substr(reportCommaIndex1 + 1, reportCommaIndex2 - reportCommaIndex1 - 1);
						float reportItemPrice = std::stof(reportLine.substr(reportCommaIndex2 + 1, reportCommaIndex3 - reportCommaIndex2 - 1));
						int reportItemQuantity = std::stoi(reportLine.substr(reportCommaIndex3 + 1, reportCommaIndex4 - reportCommaIndex3 - 1));
						float reportItemTotalPrice = std::stof(reportLine.substr(reportCommaIndex4 + 1, reportLine.length() - reportCommaIndex4 - 2));

						if (reportItemName == orderItemName)
						{
							reportItemQuantity += orderQantity;
							reportItemTotalPrice += orderTotalPrice;
						}
						temp_outf << reportSN << ',' << reportItemName << ',' << reportItemPrice << ',' << reportItemQuantity << ',' << reportItemTotalPrice << '\n';

					}
				}
			}
			else
			{
				// sidai append matra garne
				reportLineNum = 0;
				// jun name milxa tesko quantity ra total badhaune
				while (!report_inf.eof())
				{
					std::string reportLine;
					if (std::getline(report_inf, reportLine))
					{
						++reportLineNum;
						temp_outf << reportLine << '\n';
					}
				}

				float orderItemPrice = static_cast<float>(orderTotalPrice / orderQantity);
				temp_outf << reportLineNum << ',' << orderItemName << ',' << orderItemPrice << ',' << orderQantity << ',' << orderTotalPrice << '\n';
			}
			// close to perform operation on actual file
			report_inf.close();
			temp_outf.close(); 
			if (remove(finance_report_filename.c_str()) == 0) {
				// success, print something
			}
			else {
				// failure, much more interesting
				std::cout << "Can't remove " << finance_report_filename << ": "
					<< strerror(errno) << std::endl;
			}
			if (std::rename("temp.txt", finance_report_filename.c_str()) == 0)
			{
				// success
			}
			else
			{
				std::cout << "Can't rename " << "temp.txt to " << finance_report_filename << ": " << strerror(errno) << std::endl;
			}
		}
	}
	// close all files
	order_inf.close();
}


void Order::payOrderBill(Customer customer, std::string finance_report_filename)
{
	std::string orderFilePath{ "RestaurantData/Orders/" + customer.getUsername() + ".txt" };
	// payment process goes here

	std::cout << "Your password: ";
	std::string password;
	//Enter password for new customer
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == '\b') { // Backspace character
			if (!password.empty()) {
				password.pop_back();
				std::cout << "\b \b"; // Move cursor back and erase the character
			}
		}
		else {
			password.push_back(ch);
			std::cout << '*';
		}
	}

	if (customer.getPassword() == password)
	{
		std::cout << "\nPayment Successful!\n";
		addOrdersToReport(customer, finance_report_filename);
		std::remove(orderFilePath.c_str());
	}
	else
	{
		std::cout << "\nOnly authorized individual and carry out payment!\n";
	}
}