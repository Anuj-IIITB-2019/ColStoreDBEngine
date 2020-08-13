#include <bits/stdc++.h>
#include <dirent.h>
#include "insert_row.cpp"
#include "query.cpp"
#include "aggregate.cpp"
#include <sys/stat.h>

using namespace std;

class Run_Query
{
private:
	vector<string> column_list;	//stores list of all columns of selected table

public:
	string table_path;
	Run_Query(string path)
	{
		table_path = path;
		Menu();
	}

	//Function to display query menu
	void Menu()
	{
		int ch;
		string str = "QUERY MENU";
		int console_width = 120;
		int len = str.length();
		if (len % 2 == 0)
			str += " ";
		cout << setw((console_width / 2) + len / 2) << right << str << endl;
		cout << "1. Insert Data in Table \n2. Select Query\n3. Aggregate Query\n";
		cout << "Enter Choice::";
		cin >> ch;
		string ans = "y";
		string table_name = "", column_name = "";
		while (ans == "y")
		{
			Query q;
			switch (ch)
			{
				case 1:
				{
					cin.ignore();
					cout << "Enter table_name::\n";
					getline(cin, table_name);

					string ans = "y";
					while (ans == "y")
					{

						DIR *folder;
						folder = opendir((table_path + "/" + table_name).c_str());
						if (folder == NULL)
						{
							cout << "Table does not exist\n";
							cout << "Enter correct table name\n";
							getline(cin, table_name);
						}
						else
							ans = "n";
					}

					CreateFiles(table_path + "/" + table_name);
				}
				break;
				case 2:
				{
					cin.ignore();
					cout << "Enter table_name::\n";
					getline(cin, table_name);

					q.Select(table_path + "/" + table_name, table_name);
				}
				break;
				case 3:
				{
					string check_col;
					struct stat buf;
					cin.ignore();
				tabname:
					cout << "Enter table_name::\n";
					getline(cin, table_name);

					check_col = table_path + "/" + table_name;
					if (stat(check_col.c_str(), &buf) != 0)
					{
						cout << "Invalid table name\n";
						goto tabname;
					}

					column_list = q.ColumnNames(table_path + "/" + table_name);
					cout << "\n";
					for (int i = 0; i < column_list.size(); i++)
						cout << i + 1 << ":" << column_list[i] << "\t";

					cout << "\n";

				colname:
					cout << "Enter Column Name::\n";
					getline(cin, column_name);
					check_col = table_path + "/" + table_name + "/" + column_name + ".txt";
					if (stat(check_col.c_str(), &buf) != 0)
					{
						cout << "Invalid column name\n";
						goto colname;
					}
					aggregate a = aggregate(column_name, table_path + "/" + table_name);
				}	
				break;
			}
			cout << "Do You want to keep running query::\n";
			cin >> ans;
		}
	}
};
