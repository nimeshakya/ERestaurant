#ifndef ADMIN
#define ADMIN

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <chrono>
#include <thread>

static const std::string ADMIN_FILE = "./UserData/ADMIN.txt";
const std::string CUSTOMER_DATA_FILE = "./UserData/CUSTOMER.txt";

enum AdminMainMenuOptions {
	THE_ZERO,
	DISPLAY_ORDERS,
	DISPLAY_MENU,
	INVENTORY,	
	REPORT_TODAY,
	EXIT_MENU
};

class Admin
{
	std::string username = "";
	std::string password = "";
public:
	void getAdminData();
	bool validateLogin(); // string_view makes string read only to save memory

	std::string getUserName();

	bool mainMenuHandler();

	bool displayCustomersWhoOrdered(); // displays all the customer who ordered
	bool displayOrdersOfCustomer(int id);

	void displayDiscountReqests();
	// display regular customers
	void displayRegularCustomers();

	// reads from report file
	void viewFinanceReport();

	void approveRequest(int id);
	void denyRequest(int id);
};

#endif // !ADMIN
