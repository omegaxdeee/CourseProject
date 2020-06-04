//Server
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#pragma warning(disable: 4996)
SOCKET newConnection;
#define BUFF 512
#define k 3

class Cesar {
public:
	std::string encryption(std::string original) {
		std::string TC;
		for (int i = 0; i < (original.length()); i++) {
			TC = TC + (char)((original[i] + k));
		}
		return TC;
	}
	std::string decryption(std::string encOriginal) {
		std::string UnTC;
		for (int i = 0; i < (encOriginal.length()); i++) {
			UnTC = UnTC + (char)((encOriginal[i] - k));
		}
		return UnTC;
	}

};

class WorkWithFile {
private:
	vector<string> clients;
	vector<string> salons;
	Cesar shyfr;
public:
	void sendClients() {
		ifstream clients("Clients.txt");
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		while (!clients.eof()) {
			clients >> login >> pass >> name >> surname;
			strncpy(login, shyfr.decryption(login).c_str(), sizeof(login));
			strncpy(pass, shyfr.decryption(pass).c_str(), sizeof(pass));
			this->clients.push_back(string(login) + " " + string(pass) + " " + string(name) + " " + string(surname));
		}
		clients.close();
		char countClients[BUFF];
		itoa(this->clients.size(), countClients, 10);
		send(newConnection, countClients, sizeof(countClients), NULL);//отправить количество клиентов
		vector<string>::iterator it;
		for (it = this->clients.begin(); it != this->clients.end(); it++) {
			//Отправка записей клиентов на клиент
			char clientInfo[BUFF];
			strncpy(clientInfo, it->c_str(), sizeof(clientInfo));
			send(newConnection, clientInfo, sizeof(clientInfo), NULL);
		}
		this->clients.clear();
	}
	void sendSalons() {
		string salon;
		ifstream fileSalons("AvtoSalons.txt");
		while (getline(fileSalons, salon)) {
			salons.push_back(salon);
		}
		char portCount[BUFF];
		itoa(salons.size(), portCount, 10);
		send(newConnection, portCount, sizeof(portCount), NULL);
		vector<string>::iterator it;
		for (it = this->salons.begin(); it != this->salons.end(); it++) {
			char salonInfo[BUFF];
			strncpy(salonInfo, it->c_str(), sizeof(salonInfo));
			send(newConnection, salonInfo, sizeof(salonInfo), NULL);
		}
		this->salons.clear();
	}
	void recvClients() {
		char countClient[BUFF];
		recv(newConnection, countClient, sizeof(countClient), NULL);
		if (atoi(countClient) == 0)
		{
			ofstream clients("Clients.txt");
			clients.close();
			return;
		}
		ofstream clients("Clients.txt");
		char clientInfo[BUFF];
		recv(newConnection, clientInfo, sizeof(clientInfo), NULL);
		char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
		strncpy(login, strtok(clientInfo, " "), sizeof(login));
		strncpy(pass, strtok(NULL, " "), sizeof(pass));
		strncpy(name, strtok(NULL, " "), sizeof(name));
		strncpy(surname, strtok(NULL, " "), sizeof(surname));
		clients << shyfr.encryption(login) << " " << shyfr.encryption(pass) << " " << name << " " << surname;
		for (int i = 1; i < atoi(countClient); i++) {
			recv(newConnection, clientInfo, sizeof(clientInfo), NULL);
			char login[BUFF], pass[BUFF], name[BUFF], surname[BUFF];
			strncpy(login, strtok(clientInfo, " "), sizeof(login));
			strncpy(pass, strtok(NULL, " "), sizeof(pass));
			strncpy(name, strtok(NULL, " "), sizeof(name));
			strncpy(surname, strtok(NULL, " "), sizeof(surname));
			clients << endl;
			clients << shyfr.encryption(login) << " " << shyfr.encryption(pass) << " " << name << " " << surname;
		}
		clients.close();
		this->clients.clear();
	}
	void recvSalons() {
		char answer_count[BUFF];
		recv(newConnection, answer_count, sizeof(answer_count), NULL);
		if (strcmp(answer_count, "-1") == 0) {
			ofstream avtoSalons("AvtoSalons.txt");
			avtoSalons.close();
			return;
		}

		int count = atoi(answer_count);
		char dataSalons[BUFF];
		ofstream avtoSalons("AvtoSalons.txt");
		recv(newConnection, dataSalons, sizeof(dataSalons), NULL);
		avtoSalons << dataSalons;
		for (int portNum = 1; portNum < count; portNum++) {
			recv(newConnection, dataSalons, sizeof(dataSalons), NULL);
			avtoSalons << endl;
			avtoSalons << dataSalons;
		}
		avtoSalons.close();
	}

};

class Experts {
private:
	int rows;//количество услуг в сервисе (по умолчанию 5)
	int strings;//колчиество экспертов
	double** marks;
	int** b;
	int* C;
	double* V;
	double W;
	int* result;
public:
	void getMarks() {
		this->rows = 5;
		//cout << "Выставление оценок" << endl;
		char countOfExperts[BUFF];
		recv(newConnection, countOfExperts, sizeof(countOfExperts), NULL);
		//cout << "count of experts" << atoi(countOfExperts) << endl;
		this->strings = atoi(countOfExperts);
		marks = new double* [atoi(countOfExperts)];
		b = new int* [atoi(countOfExperts)];
		for (int i = 0; i < atoi(countOfExperts); i++) {
			char expertMarks[BUFF];
			marks[i] = new double[rows];
			b[i] = new int[rows];
			recv(newConnection, expertMarks, sizeof(expertMarks), NULL);
			marks[i][0] = atof(strtok(expertMarks, " "));
			for (int markNum = 1; markNum < rows; markNum++) {
				marks[i][markNum] = atof(strtok(NULL, " "));
			}
		}
		preferences();
	}
	void preferences() {
		int sum = 0;
		double S2 = 0;
		double* S;
		C = new int[rows];
		V = new double[rows];
		S = new double[rows];
		result = new int[rows];
		for (int i = 0; i < rows; i++) {
			C[i] = 0;
			V[i] = 0;
			S[i] = 0;
			result[i] = 0;
		}

		//Создание модицифированной матрици
		for (int i = 0; i < strings; i++) {
			double norm = 0;
			for (int j = 0; j < rows; j++) {
				norm += marks[i][j];
			}
			for (int j = 0; j < rows; j++) {
				marks[i][j] = double(double(marks[i][j]) / double(norm));
			}
		}

		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < strings; i++) {
				V[j] += marks[i][j];
			}
			V[j] = double(V[j] / this->strings);
		}

	}
	void sendMarks() {
		string sendMarks;
		for (int i = 0; i < 4; i++) {
			sendMarks += to_string(V[i]) + " ";
		}
		sendMarks += to_string(V[4]);
		char marks[BUFF];
		strncpy(marks, sendMarks.c_str(), sizeof(marks));
		send(newConnection, marks, sizeof(marks), NULL);

	}
	double getW() {
		return W;
	}
};

WorkWithFile database;


void menu_admin() {
	//cout << "Открыто меню админа" << endl;
	database.sendClients();
	database.sendSalons();

	char answer[BUFF];
	while (true) {
		recv(newConnection, answer, sizeof(answer), NULL);
		int choice = atoi(answer);
		//cout << "choice - " << choice << endl;
		char answer_count[BUFF]; // -1 для выхода, а иначе количество и дальше
		switch (choice) {
		case(1):
		{
			database.recvSalons();
		}
		break;
		case(2):
		{
			//cout << "Меню работы с пользователми" << endl;
			bool flag = FALSE;
			while (flag == FALSE) {
				char clientChoice[BUFF];
				recv(newConnection, clientChoice, sizeof(clientChoice), NULL);
				switch (atoi(clientChoice)) {
				case(1):
					database.recvClients();
					break;
				case(3):
					database.recvClients();
					break;
				case(4):
					flag = TRUE;
					break;
				}

			}
		}
		break;
		case(3):
		{
			double W;
			Experts expert;

			expert.getMarks();

			//Отправить оценки
			expert.sendMarks();
			database.recvSalons();

		}
		break;
		case(5):
		{
			database.recvSalons();
		}
		break;
		case(6):
		{
			database.recvSalons();
		}
		break;
		case(8):
		{
			return;
		}
		break;
		}
	}
}

void menu_client() {
	//cout << "Открыто меню пользователя" << endl;
	database.sendClients();
	database.sendSalons();

	char answer[BUFF];
	while (true) {
		recv(newConnection, answer, sizeof(answer), NULL);
		int choice = atoi(answer);
		//cout << "choice - " << choice << endl;
		char answer_count[BUFF]; // -1 для выхода, а иначе количество и дальше
		switch (choice) {
		case(1):
		{
			database.recvSalons();
		}
		break;
		case(3):
		{
			database.recvSalons();
		}
		break;
		case(4):
		{
			database.recvSalons();
		}
		break;
		case(5):
		{
			database.recvClients();
		}
		break;
		case(7):
		{
			return;
		}
		break;
		}
	}
}


void Admin() {
	Cesar shyfr;
	char recvLogin[BUFF], recvPass[BUFF];
	recv(newConnection, recvLogin, sizeof(recvLogin), NULL);
	recv(newConnection, recvPass, sizeof(recvPass), NULL);
	std::string login, pass, name, surname;
	bool checked = FALSE;
	std::ifstream infile("Admins.txt");
	while (!infile.eof()) {
		infile >> login >> pass >> name >> surname;
		login = shyfr.decryption(login);
		pass = shyfr.decryption(pass);
		//std::cout << "login - " << login << " pass - " << pass << std::endl;
		//std::cout << "recvLogin - " << recvLogin << " recvPass - " << recvPass << std::endl;
		if (login == std::string(recvLogin) && pass == std::string(recvPass)) {
			send(newConnection, "200", sizeof("200"), 0);
			checked = TRUE;
			break;
		}
	}
	infile.close();
	if (!checked) {
		send(newConnection, "404", sizeof("404"), 0);
		return;
	}
	cout << "Admin вошел" << endl;
	menu_admin();
	cout << "Admin вышел" << endl;
	return;
}

void Client() {
	Cesar shyfr;
	char recvLogin[BUFF], recvPass[BUFF];
	recv(newConnection, recvLogin, sizeof(recvLogin), NULL);
	recv(newConnection, recvPass, sizeof(recvPass), NULL);
	std::string login, pass, name, surname;
	bool checked = FALSE;
	std::ifstream infile("Clients.txt");
	while (!infile.eof()) {
		infile >> login >> pass >> name >> surname;
		login = shyfr.decryption(login);
		pass = shyfr.decryption(pass);
		/*std::cout << "login - " << login << " pass - " << pass << std::endl;
		std::cout << "recvLogin - " << recvLogin << " recvPass - " << recvPass << std::endl;*/
		if (login == std::string(recvLogin) && pass == std::string(recvPass)) {
			send(newConnection, "200", sizeof("200"), 0);
			checked = TRUE;
			break;
		}
	}
	infile.close();
	if (!checked) {
		send(newConnection, "404", sizeof("404"), 0);
		return;
	}
	cout << "Пользователь выполнил(а) вход под именем "<< name << endl;
	char client_name[BUFF];
	strncpy(client_name, name.c_str(), sizeof(client_name));
	send(newConnection, client_name, sizeof(client_name), NULL);
	menu_client();
	cout << "Пользователь " << name << " вышел(ла)" << endl;
	return;
}

int main(int argc, char* argv[]) {
	system("chcp 1251 >> null");
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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);


	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
	}
	else {
		std::cout << "Client Connected!\n";
		while (true)
		{
			char result[BUFF];
			recv(newConnection, result, sizeof(result), 0);
			int menu = atoi(result);
			switch (menu) {
			case(1):
				Admin();
				break;
			case(2):
				Client();
				break;
			}
			if (menu == 3) {
				break;
			}
		}

	}


	//system("pause");
	return 0;
}
