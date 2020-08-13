#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

class Query
{
private:
	int choice;
	vector<string> column_list;	//stores list of all columns of selected table
	vector<string> cols_to_display;	//stores list of selected columns to be displayed by query

	bool sq_error = 0;	//indicates error in select query with single condition
	bool dq_error = 0;	//indicates error in double query with double conditions

public:
	string query_table_path;

	//Function to get key and value part separate for key:value pair
	void getid(string s, int &before, string &key, string &value)
	{
		for (int i = 0; i < s.length(); i++)
		{
			if (s[i] == ':')
				before = 0;
			else
			{
				if (before == 1)
					key = key + s[i];

				else
					value = value + s[i];
			}
		}
	}

	//Function to fetch type of column colname
	char FetchType(string colname)
	{
		int i;
		bool before = 1;
		string col_name, value, tp;

		fstream ctype;
		string file_name = query_table_path + "/Column_decr.txt";
		ctype.open(file_name.c_str(), ios::in);

		if (ctype.is_open())
		{
			while (getline(ctype, tp))
			{
				before = 1;
				col_name = "";
				value = "";
				for (i = 0; i < tp.length(); i++)
				{
					if (tp[i] == ':')
					{
						if (col_name == colname)
						{
							return tp[i + 1];
							break;
						}
						else
						{
							col_name = "";
							before = 0;
						}
					}
					else
					{
						if (before == 1)
							col_name = col_name + tp[i];
					}
				}
			}
		}
		else
			cout << "Error Opening File";
	}

	//Function to return vector of keys based on single where clause
	vector<string> Select_single()
	{
		vector<string> keys;
		string where_col, where_val;
		int op, i, j, k;

		cin.ignore();
	enter:
		cout << "Enter column name:-";
		getline(cin, where_col);

		string check_col;
		check_col = query_table_path + "/" + where_col + ".txt";
		struct stat buf;

		if (stat(check_col.c_str(), &buf) != 0)
		{
			cout << "Invalid column name\n";
			goto enter;
		}

		char type = FetchType(where_col);

		cout << "Enter operator( 1.=  2.<  3.<=  4.>  5.>= ):-";
		cin >> op;

		if (op >= 2 && op <= 5)
		{
			if (type != 'i' && type != 'd')
			{
				cout << "Illegal Operation";
				sq_error = 1;
				return keys;
			}
		}

		cout << "Enter value:-";
		cin.ignore();
		getline(cin, where_val);

		string tp, key, value;
		int before;

		fstream file;
		string file_name = query_table_path + "/" + where_col + ".txt";
		file.open(file_name.c_str(), ios::in);

		switch (op)
		{
		case 1:
		{

			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (value == where_val)
					{

						keys.push_back(key);
					}
				}
			}
		}
		break;

		case 2:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) < atoi(where_val.c_str()))
							keys.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) < atof(where_val.c_str()))
							keys.push_back(key);
					}
				}
			}
		}
		break;
		case 3:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) <= atoi(where_val.c_str()))
							keys.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) <= atof(where_val.c_str()))
							keys.push_back(key);
					}
				}
			}
		}
		break;
		case 4:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) > atoi(where_val.c_str()))
							keys.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) > atof(where_val.c_str()))
							keys.push_back(key);
					}
				}
			}
		}
		break;
		case 5:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) >= atoi(where_val.c_str()))
							keys.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) >= atof(where_val.c_str()))
							keys.push_back(key);
					}
				}
			}
		}
		break;
		default:
			cout << "Invalid Choice\n";
		}
		file.close();

		return keys;
	}

	//Function to return vector of keys based on double where clause based on chosen connector
	vector<string> Select_double()
	{
		string where_col1, where_val1, where_col2, where_val2;
		int op1, op2, i, j, k;
		int connector;

		vector<string> keys1;	//stores keys obtained from first where condition
		vector<string> keys2;	//stores keys obtained from second where condition
		vector<string> keys;	//stores intersection or union of keys1 and keys2 based on connector

		cout << "\nFirst Conditon:\n";
		cin.ignore();
	enter:
		cout << "Enter column name:-";
		getline(cin, where_col1);

		string check_col;
		check_col = query_table_path + "/" + where_col1 + ".txt";
		struct stat buf;

		if (stat(check_col.c_str(), &buf) != 0)
		{
			cout << "Invalid column name\n";
			goto enter;
		}

		char type = FetchType(where_col1);

		cout << "Enter operator( 1.=  2.<  3.<=  4.>  5.>= ):-";
		cin >> op1;

		cout << "Enter value:-";
		cin.ignore();
		getline(cin, where_val1);

		string tp, key, value;
		int before;

		fstream file;
		string file_name = query_table_path + "/" + where_col1 + ".txt";
		file.open(file_name.c_str(), ios::in);

		switch (op1)
		{
		case 1:
		{

			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (value == where_val1)
						keys1.push_back(key);
				}
			}
		}
		break;

		case 2:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) < atoi(where_val1.c_str()))
							keys1.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) < atof(where_val1.c_str()))
							keys1.push_back(key);
					}
				}
			}
		}
		break;
		case 3:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) <= atoi(where_val1.c_str()))
							keys1.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) <= atof(where_val1.c_str()))
							keys1.push_back(key);
					}
				}
			}
		}
		break;
		case 4:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) > atoi(where_val1.c_str()))
							keys1.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) > atof(where_val1.c_str()))
							keys1.push_back(key);
					}
				}
			}
		}
		break;
		case 5:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) >= atoi(where_val1.c_str()))
							keys1.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) >= atof(where_val1.c_str()))
							keys1.push_back(key);
					}
				}
			}
		}
		break;
		default:
			cout << "Invalid Choice\n";
		}
		file.close();

	Con:
		cout << "Enter logical connector(1.OR(||)  2.AND(&&)):-";
		cin >> connector;

		cout << "\nSecond Conditon:\n";
		cin.ignore();
	enter2:
		cout << "Enter column name:-";
		getline(cin, where_col2);

		check_col = query_table_path + "/" + where_col2 + ".txt";

		if (stat(check_col.c_str(), &buf) != 0)
		{
			cout << "Invalid column name\n";
			goto enter2;
		}

		type = FetchType(where_col2);

		cout << "Enter operator( 1.=  2.<  3.<=  4.>  5.>= ):-";
		cin >> op2;

		cout << "Enter value:-";
		cin.ignore();
		getline(cin, where_val2);

		file_name = query_table_path + "/" + where_col2 + ".txt";
		file.open(file_name.c_str(), ios::in);

		switch (op2)
		{
		case 1:
		{

			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (value == where_val2)
						keys2.push_back(key);
				}
			}
		}
		break;

		case 2:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) < atoi(where_val2.c_str()))
							keys2.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) < atof(where_val2.c_str()))
							keys2.push_back(key);
					}
				}
			}
		}
		break;
		case 3:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) <= atoi(where_val2.c_str()))
							keys2.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) <= atof(where_val2.c_str()))
							keys2.push_back(key);
					}
				}
			}
		}
		break;
		case 4:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) > atoi(where_val2.c_str()))
							keys2.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) > atof(where_val2.c_str()))
							keys2.push_back(key);
					}
				}
			}
		}
		break;
		case 5:
		{
			if (file.is_open())
			{
				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (type == 'i')
					{
						if (atoi(value.c_str()) >= atoi(where_val2.c_str()))
							keys2.push_back(key);
					}
					else if (type == 'd')
					{
						if (atof(value.c_str()) >= atof(where_val2.c_str()))
							keys2.push_back(key);
					}
				}
			}
		}
		break;
		default:
			cout << "Invalid Choice\n";
		}
		file.close();

		switch (connector)
		{
		case 1:
		{
			sort(keys1.begin(), keys1.end());
			sort(keys2.begin(), keys2.end());
			set_union(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys));
		}
		break;
		case 2:
		{
			sort(keys1.begin(), keys1.end());
			sort(keys2.begin(), keys2.end());
			set_intersection(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys));
		}
		break;
		default:
			cout << "Invalid Connector Selected";
			goto Con;
		}

		sort(keys.begin(), keys.end());

		return keys;
	}

	//Function return the list of all columns of given table
	vector<string> ColumnNames(string table_name)
	{
		ifstream fin;
		string filename(table_name + "/" + "Column_decr.txt");

		fin.open(filename.c_str());
		string line;
		vector<string> column_name;
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
					column_name.push_back(a);
				}
			}
		}
		return column_name;
	}

	//Function to provide select query inputs
	void Select(string path, string table_name)
	{
		int input, i, j, k, col_no, choice;
		query_table_path = path;
		column_list = ColumnNames(query_table_path);

		cout << "SELECT\n";

		cout << "Select column numbers to display(-1 to end):-\n";
		cout << "1:All\t";
		for (i = 0; i < column_list.size(); i++)
			cout << i + 2 << ":" << column_list[i] << "\t";

		cout << "\n";

		cin >> col_no;

		while (col_no != -1)
		{
			if (col_no == 1)
			{
				cols_to_display = column_list;
				break;
			}
			cols_to_display.push_back(column_list[col_no - 2]);
			cin >> col_no;
		}

		cout << "FROM " << table_name << "\n";

		cout << "WHERE\n";

		cout << "1. Select query with single condition\n";
		cout << "2. Select query with two conditions\n";

		cin >> choice;

		vector<string> keys;

		switch (choice)
		{
		case 1:
		{
			keys = Select_single();
			if (sq_error == 1)
				return;
		}
		break;
		case 2:
		{
			keys = Select_double();
			if (dq_error == 1)
				return;
		}
		break;
		default:
			cout << "Invalid Choice\n";
		}

		vector<vector<string> > rows;
		vector<string> row;

		vector<int> max_length(cols_to_display.size());

		for (i = 0; i < cols_to_display.size(); i++)
			max_length[i] = cols_to_display[i].length();

		string tp, key, value;
		int before;

		for (i = 0; i < keys.size(); i++)
		{
			row.push_back(keys[i] + ":");
			for (j = 0; j < cols_to_display.size(); j++)
			{
				fstream file;
				string file_name = query_table_path + "/" + cols_to_display[j] + ".txt";
				file.open(file_name.c_str(), ios::in);

				while (getline(file, tp))
				{
					before = 1;
					key = "";
					value = "";

					getid(tp, before, key, value);

					if (key == keys[i])
					{
						row.push_back(value);

						if (value.length() > max_length[j])
							max_length[j] = value.length();

						break;
					}
				}
				file.close();
			}
			rows.push_back(row);
			row.clear();
		}

		cout << "Number of rows:-";
		cout << rows.size() << "\n";

		if (rows.size())
		{
			cout << setw(6) << "Row ";

			for (i = 0; i < cols_to_display.size(); i++)
				cout << setw(max_length[i]) << cols_to_display[i] << " ";

			cout << "\n";

			for (i = 0; i < rows.size(); i++)
			{
				for (j = 0; j < rows[i].size(); j++)
				{
					if (j == 0)
						cout << setw(5) << rows[i][j] << " ";
					else
						cout << setw(max_length[j - 1]) << rows[i][j] << " ";
				}
				cout << "\n";
			}
		}
	}
};
