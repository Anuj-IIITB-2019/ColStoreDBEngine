#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "table_menu.cpp"

using namespace std;

class Database
{
private:
	string directory_path;	//variable to store path of working directory

public:
	//Function to handle database functionalities
	Database()
	{
		char ans = 'y';
		do
		{
			int ch;
			string db_name;
			bool cr = 0;

			string str = "DATABASE MENU";
			int console_width = 120;
			int len = str.length();
			if (len % 2 == 0)
				str += " ";
			cout << setw((console_width / 2) + len / 2) << right << str << endl;

			cout << "1. Display Database List\n2. Create Database\n3. Delete Database\n4. Use Database\n";
			cout << "Enter Choice::";
			cin >> ch;
			directory_path = "Database";
			string temp_path = "";
			switch (ch)
			{
				case 1:
				{
					str = "DATABASE LIST";
					len = str.length();
					if (len % 2 == 0)
						str += " ";
					cout << setw((console_width / 2) + len / 2) << right << str << endl;

					display_database();
				}
				break;
				case 2:
				{
					while (cr != 1)
					{
						cin.ignore();
						cout << "Enter Database name to be created::\n";
						getline(cin, db_name);
						cr = create_database(db_name);
						if (cr == 1)
							cout << "Database Created Successfully!\n";
					}
				}
				break;
				case 3:
				{
					cin.ignore();
					cout << "Enter Database name to be deleted::\n";
					getline(cin, db_name);
				}
				break;
				case 4:
				{
					cin.ignore();
					cout << "Enter Database to use::\n";
					getline(cin, db_name);
					temp_path = directory_path + "/" + db_name;

					DIR *folder;
					folder = opendir(temp_path.c_str());
					if (folder == NULL)
					{
						perror("");
						return;
					}

					Table obj = Table(temp_path);
				}
				break;
			}
			cout << "Enter to continue(y):";
			cin >> ans;

		} while (ans == 'y');
	}

	//Function to display list of databases
	void display_database()
	{
		DIR *folder;
		struct dirent *entry;
		int files = 0;

		folder = opendir(directory_path.c_str());

		if (folder == NULL)
		{
			perror("");
			return;
		}

		while ((entry = readdir(folder)))
		{
			files++;
			string temp = entry->d_name;
			if (temp != "." && temp != "..")
				cout << "\t" << entry->d_name << "\n";
		}

		closedir(folder);
	}

	//Function to create new database
	bool create_database(string db_name)
	{
		DIR *folder;
		struct dirent *entry;
		string path = directory_path + "/" + db_name;
		folder = opendir(path.c_str());
		if (folder == NULL)
		{
			mkdir(path.c_str());
			return 1;
		}
		else
		{
			cout << "Database Already Exists!!\n";
			return 0;
		}
	}
};

//Execution starts from here
int main()
{
	Database obj = Database();
	return 0;
}
