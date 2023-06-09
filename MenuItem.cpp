#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <iomanip>
#include "welcome.h"
#include "box.h"

MenuItem::MenuItem()
{
	id = 0;
	menuItemName = "";
	menuItemPrice = 0.00f;
}

int MenuItem::getId()
{
	return (id);
}

std::string MenuItem::getName()
{
	return (menuItemName);
}

float MenuItem::getPrice()
{
	return (menuItemPrice);
}
void MenuItem::showMenu()
{
	//gotoxy(40, 5);
	std::ifstream inf;
	inf.open(MENU_FILE, std::ios::in);

	if (!inf)
	{
		std::cout << "File could not be open! FILE_NAME: " << MENU_FILE << '\n';
	}
	else
	{
		std::cout << std::setfill(' ') << std::setw(5) << ' '
			<< std::setw(20) << ' '
			<< std::setw(10) << ' ' << std::setfill(' ') << std::endl;

		std::cout << std::setw(5) << std::left << "SN"
			<< std::setw(20) << std::left << "Item"
			<< std::setw(10) << std::left << "Price" << std::endl;

		std::cout << std::setfill('-') << std::setw(5) << '-'
			<< std::setw(20) << '-'
			<< std::setw(10) << '-' << std::setfill(' ') << std::endl;

		std::string line;
		while (std::getline(inf, line))
		{
			std::string menuItemName;
			float menuItemPrice;
			int id;

			int commaIndex1 = line.find(",");
			int commaIndex2 = line.find(",", commaIndex1 + 1);
			id = std::stoi(line.substr(0, commaIndex1));
			menuItemName = line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1);
			menuItemPrice = std::stof(line.substr(commaIndex2 + 1));

			std::cout << std::setw(5) << std::left << id
				<< std::setw(20) << std::left << menuItemName
				<< std::setw(10) << std::left << menuItemPrice << std::endl;
		}
	}
}




void MenuItem::inputData()
{
	gotoxy(40, 11);
	std::cout << "\nUpdate Or Add Item\n";
	std::cout << "\n";
	gotoxy(40, 12);
	std::cout << "Enter item id: ";
	std::cin >> id;
	gotoxy(40, 13);
	std::cout << "Enter item name: ";
	std::getline(std::cin >> std::ws, menuItemName);
	gotoxy(40, 14);
	std::cout << "Enter item price: ";
	std::cin >> menuItemPrice;
}

void MenuItem::updateMenu(MenuItem newMenuItem)
{

	std::ifstream inf;
	inf.open(MENU_FILE);

	std::ofstream outf;
	outf.open("RestaurantData/temp.txt", std::ios::app);

	bool entryFound{ false }; // if the provided menu item is found and is there to be updated

	if (inf.is_open())
	{
		std::string line;
		while (std::getline(inf, line))
		{
			int tID;
			std::string tName;
			float tPrice;
			int commaIndex1 = line.find(","); // "find" returns unsigned int while we're storing in signed int so possible loss of data
			int commaIndex2 = line.find(",", commaIndex1 + 1);

			tID = std::stoi(line.substr(0, commaIndex1));
			tName = line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1);
			tPrice = std::stof(line.substr(commaIndex2 + 1));

			if (tID == newMenuItem.getId())
			{
				outf << newMenuItem.getId() << ',' << newMenuItem.getName() << ',' << newMenuItem.getPrice() << '\n';
				entryFound = true;
			}
			else
			{
				outf << tID << ',' << tName << ',' << tPrice << '\n';
			}
		}

		if (!entryFound)
		{
			outf << newMenuItem.getId() << ',' << newMenuItem.getName() << ',' << newMenuItem.getPrice() << '\n';
		}

		inf.close();
		outf.close();

		std::remove("RestaurantData/Menu.txt");
		std::rename("RestaurantData/temp.txt", "RestaurantData/Menu.txt");
	}
}

MenuItem MenuItem::getItem(int itid)
{
	MenuItem theItem;
	std::ifstream inf;
	inf.open(MENU_FILE);

	if (inf.is_open())
	{
		std::string line;
		while (std::getline(inf, line))
		{
			int itemid;
			std::string name;
			float price;

			int commaIndex1 = line.find(",");
			int commaIndex2 = line.find(",", commaIndex1 + 1);

			itemid = std::stoi(line.substr(0, commaIndex1));
			name = line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1);
			price = std::stof(line.substr(commaIndex2 + 1));

			if (itid == itemid)
			{
				theItem.id = itemid;
				theItem.menuItemName = name;
				theItem.menuItemPrice = price;
			}
		}
	}
	inf.close();
	return theItem;
}