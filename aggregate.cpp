#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

class aggregate
{
private:
	string colname;	//variable to store name of column to apply aggregate query on
	char coltype;	//variable to store type of column to apply aggregate query on

public:
	string aggregate_table_path;


	aggregate(string column, string path)
	{
		colname = column;
		aggregate_table_path = path;
		Menu();
	}

	void setcoltype(char type)
	{
		coltype = type;
	}

	//Function to fetch type of column given by user
	char FetchType()
	{
		int i;
		bool before = 1;
		string col_name, value, tp;

		fstream ctype;
		string file_name = aggregate_table_path + "/" + "Column_decr.txt";
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

	//Function to calculate count of rows for given column
	int FetchCount()
	{
		string tp;
		int count = 0;

		fstream file;
		string file_name = aggregate_table_path + "/" + colname + ".txt";
		file.open(file_name.c_str(), ios::in);

		if (file.is_open())
		{
			while (getline(file, tp))
				count++;
			file.close();
		}
		return count;
	}

	//Function to fetch values of entire column of type int and return those values in a vactor
	vector<int> FetchInt()
	{
		int i;
		string tp, value;

		vector<int> values;

		fstream file;
		string file_name = aggregate_table_path + "/" + colname + ".txt";
		file.open(file_name.c_str(), ios::in);

		if (file.is_open())
		{
			while (getline(file, tp))
			{
				value = "";
				for (i = 0; i < tp.length(); i++)
				{
					if (tp[i] == ':')
						value = "";
					else
						value = value + tp[i];
				}

				int num = atoi(value.c_str());
				values.push_back(num);
			}
			file.close();
		}
		return values;
	}

	//Function to fetch values of entire column of type double and return those values in a vactor
	vector<double> FetchDouble()
	{
		int i;
		string tp, value;

		vector<double> values;

		fstream file;
		string file_name = aggregate_table_path + "/" + colname + ".txt";
		file.open(file_name.c_str(), ios::in);

		if (file.is_open())
		{
			while (getline(file, tp))
			{
				value = "";
				for (i = 0; i < tp.length(); i++)
				{
					if (tp[i] == ':')
						value = "";
					else
						value = value + tp[i];
				}

				double num = atof(value.c_str());

				values.push_back(num);
			}
			file.close();
		}
		return values;
	}

	//Function to return count
	int Count()
	{
		return FetchCount();
	}

	//Function to return Sum of all values for given column
	long double Sum()
	{
		long double sum = 0;
		switch (coltype)
		{
			case 'i':
			{
				vector<int> v;
				v = FetchInt();
				return accumulate(v.begin(), v.end(), 0);
			}
			break;

			case 'd':
			{
				vector<double> v;
				v = FetchDouble();

				return accumulate(v.begin(), v.end(), 0.0);
			}
			break;

			default:	cout << "Invalid Column Type";
		}
	}

	//Function to return average of all values for given column
	double Average()
	{
		switch (coltype)
		{
			case 'i':
			{
				vector<int> v;
				v = FetchInt();
				return (double)accumulate(v.begin(), v.end(), 0) / v.size();
			}
			break;

			case 'd':
			{
				vector<double> v;
				v = FetchDouble();

				return (double)(accumulate(v.begin(), v.end(), 0.0) / v.size());
			}
			break;

			default:	cout << "Invalid Column Type";
		}
	}

	//Function to return minimum among all values for given column
	double Minimum()
	{
		switch (coltype)
		{
			case 'i':
			{
				vector<int> v = FetchInt();
				return *min_element(v.begin(), v.end());
			}
			break;

			case 'd':
			{
				vector<double> v = FetchDouble();
				return *min_element(v.begin(), v.end());
			}
			break;

			default:	cout << "Invalid Column Type";
		}
	}

	//Function to return maximum among all values for given column
	double Maximum()
	{

		switch (coltype)
		{
			case 'i':
			{
				vector<int> v = FetchInt();
				return *max_element(v.begin(), v.end());
			}
			break;

			case 'd':
			{
				vector<double> v = FetchDouble();
				return *max_element(v.begin(), v.end());
			}
			break;
			default:	cout << "Invalid Column Type";
		}
	}

	//Function to print aggregate menu
	void Menu()
	{
		char type = FetchType();
		setcoltype(type);
		int choice;

		cout << "Select aggregate Function:\n";
		cout << "1.Count\n2.Minimum\n3.Maximum\n4.Sum\n5.Average\n";
		cin >> choice;
		switch (choice)
		{
			case 1:
			{
				cout << "Count:" << Count() << "\n";
			}
			break;
			case 2:
			{
				if (type == 'i')
					cout << "Minimum:" << (int)Minimum() << "\n";
				else if (type == 'd')
					cout << "Minimum:" << Minimum() << "\n";
			}
			break;
			case 3:
			{
				if (type == 'i')
					cout << "Maximum:" << (int)Maximum() << "\n";
				else if (type == 'd')
					cout << fixed << setprecision(5) << "Maximum:" << Maximum() << "\n";
			}
			break;
			case 4:
			{
				if (type == 'i')
					cout << "Sum:" << Sum() << "\n";
				else if (type == 'd')
					cout << fixed << setprecision(5) << "Sum:" << Sum() << "\n";
			}
			break;
			case 5:
			{
				cout << "Average:" << (double)Average() << "\n";
			}
			break;
		}
	}
};
