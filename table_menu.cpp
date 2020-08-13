#include <bits/stdc++.h>
#include <dirent.h>
#include "import_table.cpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include "run_query.cpp"
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

class Table
{
public:
	string database_path;

	Table(string db_path)
	{
		database_path = db_path;
		string ans = "y";
		while (ans == "y" || ans == "Y")
		{
			Menu();
			cout << "Do you want to continue(y/n)::";
			cin >> ans;
		}
	}

	//Function to diplay table menu
	void Menu()
	{
		int ch;
		string str = "TABLE MENU";
		int console_width = 120;
		int len = str.length();

		if (len % 2 == 0)
			str += " ";
		
		cout << setw((console_width / 2) + len / 2) << right << str << endl;
		cout << "1. Display Table List\n2. Create Table\n3. Import Table\n4. Run Query\n";
		cout << "Enter Choice::";
		cin >> ch;
		
		string table_name = "";
		switch (ch)
		{
			case 1: {
						str = "TABLE LIST";
						console_width = 120;
						len = str.length();
						if (len % 2 == 0)
							str += " ";
						cout << setw((console_width / 2) + len / 2) << right << str << endl;

						cout << "\t\t\t Table List\n";

						show_tables();
				    }break;
			case 2:	{
						cin.ignore();
						cout << "Enter Table Name :: ";
						getline(cin, table_name);

						create_table(table_name);
					}break;
			case 3:	{
						cin.ignore();
						cout << "Enter FILE NAME to import :: ";
						getline(cin, table_name);
						import_table(table_name);
					}break;
			case 4:	{
						cout << "RUN QUERY ::";
						system("cls");
						Run_Query obj = Run_Query(database_path);
					}break;
		}
	}

	//Function for importing table from .csv file
	void import_table(string tb_name)
	{
		string table;

		cin.ignore();
		cout << "Enter Table Name::";

		getline(cin, table);
		mkdir((database_path + "/" + table).c_str());
		string new_table_path = database_path + "/" + table;
		CreateFiles(tb_name, new_table_path);

		cout << "Creating Table.....\n";
		EnterRow(tb_name);
	}

	//Function to create new table inside current database
	void create_table(string tb_name)
	{
		string path = database_path + "/" + tb_name;

		DIR *folder;
		struct dirent *entry;
		folder = opendir(path.c_str());
		
		if (folder == NULL)
		{

			mkdir(path.c_str());
			path = path + "/";
			int num_col;
			cout << "Enter Number of Columns::";
			cin >> num_col;

			FILE *fp_col;
			fp_col = fopen((path + "/Column_decr.txt").c_str(), "a+");
			
			for (int i = 1; i <= num_col; i++)
			{
				string col_name, col_type;
				cin.ignore();
				cout << "Column " << i << " ::\nName::";
				getline(cin, col_name);
				cout << "\nType (i for int, v for varchar,c for char,d for double)::";

				cin >> col_type;
				if (col_type == "i" || col_type == "v" || col_type == "c" || col_type == "d")
				{
					fputs((col_name + ":" + col_type + "\n").c_str(), fp_col);
				}
				else
				{
					while (col_type != "i" && col_type != "v" && col_type != "c" && col_type != "d")
					{
						cout << "Enter Correct Type::";
						cin >> col_type;
					}
					fputs((col_name + ":" + col_type + "\n").c_str(), fp_col);
				}

				FILE *fp;
				fp = fopen((path + "/" + col_name + ".txt").c_str(), "w+");
				fclose(fp);
			}
			fclose(fp_col);
		}
		else
		{
			cout << "Table Already Exists!!\n";
			return;
		}
	}

	//Function to display list of all tables in current database
	void show_tables()
	{
		DIR *folder;
		struct dirent *entry;
		int files = 0;

		folder = opendir(database_path.c_str());
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
};
