//Client
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <exception>
#include <string>
#include <iomanip>

#define BUFF 512
#pragma warning(disable: 4996)
SOCKET Connection;
using namespace std;

void term_func();

class Client {
protected:
	char name[BUFF];
	char surname[BUFF];
	char login[BUFF];
	char password[BUFF];
public:
	Client() {
		std::strncpy(this->name, "", sizeof(this->name));
		std::strncpy(this->surname, "", sizeof(this->surname));
		std::strncpy(this->login, "", sizeof(this->login));
		std::strncpy(this->password, "", sizeof(this->password));
	};
	void addSalon();
	void addClient(char name[BUFF], char surname[BUFF], char login[BUFF], char password[BUFF]) {
		std::strncpy(this->name, name, sizeof(this->name));
		std::strncpy(this->surname, surname, sizeof(this->surname));
		std::strncpy(this->login, login, sizeof(this->login));
		std::strncpy(this->password, password, sizeof(this->password));
	}
	void editSalon();
	void delSalon();
	void changePass(char pass[BUFF]) {
		strncpy(this->password, pass, sizeof(this->password));
	}
	string getName() {
		return string(name);
	};
	string getSurname() {
		return string(surname);
	};
	string getLogin() {
		return string(login);
	};
	string getPass() {
		return string(password);
	};
	string getInfo() {
		string result;
		result += string(login) + " " + string(password) + " " + string(name) + " " + string(surname);
		return result;
	}
};

//unsigned int Client::id = 0;
class Admin : public Client {
public:
	Admin() {
		std::strncpy(this->login, "", sizeof(this->login));
		std::strncpy(this->password, "", sizeof(this->password));
	}
	void setAutorisation(const char login[BUFF], const char password[BUFF]) {
		std::strncpy(this->login, login, sizeof(this->login));
		std::strncpy(this->password, password, sizeof(this->password));
	}
	void setMarks();
	void calculate();
	void addClient();
	void generateResalon();
};

class Service {
private:
	string name;
	double expenses;// затраты на услугу
	double perInterest;// процент заинтересованности относительно всех услуг
	double profit; // прибыль
	int countEmployee; // количество сотрудников
public:
	Service(string name) {
		this->name = name;
	};
	void setExpenses(double expenses) {
		this->expenses = expenses;
	}
	void setPerInterest(double perInterest) {
		this->perInterest = perInterest;
	}
	void setProfit(double profit) {
		this->profit = profit;
	}
	void setCountEmployee(int count) {
		this->countEmployee = count;
	}
	string getName() {
		return name;
	}
	string getInfo() {
		string result;
		result += to_string(expenses) + " " + to_string(perInterest) + " "
			+ to_string(profit) + " " + to_string(countEmployee);
		return result;
	}
	void getInfoInTable(int id) {
		cout << setw(1) << "|" << setw(4) << id << setw(1) << "|" <<
			setw(25) << this->name << setw(5) << "|" <<
			setw(8) << this->expenses << setw(5) << "|" <<
			setw(15) << this->perInterest << setw(12) << "|" <<
			setw(8) << this->profit << setw(5) << "|" <<
			setw(12) << this->countEmployee << setw(11) << "|";
	}
	void getInfoInWholeTable(int id) {
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "id" << setw(1) << "|" <<
			setw(25) << "Название услуги" << setw(5) << "|" <<
			setw(8) << "Затраты" << setw(5) << "|" <<
			setw(25) << "Процент заинтересованных" << setw(2) << "|" <<
			setw(8) << "Прибыль" << setw(5) << "|" <<
			setw(20) << "Количество рабочих" << setw(3) << "|" << endl;
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		this->getInfoInTable(id);
		cout << endl;
		cout << setw(112) << setfill('=') << "=" << endl;
		cout << setfill(' ');
	}
	string getExpenses() {
		return to_string(this->expenses);
	}
	string getPerInterest() {
		return to_string(this->perInterest);
	}
	string getProfit() {
		return to_string(this->profit);
	}
	string getCountEmployee() {
		return to_string(this->countEmployee);
	}
};

class SalonException : public std::exception {
private:
	std::string m_error;
public:
	SalonException(std::string error) :m_error(error) {}
	const char* what() const noexcept {
		return m_error.c_str();
	}
};

string beautiful(string original) {
	for (int i = 0; i < original.length(); i++) {
		if (original[i] == ' ') {
			original[i] = '_';
		}
	}
	return original;
}

class Avtosalon {
private:
	char name[BUFF];
	char owner_name[BUFF];
	std::vector<Service> services;
	bool isChecked = false;
	double marks[5];
public:
	Avtosalon(const char owner_name[BUFF]) {
		strncpy(this->name, "", sizeof(this->name));
		strncpy(this->owner_name, owner_name, sizeof(this->owner_name));
		for (int i = 0; i < 5; i++) marks[i] = 0;
	};
	void setMarks(double marks[5]) {
		for (int i = 0; i < 5; i++) {
			this->marks[i] = marks[i];
		}
	}
	bool checkMarks(int expertMark, int marks[5]) {
		for (int i = 0; i < 5; i++) {
			if (marks[i] == expertMark) return true;
		}
		return false;
	}
	void setMarks() {
		system("cls");
		int expertsCount = 0;
		while (true) {
			try
			{
				cout << "Введите количество экспертов: ";
				cin >> expertsCount;
				if (!cin.good() || expertsCount < 2 || expertsCount > 10) {
					throw SalonException("Количество экспертов может быть от 2 до 10");
				}

				break;
			}
			catch (SalonException& error)
			{
				cout << error.what() << endl;
				cin.clear();
				rewind(stdin);
				cout << "Попробуйте еще раз" << endl;
			}
		}
		char countOfExperts[BUFF];
		itoa(expertsCount, countOfExperts, sizeof(countOfExperts));
		send(Connection, countOfExperts, sizeof(countOfExperts), NULL);
		for (int i = 0; i < expertsCount; i++) {
			system("cls");
			int expertMarks[5];
			for (int i = 0; i < 5; i++) {
				expertMarks[i] = -1;
			}
			vector<Service>::iterator itService;
			itService = services.begin();
			for (int serviceNum = 0; serviceNum < 5; serviceNum++) {
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				cout << setw(1) << "|" << setw(4) << "id" << setw(1) << "|" <<
					setw(25) << "Название услуги" << setw(5) << "|" <<
					setw(8) << "Затраты" << setw(5) << "|" <<
					setw(25) << "Процент заинтересованных" << setw(2) << "|" <<
					setw(8) << "Прибыль" << setw(5) << "|" <<
					setw(20) << "Количество рабочих" << setw(3) << "|" << endl;
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				itService->getInfoInTable(serviceNum + 1);
				cout << endl;
				cout << setw(112) << setfill('=') << "=" << endl;
				cout << setfill(' ');
				while (true) {
					try
					{
						cout << "Введите оценку для этого сервиса: ";
						int mark;
						cin >> mark;
						if (!cin.good() || mark < 1 || mark > 10 || checkMarks(mark, expertMarks))
							throw SalonException("Ошибка! Оценка должна быть в пределах от 1 до 10 и не повторятся");
						expertMarks[serviceNum] = mark;
						break;
					}
					catch (SalonException& error)
					{
						cout << error.what() << endl << "Попробуйте еще раз!" << endl;
						cin.clear();
						rewind(stdin);

					}
				}


				itService++;
			}
			char sendMarksString[BUFF];
			strcpy(sendMarksString, "");
			for (int i = 0; i < 4; i++) {
				char mark[BUFF];
				itoa(expertMarks[i], mark, 10);
				strcat(sendMarksString, mark);
				strcat(sendMarksString, " ");
			}

			char mark[BUFF];
			itoa(expertMarks[4], mark, 10);
			strcat(sendMarksString, mark);
			send(Connection, sendMarksString, sizeof(sendMarksString), NULL);
		}

	}
	void addService(string service_name) {
		double expenses;// затраты на услугу
		double perInterest;// процент заинтересованности относительно всех услуг
		double profit; // прибыль
		int countEmployee; // количество сотрудников
		Service temp(service_name);
		int choice;
		while (true)
		{
			try {

				cout << "\tВведите затраты на услугу: "; cin >> expenses;
				if (expenses < 0) throw SalonException("Затраты не могут быть меньше 0!");
				cout << "\tВведите процент заинтересованности на услугу (0-100%): "; cin >> perInterest;
				if (perInterest < 0 || perInterest > 100) throw SalonException("Введите процент в промежутке от 0 до 100!");
				cout << "\tВведите прибыль на услугу: "; cin >> profit;
				if (profit < 0) throw SalonException("Прибыль не может быть меньше 0!");
				cout << "\tВведите количество сотрудников на услугу: "; cin >> countEmployee;
				if (countEmployee < 0) throw SalonException("Количество сотрудников не может быть меньше 0!");
				temp.setExpenses(expenses);
				temp.setPerInterest(perInterest);
				temp.setProfit(profit);
				temp.setCountEmployee(countEmployee);
				services.push_back(temp);
				break;
			}
			catch (SalonException& salon_error)
			{
				cout << salon_error.what();
				cout << "Попробовать еще раз?\n1. Да\n0. Нет" << endl;
				while (!scanf_s("%d", &choice) || (choice < 0 || choice > 1))
				{
					rewind(stdin);
					cout << "Ошибка. Введите 1 или 0!\n";
				}
				if (choice == 0) {
					throw 1;
					cin.clear();
					rewind(stdin);
					return;
				}
			}
			catch (...) {
				term_func();
			}
			cin.clear();
			rewind(stdin);
		}


	};
	void createSalon() {
		cin.clear();
		rewind(stdin);
		int choice;
		while (true)
		{
			try
			{
				cout << "Введите название автосалона: ";
				char Salonname[BUFF];
				cin.getline(Salonname, BUFF, '\n');
				rewind(stdin);
				strncpy(this->name, beautiful(Salonname).c_str(), sizeof(this->name));
				if (strlen(this->name) < 2) throw SalonException("Имя автосалона слишком короткое!\n");
				cout << "Заполнение информации об услуге <Наличие Trade-In>\n";
				addService("Наличие Trade-In");
				cout << "Заполнение информации об услуге <Проведение выставок>\n";
				addService("Проведение выставок");
				cout << "Заполнение информации об услуге <Регистрация автомобиля>\n";
				addService("Регистрация автомобиля");
				cout << "Заполнение информации об услуге <Покупка автомобиля>\n";
				addService("Покупка автомобиля");
				cout << "Заполнение информации об услуге <Аренда автомобиля>\n";
				addService("Аренда автомобиля");
				break;
			}
			catch (SalonException& salon_error)
			{
				cout << salon_error.what();
				cout << "Попробовать еще раз?\n1. Да\n0. Нет" << endl;
				while (!scanf_s("%d", &choice) || (choice < 0 || choice > 1))
				{
					rewind(stdin);
					cout << "Ошибка. Введите 1 или 0!\n";
				}
				if (choice == 0) {
					throw 1;
					cin.clear();
					rewind(stdin);
					return;
				}
			}
			catch (int) {
				throw 1;
				cin.clear();
				rewind(stdin);
				return;
			}
			catch (...) {
				term_func();
			}
			cin.clear();
			rewind(stdin);
		}
	}
	string getInfo() {
		string infoSalon;

		infoSalon += beautiful(string(name)) + " " + string(owner_name) + " " + (isChecked ? "1" : "0") + " ";
		for (Service temp : services) {
			infoSalon += temp.getInfo() + " ";
		}
		for (int i = 0; i < 9; i++) {
			infoSalon += to_string(marks[i]) + " ";
		}
		infoSalon += to_string(marks[4]);
		return infoSalon;
	}
	void addService(const char service_name[BUFF], double expenses, double perInterest, double profit, int countEmployee) {
		Service temp(service_name);
		temp.setExpenses(expenses);
		temp.setPerInterest(perInterest);
		temp.setProfit(profit);
		temp.setCountEmployee(countEmployee);
		services.push_back(temp);
	}
	void isCheckedSet(int isChecked) {
		this->isChecked = isChecked;
	}
	void setSalonname(const char name[BUFF]) {
		strncpy(this->name, name, sizeof(this->name));
	}
	string getName() {
		return string(name);
	}
	string getIsChecked() {
		return this->isChecked ? "Проверено" : "Не проверено";
	}
	string getOwnerName() {
		return string(owner_name);
	}
	string getMarks() {
		string marks;
		for (int mark = 0; mark < 4; mark++) {
			marks += to_string(this->marks[mark]) + " ";
		}
		marks += to_string(this->marks[4]);
		return marks;
	}
	void changeInfo() {
		char choice;
		cout << "1. Изменить имя автосалона" << endl;
		cout << "2. Изменить параметры сервиса" << endl;
		while (true) {
			cin.clear();
			rewind(stdin);
			choice = _getch();
			if (choice == '1' || choice == '2') break;
		}
		switch (choice) {
		case('1'):
		{
			//Изменить имя автосалона
			while (true) {
				try
				{
					cout << "\tВведите название автосалона: ";
					char Salonname[BUFF];
					cin.getline(Salonname, BUFF, '\n');
					rewind(stdin);
					strncpy(this->name, beautiful(Salonname).c_str(), sizeof(this->name));
					if (strlen(this->name) < 2) throw SalonException("\tИмя автосалона слишком короткое!\n");
					break;
				}
				catch (SalonException& error) {
					cout << error.what() << endl;
					cout << "\tПопробуйте еще раз!\nНажмите Enter..." << endl;
					cin.clear();
					rewind(stdin);
					cin.get();
				}
			}
		}
		break;
		case('2'):
		{
			//Изменить сервисы
			cout << "При изменении оценок сервисов потребуется повторная проверка экспертов\n";
			vector<Service>::iterator itService;
			itService = services.begin();
			for (int i = 0; i < 5; i++, itService++) {
				cout << "\t" << i + 1 << ". " << itService->getName() << endl;
			}
			itService = services.begin();
			char choice;
			while (true) {
				cin.clear();
				rewind(stdin);
				choice = _getch();
				if (choice >= 49 && choice <= 53) break;
			}
			advance(itService, (int(choice) - 49));


			double expenses;// затраты на услугу
			double perInterest;// процент заинтересованности относительно всех услуг
			double profit; // прибыль
			int countEmployee; // количество сотрудников
			while (true)
			{
				try {
					system("cls");
					itService->getInfoInWholeTable(int(choice - 48));
					cout << "\tВведите затраты на услугу: "; cin >> expenses;
					if (expenses < 0) throw SalonException("Затраты не могут быть меньше 0!");
					cout << "\tВведите процент заинтересованности на услугу (0-100%): "; cin >> perInterest;
					if (perInterest < 0 || perInterest > 100) throw SalonException("Введите процент в промежутке от 0 до 100!");
					cout << "\tВведите прибыль на услугу: "; cin >> profit;
					if (profit < 0) throw SalonException("Прибыль не может быть меньше 0!");
					cout << "\tВведите количество сотрудников на услугу: "; cin >> countEmployee;
					if (countEmployee < 0) throw SalonException("Количество сотрудников не может быть меньше 0!");
					itService->setExpenses(expenses);
					itService->setPerInterest(perInterest);
					itService->setProfit(profit);
					itService->setCountEmployee(countEmployee);
					break;
				}
				catch (SalonException& salon_error)
				{
					cout << salon_error.what();
					cout << "Попробуйте еще раз!\nНажмите Enter..." << endl;
					cin.clear();
					rewind(stdin);
					cin.get();
				}
				catch (...) {
					term_func();
				}
				cin.clear();
				rewind(stdin);
			}
			this->isChecked = false;
			double defMarks[] = { 0,0,0,0,0 };
			this->setMarks(defMarks);
			system("cls");
			itService->getInfoInWholeTable(int(choice - 48));
			cout << "Нажмите Enter" << endl;
			cin.clear();
			rewind(stdin);
			cin.get();

		}
		break;
		}
	}
	void resalon(int id) {
		if (this->marks[0] == 0) {
			cout << "Нельзя сформировать отчет для непроверенного автосалона!\n";
			cout << "Нажмите Enter..." << endl;
			cin.clear();
			rewind(stdin);
			cin.get();
			return;
		}
		char file_name[BUFF];
		cin.clear();
		rewind(stdin);
		while (true) {
			try {
				cout << "Введите имя файла: ";
				cin.getline(file_name, BUFF, '\n');
				rewind(stdin);
				strncpy(file_name, beautiful(file_name).c_str(), sizeof(file_name));
				if (strlen(file_name) < 2) throw SalonException("Имя файла слишком короткое!\n");
				break;
			}
			catch (SalonException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте ещё раз!\nНажмите Enter...\n";
				cin.clear();
				rewind(stdin);
				cin.get();
			}
		}
		strcat(file_name, ".txt");
		fstream outfile;
		outfile.open(file_name, ios::out);
		outfile << "\t\tОтчет по автосалону " + string(this->name) << endl << endl;

		outfile << "\tid: " << id << endl;
		outfile << "\tНазвание: " << this->getName() << endl;
		outfile << "\tВладелец: " << this->getOwnerName() << endl;
		outfile << "\tПроверка от эксперта: " << this->getIsChecked() << endl;
		outfile << "\tОценки: " << this->getMarks() << endl << endl << endl;

		outfile << "\t\tСервисы автосалона" << endl << endl;

		vector<Service>::iterator itService;
		itService = services.begin();
		int i;
		for (itService = services.begin(), i = 1; itService != services.end(); itService++, i++) {
			outfile << i << ". " + itService->getName() << endl;
			outfile << "\t" << "Затраты: " << itService->getExpenses() << endl;
			outfile << "\t" << "Процент заинтересованности: " << itService->getPerInterest() << endl;
			outfile << "\t" << "Прибыль: " << itService->getProfit() << endl;
			outfile << "\t" << "Количество рабочих: " << itService->getCountEmployee() << endl;
		}
	}
};

class Company {
private:
	char companyName[BUFF];
	Admin admin;
	std::vector<Client> clients;
	std::vector<Avtosalon> salons;
	std::vector<Client>::iterator itClient;
	std::vector<Avtosalon>::iterator itSalon;
public:
	Company(const char companyName[BUFF]) {
		std::strncpy(this->companyName, companyName, sizeof(this->companyName));
		admin.setAutorisation("admin", "admin");

	};
	~Company() {
		clients.clear();
		salons.clear();
	}
	void sendsalons() {
		std::vector<Avtosalon>::iterator it;
		int countsalons = salons.size();
		char count[BUFF];
		itoa(countsalons, count, 10);
		send(Connection, count, sizeof(count), NULL);
		for (it = salons.begin(); it != salons.end(); it++) {
			char salonInfo[BUFF];
			strncpy(salonInfo, it->getInfo().c_str(), sizeof(salonInfo));
			send(Connection, salonInfo, sizeof(salonInfo), NULL);
		}
	}
	void sendClient() {
		char countClient[BUFF];
		itoa(clients.size(), countClient, 10);
		send(Connection, countClient, sizeof(countClient), NULL);
		for (itClient = clients.begin(); itClient != clients.end(); itClient++) {
			char clientInfo[BUFF];
			strncpy(clientInfo, itClient->getInfo().c_str(), sizeof(clientInfo));
			send(Connection, clientInfo, sizeof(clientInfo), NULL);
		}
	}
	void addSalon(const char Salonname[BUFF]) {
		Avtosalon temp(Salonname);
		try
		{
			temp.createSalon();
		}
		catch (int)
		{
			send(Connection, "-1", sizeof("-1"), NULL);
			return;
		}
		salons.push_back(temp);
		sendsalons();
	}
	void recvClient() {
		char clientCount[BUFF];
		recv(Connection, clientCount, sizeof(clientCount), NULL);
		for (int i = 0; i < atoi(clientCount); i++) {
			char clientInfo[BUFF];
			Client temp;
			recv(Connection, clientInfo, sizeof(clientInfo), NULL);
			if (strlen(clientInfo) < 4) return;
			char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
			strncpy(login, strtok(clientInfo, " "), sizeof(login));
			strncpy(pass, strtok(NULL, " "), sizeof(pass));
			strncpy(name, strtok(NULL, " "), sizeof(name));
			strncpy(surname, strtok(NULL, " "), sizeof(surname));
			temp.addClient(name, surname, login, pass);
			clients.push_back(temp);
		}

	}
	void recvsalons() {
		char salonCount[BUFF];
		recv(Connection, salonCount, sizeof(salonCount), NULL);
		for (int i = 0; i < atoi(salonCount); i++) {
			char salonInfo[BUFF];
			recv(Connection, salonInfo, sizeof(salonInfo), NULL);
			if (strlen(salonInfo) < 4) return;
			char Salonname[BUFF], owner_name[BUFF], isChecked[BUFF];
			strncpy(Salonname, strtok(salonInfo, " "), sizeof(Salonname));
			strncpy(owner_name, strtok(NULL, " "), sizeof(owner_name));
			strncpy(isChecked, strtok(NULL, " "), sizeof(isChecked));
			Avtosalon temp(owner_name);
			temp.setSalonname(Salonname);
			for (int service = 0; service < 5; service++) {
				double expenses = atoi(strtok(NULL, " "));
				double perInterest = atoi(strtok(NULL, " "));
				double profit = atoi(strtok(NULL, " "));
				int countEmployee = atoi(strtok(NULL, " "));
				switch (service) {
				case(0):
					temp.addService("Наличие Trade-In", expenses, perInterest, profit, countEmployee);
					break;
				case(1):
					temp.addService("Проведение выставок", expenses, perInterest, profit, countEmployee);
					break;
				case(2):
					temp.addService("Регистрация автомобиля", expenses, perInterest, profit, countEmployee);
					break;
				case(3):
					temp.addService("Покупка автомобиля", expenses, perInterest, profit, countEmployee);
					break;
				case(4):
					temp.addService("Аренда автомобиля", expenses, perInterest, profit, countEmployee);
					break;
				}
			}
			temp.isCheckedSet(atoi(isChecked));
			double marks[5];
			for (int i = 0; i < 5; i++) {
				marks[i] = atoi(strtok(NULL, " "));
			}
			temp.setMarks(marks);
			salons.push_back(temp);
		}
	}
	void printClientInfo() {
		int id = 0;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "id" << setw(4) << "|" <<
			setw(12) << "Имя" << setw(8) << "|" <<
			setw(12) << "Фамилия" << setw(8) << "|" <<
			setw(12) << "Логин" << setw(8) << "|" <<
			setw(12) << "Пароль" << setw(8) << "|" << endl;
		cout << setw(89) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		for (itClient = clients.begin(); itClient != clients.end(); itClient++, id++) {
			cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
				setw(12) << itClient->getName() << setw(8) << "|" <<
				setw(12) << itClient->getSurname() << setw(8) << "|" <<
				setw(12) << itClient->getLogin() << setw(8) << "|" <<
				setw(12) << itClient->getPass() << setw(8) << "|" << endl;
		}
		cout << setw(89) << setfill('=') << "=" << endl;
	};
	int getClientsSize() {
		return clients.size();
	}
	int getsalonsSize() {
		return salons.size();
	}
	void delClient() {
		int id;
		while (true) {
			try
			{
				cout << "Введите ID для удаления."; cin >> id;
				if (!cin.good() || id < 0 || (id > (clients.size() - 1))) throw SalonException("Неверный ID");
				break;
			}
			catch (SalonException& error)
			{
				cin.clear();
				rewind(stdin);
				cout << error.what() << endl << "Попробуйте еще раз!" << endl;

			}
		}
		itClient = clients.begin();
		advance(itClient, id);
		clients.erase(itClient);
	}
	void addClient() {
		Client temp;
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		while (true) {
			try {
				system("cls");
				cout << "Введите имя пользователя: "; cin >> name;
				if (strlen(name) < 2) throw SalonException("Имя пользователя слишком короткое!");
				for (int i = 0; i < clients.size(); i++) {
					if (strcmp(clients[i].getName().c_str(), name) == 0) {
						throw SalonException("Пользователь с таким именем уже зарегистрирован!");
					}
				}
				cout << "Введите фамилию пользователя: "; cin >> surname;
				if (strlen(surname) < 2) throw SalonException("Слишком короткая фамилия!");
				cout << "Введите логин пользователя: "; cin >> login;
				if (strlen(login) < 6) throw SalonException("Слишком короткий логин!");
				cout << "Введите пароль пользователя: "; cin >> pass;
				if (strlen(pass) < 6) throw SalonException("Слишком короткий пароль!");

				break;
			}
			catch (SalonException& error) {
				rewind(stdin);
				cout << error.what() << endl << "Попробуйте еще раз!\nНажмите Enter...\n";
				cin.clear();
				cin.get();
			}
		}
		temp.addClient(name, surname, login, pass);
		clients.push_back(temp);
	}
	void printSalonInfo() {
		int id = 0;
		cout << setw(114) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		cout << setw(1) << "|" << setw(4) << "ID" << setw(4) << "|" <<
			setw(15) << "Название" << setw(5) << "|" <<
			setw(15) << "Владелец" << setw(5) << "|" <<
			setw(12) << "Проверено" << setw(8) << "|" <<
			setw(12) << "Оценки" << setw(33) << "|" << endl;
		cout << setw(114) << setfill('=') << "=" << endl;
		cout << setfill(' ');
		for (itSalon = salons.begin(); itSalon != salons.end(); itSalon++, id++) {
			cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
				setw(15) << itSalon->getName() << setw(5) << "|" <<
				setw(15) << itSalon->getOwnerName() << setw(5) << "|" <<
				setw(12) << itSalon->getIsChecked() << setw(8) << "|" <<
				setw(12) << itSalon->getMarks() << setw(33) << endl;
		}
		cout << setw(114) << setfill('=') << "=" << endl;
	}
	void printSalonInfo(string owner_name) {
		int CountSalon = 0;
		for (itSalon = salons.begin(); itSalon != salons.end(); itSalon++) {
			if (itSalon->getOwnerName() == owner_name) CountSalon++;
		}
		if (CountSalon > 0) {
			int id = 0;
			cout << setw(114) << setfill('=') << "=" << endl;
			cout << setfill(' ');
			cout << setw(1) << "|" << setw(4) << "ID" << setw(4) << "|" <<
				setw(15) << "Название" << setw(5) << "|" <<
				setw(15) << "Владелец" << setw(5) << "|" <<
				setw(12) << "Проверено" << setw(8) << "|" <<
				setw(12) << "Оценки" << setw(33) << "|" << endl;
			cout << setw(114) << setfill('=') << "=" << endl;
			cout << setfill(' ');
			for (itSalon = salons.begin(); itSalon != salons.end(); itSalon++, id++) {
				if (itSalon->getOwnerName() == owner_name) {
					cout << setw(1) << "|" << setw(4) << id << setw(4) << "|" <<
						setw(15) << itSalon->getName() << setw(5) << "|" <<
						setw(15) << itSalon->getOwnerName() << setw(5) << "|" <<
						setw(12) << itSalon->getIsChecked() << setw(8) << "|" <<
						setw(12) << itSalon->getMarks() << setw(33) << endl;
				}
			}
			cout << setw(114) << setfill('=') << "=" << endl;
		}
		else {
			cout << "Автосалоны отсутствуют!" << endl;
		}
	}
	void setMarks() {
		int numSalon;
		double marks[5];
		while (true) {
			try
			{
				cout << "Введите ID автосалона: "; cin >> numSalon;
				if (!cin.good() || (numSalon < 0 || numSalon >= this->salons.size()))
					throw(SalonException("Введите ID от 0 до " + to_string((this->salons.size() - 1))));
				break;
			}
			catch (SalonException& error)
			{
				cout << error.what() << endl;
			}
		}
		itSalon = salons.begin();
		advance(itSalon, numSalon);
		char result[BUFF];
		itSalon->setMarks();
		system("cls");
		recv(Connection, result, sizeof(result), NULL);
		cout << result;
		//Выводим оценки и устанавливаем что salon проверен
		char mark[BUFF];
		strncpy(mark, strtok(result, " "), sizeof(mark));
		marks[0] = atof(mark);
		for (int i = 1; i < 5; i++) {
			strncpy(mark, strtok(NULL, " "), sizeof(mark));
			marks[i] = atof(mark);
		}
		itSalon->setMarks(marks);
		itSalon->isCheckedSet(1);
	}
	void delSalon() {
		int salonNum;
		while (true)
		{
			try {
				cout << "Введите номер автосалона: ";
				cin >> salonNum;
				if (!cin.good() || salonNum < 0 || salonNum >= salons.size())
					throw SalonException("Номер автосалона должен быть в пределах от 0 до " + to_string(salons.size() - 1));
				break;
			}
			catch (SalonException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз!" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itSalon = salons.begin();
		advance(itSalon, salonNum);
		salons.erase(itSalon);
	}
	void edit() {
		int salonNum;
		while (true)
		{
			try {
				cout << "Введите номер автосалона: ";
				cin >> salonNum;
				if (!cin.good() || salonNum < 0 || salonNum >= salons.size())
					throw SalonException("Номер автосалона должен быть в пределах от 0 до " + to_string(salons.size() - 1));
				break;
			}
			catch (SalonException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз!" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itSalon = salons.begin();
		advance(itSalon, salonNum);
		itSalon->changeInfo();
	}
	void createResalon() {
		int salonNum;
		while (true)
		{
			try {
				cout << "Введите номер автосалона: ";
				cin >> salonNum;
				if (!cin.good() || salonNum < 0 || salonNum >= salons.size())
					throw SalonException("Номер автосалона должен быть в пределах от 0 до " + to_string(salons.size() - 1));
				break;
			}
			catch (SalonException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз!" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itSalon = salons.begin();
		advance(itSalon, salonNum);
		itSalon->resalon(salonNum);
	}
	void createResalon(string owner_name) {
		int salonNum;
		while (true)
		{
			try {
				cout << "Введите номер автосалона: ";
				cin >> salonNum;
				if (salonNum >= salons.size()) throw SalonException("Автосалон с таким номером не найден!");
				if (cin.good()) {
					itSalon = salons.begin();

					advance(itSalon, salonNum);
					if (itSalon->getOwnerName() == owner_name) {
						break;
					}
					else throw SalonException("Автосалон с таким номером не найден!");
				}
				if (!cin.good())
					throw SalonException("Неизвестный ввод");
				break;
			}
			catch (SalonException& error) {
				cout << error.what() << endl;
				cout << "Попробуйте еще раз!" << endl;
				cin.clear();
				rewind(stdin);
			}
		}
		itSalon = salons.begin();
		advance(itSalon, salonNum);
		itSalon->resalon(salonNum);
	}
	void changeClientPass(string owner_name) {
		char pass[BUFF];
		for (itClient = clients.begin(); itClient != clients.end(); itClient++) {
			if (itClient->getName() == owner_name) {
				break;
			}
		}
		cout << "Введите новый пароль: ";
		cin.getline(pass, BUFF, '\n');
		itClient->changePass(pass);
	}
};


void menu(int type, char owner_name[BUFF]) {
	Company salon("ООО 'Автосалон'");
	salon.recvClient();
	salon.recvsalons();

	rewind(stdin);

	system("cls");
	rewind(stdin);
	std::cin.clear();
	if (type == 1) {
		rewind(stdin);
		bool flag1 = FALSE;
		while (flag1 == FALSE) {
			system("cls");
			std::cout << "\t\t\tМеню администратора" << std::endl;
			std::cout << "(1) Управление пользователями" << std::endl;
			std::cout << "(2) Добавление нового автосалона" << std::endl;
			std::cout << "(3) Просмотр существующих данных" << std::endl;
			std::cout << "(4) Редактирование данных" << std::endl;
			std::cout << "(5) Удаление данных" << std::endl;
			std::cout << "(6) Выставление оценок экспертов" << std::endl;
			std::cout << "(7) Создание отчета" << std::endl;
			std::cout << "(8) Выход в меню авторизации" << std::endl;
			rewind(stdin);
			char menu_2;
			menu_2 = _getch();
			switch (menu_2) {
			case('1'):
				//Работа с пользователями
			{
				send(Connection, "2", sizeof("2"), NULL);
				rewind(stdin);
				bool flag21 = FALSE;
				while (flag21 == FALSE) {
					system("cls");
					cout << "\t\t\tУправление пользователями" << endl;
					cout << "(1) Добавление нового пользователя" << endl;
					cout << "(2) Просмотр существующих пользователей" << endl;
					cout << "(3) Удаление пользователя" << endl;
					cout << "(4) Назад" << endl;
					char client_choice;
					client_choice = _getch();
					switch (client_choice) {
					case('1'):
					{
						send(Connection, "1", sizeof("1"), NULL);
						salon.addClient();
						salon.sendClient();
					}
					break;
					case('2'):
					{

						system("cls");
						if (salon.getClientsSize() == 0) {
							cout << "Пользователи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
							rewind(stdin);
							cin.clear();
							rewind(stdin);
							cin.get();
							continue;
						}
						send(Connection, "2", sizeof("2"), NULL);
						salon.printClientInfo();
						rewind(stdin);
						cin.get();
					}
					break;
					case('3'):
					{

						system("cls");
						if (salon.getClientsSize() == 0) {
							cout << "Пользователи отсутствуют" << endl << "Нажмите Enter чтобы продолжить..." << endl;
							rewind(stdin);
							cin.clear();
							rewind(stdin);
							cin.get();
							continue;
						}
						send(Connection, "3", sizeof("3"), NULL);
						salon.printClientInfo();
						salon.delClient();
						system("cls");
						salon.printClientInfo();
						rewind(stdin);
						cin.get();
						salon.sendClient();
					}
					break;
					case('4'):
					{
						send(Connection, "4", sizeof("4"), NULL);
						rewind(stdin);
						flag21 = TRUE;
					}
					break;
					}
				}


			}
			break;
			case('2'):
				//Добавление 
			{
				send(Connection, "1", sizeof("1"), NULL);
				salon.addSalon(owner_name);
			}
			break;
			case('3'):
				//Чтение данных
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				salon.printSalonInfo();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('4'):
				//Редактирование записей
			{
				send(Connection, "5", sizeof("5"), NULL);
				system("cls");
				salon.printSalonInfo();
				salon.edit();
				system("cls");
				salon.printSalonInfo();
				salon.sendsalons();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('5'):
				//Удаление записей
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "6", sizeof("6"), NULL);
				salon.printSalonInfo();
				cout << endl;
				salon.delSalon();
				system("cls");
				salon.printSalonInfo();
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				if (salon.getsalonsSize() == 0) {
					send(Connection, "-1", sizeof("-1"), NULL);
					break;
				}
				salon.sendsalons();
			}
			break;
			case('6'):
				//Выставление оценок
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "3", sizeof("3"), NULL);
				salon.printSalonInfo();
				salon.setMarks();
				salon.sendsalons();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
			}
			break;
			case('7'):
				//Формирование отчетов
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				salon.printSalonInfo();
				salon.createResalon();

			}
			break;
			case('8'):
				//Выход к авторизации
			{
				send(Connection, "8", sizeof("8"), NULL);
				rewind(stdin);
				flag1 = TRUE;
			}
			break;
			}
		}
	}

	if (type == 2) {
		rewind(stdin);
		bool flag1 = FALSE;
		while (flag1 == FALSE) {
			system("cls");
			std::cout << "\t\t\tДобро пожаловать, " << owner_name << "!" << std::endl;
			std::cout << "(1) Добавление нового автосалона" << std::endl;
			std::cout << "(2) Просмотр существующих данных" << std::endl;
			std::cout << "(3) Редактирование данных" << std::endl;
			std::cout << "(4) Удаление данных" << std::endl;
			std::cout << "(5) Создание отчета" << std::endl;
			std::cout << "(6) Смена пароля" << std::endl;
			std::cout << "(7) Выход в меню авторизации" << std::endl;
			rewind(stdin);
			char menu_2;
			menu_2 = _getch();
			switch (menu_2) {
			case('1'):
				//Добавление 
			{
				send(Connection, "1", sizeof("1"), NULL);
				salon.addSalon(owner_name);
			}
			break;
			case('2'):
				//Чтение данных
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				salon.printSalonInfo(owner_name);
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
				
			}
			break;
			case('3'):
				//Редактирование
			{
				send(Connection, "3", sizeof("3"), NULL);
				system("cls");
				salon.printSalonInfo();
				salon.edit();
				system("cls");
				salon.printSalonInfo();
				salon.sendsalons();
				cout << endl;
				cout << "Нажмите Enter чтобы продолжить" << endl;
				rewind(stdin);
				cin.clear();
				rewind(stdin);
				cin.get();
				
			}
			break;
			case('4'):
				//Удаление
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				send(Connection, "4", sizeof("4"), NULL);
				salon.printSalonInfo();
				cout << endl;
				salon.delSalon();
				system("cls");
				salon.printSalonInfo();
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				if (salon.getsalonsSize() == 0) {
					send(Connection, "-1", sizeof("-1"), NULL);
					break;
				}
				salon.sendsalons();
			}
			break;
			case('5'):
				//Формирование отчетов
			{
				system("cls");
				if (salon.getsalonsSize() == 0) {
					cout << "Записи отсутствуют" << endl << "Нажмите Enter чтобы продолжить" << endl;
					rewind(stdin);
					cin.clear();
					rewind(stdin);
					cin.get();
					continue;
				}
				salon.printSalonInfo(owner_name);
				salon.createResalon(owner_name);
			}
			break;
			case('6'):
				//Смена пароля
			{
				send(Connection, "6", sizeof("6"), NULL);
				system("cls");
				salon.changeClientPass(owner_name);
				cin.clear();
				rewind(stdin);
				cout << "Нажмите Enter чтобы продолжить " << endl; cin.get();
				salon.sendClient();
			}
			break;
			
			case('7'):
				//Выход к авторизации
			{
				send(Connection, "7", sizeof("7"), NULL);
				rewind(stdin);
				flag1 = TRUE;
			}
			break;
			}
		}
	}
}

void autorisation(const char companyName[BUFF]) {
	while (true) {
		system("cls");
		std::cout << "Добро пожаловать в " << companyName << "!" << std::endl;
		std::cout << "(1) Вход в качестве администратора" << std::endl;
		std::cout << "(2) Вход в качестве пользователя" << std::endl;
		std::cout << "(3) Выход" << std::endl;
		while (true) {
			rewind(stdin);
			std::cin.clear();
			char choice;
			choice = _getch();
			switch (choice) {
			case('1'):
			{
				send(Connection, "1", sizeof("1"), NULL);
				char login[BUFF], pass[BUFF];
				system("cls");
				while (true)
				{
					rewind(stdin);
					std::cin.clear();
					std::cout << "\t\t\tАвторизация администратора" << std::endl;
					std::cout << "Введите логин: "; std::cin >> login;
					std::cout << "Введите пароль: "; std::cin >> pass;
					if (strlen(login) < 3) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий логин. \nПопробуйте еще раз" << std::endl;
						continue;
					}
					if (strlen(pass) < 1) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий пароль. \nПопробуйте еще раз" << std::endl;
						continue;
					}
					break;
				}

				//std::cout << "login - " << login << " pass - " << pass << std::endl;
				char answer_code[BUFF];//200 or 404
				send(Connection, login, sizeof(login), NULL);
				send(Connection, pass, sizeof(pass), NULL);
				recv(Connection, answer_code, sizeof(answer_code), NULL);
				rewind(stdin);
				//std::cout << answer_code << std:: endl;
				//std::cin.get();
				if (atoi(answer_code) == 200) {
					char owner[BUFF];
					strncpy(owner, "Admin", sizeof(owner));
					menu(1, owner);
				}
			}
			break;
			case('2'):
			{

				send(Connection, "2", sizeof("2"), NULL);
				char login[BUFF], pass[BUFF];
				system("cls");
				while (true)
				{
					rewind(stdin);
					std::cin.clear();
					std::cout << "\t\t\tАвторизация пользователя" << std::endl;
					std::cout << "Введите логин: "; std::cin >> login;
					std::cout << "Введите пароль: "; std::cin >> pass;
					if (strlen(login) < 6) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий логин. \nМинимум 6 символов\nПопробуйте еще раз" << std::endl;
						continue;
					}
					if (strlen(pass) < 6) {
						system("cls");
						std::cout << "Ошибка! Слишком короткий пароль. \nМинимум 6 символов\nПопробуйте еще раз" << std::endl;
						continue;
					}
					break;
				}
				//std::cout << "login - " << login << " pass - " << pass << std::endl;
				char answer_code[BUFF];//200 or 404
				send(Connection, login, sizeof(login), NULL);
				send(Connection, pass, sizeof(pass), NULL);
				recv(Connection, answer_code, sizeof(answer_code), NULL);

				rewind(stdin);
				//std::cout << answer_code << std::endl;
				//std::cin.get();
				if (atoi(answer_code) == 200) {
					char client_name[BUFF];
					recv(Connection, client_name, sizeof(client_name), NULL);
					menu(2, client_name);
				}
			}
			break;
			case('3'):
			{
				send(Connection, "3", sizeof("3"), NULL);
				exit(0);
			}
			break;
			}
			break;
		}
	}
}

void term_func() {
	cout << "Произошла неизвестная ошибка" << endl;
	system("pause");
	exit(-1);
}

int main(int argc, char* argv[]) {
	system("chcp 1251 >> null");
	set_terminate(term_func);
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";



	autorisation("Автосалон");



	system("pause");
	return 0;
}