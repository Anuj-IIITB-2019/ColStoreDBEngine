#include <bits/stdc++.h>

using namespace std;

vector<pair<string, string> > column_list_type;
string table_name_path;

//Function to fetch last key stored in key.txt file
int GenerateKey_runquery()
{
	fstream file;
	int lastkey = 0;
	string filename;

	filename = table_name_path + "/" + "key.txt";

	file.open(filename.c_str());

	while (file >> lastkey);

	file.close();

	return lastkey;
}

//Function to insert given row data into various column files
void InsertData_runquery(int primary_key, string row)
{
	int len = row.length(), j = 0;
	string value = "";
	for (int i = 0; i < len && j < column_list_type.size(); i++)
	{
		if (row[i] == ',')
		{
			if (value == "")
				value = "NULL";

			stringstream ss;
			ss << primary_key;

			string data = ss.str() + ":" + value;

			FILE *file;
			string file_name = table_name_path + "/" + column_list_type[j].first + ".txt";
			file = fopen(file_name.c_str(), "a+");
			fputs((data + "\n").c_str(), file);

			fclose(file);

			j++;

			value = "";
		}
		else
			value = value + row[i];
	}

	if (value == "")
		value = "NULL";

	stringstream ss;
	ss << primary_key;

	string data = ss.str() + ":" + value;

	FILE *file;
	string file_name = table_name_path + "/" + column_list_type[j].first + ".txt";
	file = fopen(file_name.c_str(), "a+");
	fputs((data + "\n").c_str(), file);

	fclose(file);
}

//Function to read rows one by one from user
void Enter_Data_Row()
{

	int key, i;
	char choice;
	string row;

	key = GenerateKey_runquery();
	key++;
	FILE *file;
	string file_name = table_name_path + "/key.txt";
	file = fopen(file_name.c_str(), "a+");

	do
	{
		cin.ignore();
		cout << "Enter row " << key << " (comma separated)::\n";
		
		getline(cin, row);

		InsertData_runquery(key, row);
		stringstream ss;
		ss << key;
		fputs((ss.str() + "\n").c_str(), file);

		key++;

		cout << "Do you want to enter more?(y/n)";
		cin >> choice;

	} while (choice == 'y' || choice == 'Y');
	fclose(file);
}

//Function to create files for all columns of given table
void CreateFiles(string database_name)
{
	table_name_path = database_name;
	database_name += "/Column_decr.txt";

	ifstream fin;
	string filename(database_name);
	fin.open(database_name.c_str());

	string line;
	int i = 0;
	string table_decr = "";

	while (getline(fin, line))
	{
		stringstream ss(line);
		string s;
		const char delim = ':';
		int i = 0;
		string a, b;
		while (std::getline(ss, s, delim))
		{
			if (i == 0)
			{
				a = s;
				i++;
			}
			else
				b = s;
		}
		table_decr += a + "(" + b + "),";
		column_list_type.push_back(make_pair(a, b));
	}

	int num_col = column_list_type.size();

	for (i = 0; i < num_col; i++)
	{
		fstream file;
		string file_name = table_name_path + "/" + column_list_type[i].first + ".txt";
		file.open(file_name.c_str(), fstream::in | fstream::out | fstream::app);

		if (!file)
		{
			file.open(file_name.c_str(), fstream::in | fstream::out | fstream::trunc);
			file.close();
		}
	}

	fstream file;
	string key_file_name = table_name_path + "/" + "key.txt";
	file.open(key_file_name.c_str(), fstream::in | fstream::out | fstream::app);
	
	if (!file)
	{
		file.open(key_file_name.c_str(), fstream::in | fstream::out | fstream::trunc);
		file.close();
	}
	cin.ignore();
	cout << "Enter Data::\n"
		 << table_decr << "\n";
	Enter_Data_Row();
}
