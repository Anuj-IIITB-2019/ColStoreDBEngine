#include <bits/stdc++.h>

using namespace std;

vector<string> column_list;	//stores list of all columns of imported table
string data_path;

//Function to create files for all columns of given table
void CreateFiles(string table_name, string db_path)
{
	data_path = db_path;

	ifstream fin;
	string filename(table_name);
	fin.open(table_name.c_str());
	string line;
	int i = 0;

	getline(fin, line);

	stringstream ss(line);
	string s;
	const char delim = ',';
	while (std::getline(ss, s, delim))
	{
		column_list.push_back(s);
	}
	FILE *fp_col;
	string temp = (db_path + "/Column_decr.txt");
	fp_col = fopen(temp.c_str(), "a+");
	int num_col = column_list.size();
	cout << "ENTER TABLE SCHEMA: Column Type((i for int, v for varchar,c for char,d for double)::\n";
	for (int i = 0; i < num_col; i++)
	{
		string col_type, col_name = column_list[i];
		cout << "Column" << i + 1 << ":: " << column_list[i] << " :: ";
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
	}
	fclose(fp_col);

	for (i = 0; i < num_col; i++)
	{
		fstream file;
		string file_name = data_path + "/" + column_list[i] + ".txt";
		file.open(file_name.c_str(), fstream::in | fstream::out | fstream::binary | fstream::app);

		if (!file)
		{
			file.open(file_name.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);
			file.close();
		}
	}
	fstream file;
	string key_file_name = data_path + "/" + "key.txt";
	file.open(key_file_name.c_str(), fstream::in | fstream::out | fstream::binary | fstream::app);
	if (!file)
	{
		file.open(key_file_name.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);
		file.close();
	}
}

//Function to fetch last key stored in key.txt file
int GenerateKey()
{
	fstream file;
	static int lastkey = 1;
	string filename;

	filename = data_path + "/" + "key.txt";

	file.open(filename.c_str());

	while (file >> lastkey)
		;

	file.close();

	return lastkey;
}

//Function to insert given row data into various column files
void InsertData(int primary_key, string row)
{
	int len = row.length(), j = 0;
	string value = "";
	for (int i = 0; i < len; i++)
	{
		if (row[i] == ',')
		{
			if (value == "")
				value = "NULL";
			stringstream ss;
			ss << primary_key;

			string data = ss.str() + ":" + value;

			FILE *file;
			string file_name = (data_path + "/" + column_list[j] + ".txt");
			file = fopen(file_name.c_str(), "a+");
			data = data + "\n";
			fputs((data).c_str(), file);
			fclose(file);

			j++;

			value = "";
		}
		else
		{
			value = value + row[i];
		}
	}
	if (value == "")
		value = "NULL";
	stringstream ss;
	ss << primary_key;

	string data = ss.str() + ":" + value;

	FILE *file;
	string file_name = (data_path + "/" + column_list[j] + ".txt");
	file = fopen(file_name.c_str(), "a+");
	data = data + "\n";
	fputs((data).c_str(), file);
	fclose(file);
}

//Function to read rows one by one from imported table
void EnterRow(string database_name)
{
	int key, i;
	string row;

	key = GenerateKey();

	FILE *file;
	string file_name = (data_path + "/key.txt");
	file = fopen(file_name.c_str(), "a+");

	ifstream fin;
	string filename(database_name);
	fin.open(database_name.c_str());
	string line;

	getline(fin, line);

	while (!fin.eof())
	{
		getline(fin, line);

		InsertData(key, line);

		stringstream ss;
		ss << key;

		fputs((ss.str() + "\n").c_str(), file);

		key++;
	}
	fclose(file);

	fin.close();
	cout << "Writing Complete\n";
	column_list.clear();
}
