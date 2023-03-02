#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <algorithm>
#include <ctime>
#include <limits>

using namespace std;

void printQuote();
void printHelp();
void listTasks(bool show);
void setTask(bool daily, string task);
void removeTask(int taskId);
void finishTask(int taskId);
void unfinishTask(int taskId);
string toLower(string data);
string getDate();
string configDay(int day);
string configDate(int day);
string configMonth(int month);

int main()
{
	system("clear");
	printQuote();
	
	string command;
	string command2;
	int j = 0;
	int scale = 0;
	while (1)
	{
		cout << "> ";
		
		cin >> command;

		//cin.ignore(numeric_limits<streamsize>::max(),'\n');
		//cout << toLower(command) << endl;
		string lower = toLower(command);
		if (lower == "exit" || lower == "quit" || lower == ":q") break;
		else if (lower.find("help") != string::npos || lower.find("panic") != string::npos)
		{
			if  (lower.find("panic") != string::npos) cout << "Don't Panic!!" << endl;
			printHelp();
		}
		else if (lower == "list")
		{
			listTasks(1);
		}
		else if (lower.find("set") != string::npos)
		{
			cin >> command2;
			setTask(lower.find("daily") != string::npos, command2);
		}
		else if (lower == "remove")
		{
			cin >> command2;
			try
			{
				removeTask(stoi(command2));
			}
			catch (...)
			{ /* why hello there! */ }
		}
		else if (lower == "finished")
		{
			cin >> command2;
			try
			{
				finishTask(stoi(command2));
			}
			catch (...)
			{ /* general kenobi! */ }
		}
		else if (lower == "finished-not")
		{
			cin >> command2;
			try
			{
				unfinishTask(stoi(command2));
			}
			catch (...)
			{ /* general kenobi! */ }
		}
		else
		{
			if (scale == 0)
			{
				cout << "On a scale of 1 - 10, how sure are you that this is a function?\n";
				scale = 1;
			}
			else
			{
				cout << "Close!\n";
				scale = 0;
			}
		}
	}

	return 0;
}

void removeTask(int taskId)
{
	taskId--;
	listTasks(0);

	ifstream tasks_file("tasks.json", ifstream::binary);
	Json::Value tasks;
	tasks_file >> tasks;

	Json::FastWriter fw;
	string output = fw.write(tasks);

	if (output != "[]\n")
	{
		Json::Value val;
		for (int i = 0; i < tasks.size(); i++)
		{
			if (i != taskId) val[val.size()] = tasks[i];
		}
		ofstream f("tasks.json");
		f << val;
		f.close();
	}
}

void finishTask(int taskId)
{
	taskId--;
	listTasks(0);

	ifstream tasks_file("tasks.json", ifstream::binary);
	Json::Value tasks;
	tasks_file >> tasks;

	Json::FastWriter fw;
	string output = fw.write(tasks);

	if (output != "[]\n")
	{
		Json::Value val;
		for (int i = 0; i < tasks.size(); i++)
		{
			val[val.size()] = tasks[i];
			if (i == taskId)
			{
				string taskName = fw.write(tasks[i]["task"]);
				taskName = taskName.substr(1, taskName.length() - 3);
				if (taskName.find("[completed]") == string::npos)
					taskName += " [completed]";
				val[val.size() - 1]["task"] = taskName;
			}
		}
		ofstream f("tasks.json");
		f << val;
		f.close();
	}
}

void unfinishTask(int taskId)
{
	taskId--;
	listTasks(0);

	ifstream tasks_file("tasks.json", ifstream::binary);
	Json::Value tasks;
	tasks_file >> tasks;

	Json::FastWriter fw;
	string output = fw.write(tasks);

	if (output != "[]\n")
	{
		Json::Value val;
		for (int i = 0; i < tasks.size(); i++)
		{
			val[val.size()] = tasks[i];
			if (i == taskId)
			{
				string taskName = fw.write(tasks[i]["task"]);
				taskName = taskName.substr(1, taskName.length() - 3);
				cout << "task name: " << taskName << endl;
				if (taskName.find("[completed]") != string::npos)
					taskName = taskName.substr(0, taskName.length() - 12);
				val[val.size() - 1]["task"] = taskName;
			}
		}
		ofstream f("tasks.json");
		f << val;
		f.close();
	}
}

void setTask(bool daily, string task)
{
	listTasks(0);
	
	Json::Value val;
	ifstream tasks_file("tasks.json", ifstream::binary);
	Json::Value tasks;
	tasks_file >> tasks;

	Json::FastWriter fw;
	string output = fw.write(tasks);

	if (output == "[]\n")
	{
		val["task"] = task;
		val["daily"] = daily;
		Json::Value vec(Json::arrayValue);
		vec.append(val);
		ofstream f("tasks.json");
		f << vec;
		f.close();
	}
	else
	{
		val["task"] = task;
		val["daily"] = daily;
		tasks[tasks.size()] = val;
		ofstream f("tasks.json");
		f << tasks;
		f.close();
	}
}

void listTasks(bool show)
{
	ifstream tasks_file("tasks.json", ifstream::binary);
	Json::Value tasks;
	if (!tasks_file)
	{
		ofstream f("tasks.json");
		f << "[]";
		f.close();
		listTasks(show);
	}
	else
	{
		tasks_file >> tasks;
		if (!tasks.size() && show) cout << "Hmmm... You don't seem to have any tasks!\n\n";
		else if (show)
		{
			for (int i = 0; i < tasks.size(); i++)
			{
				Json::FastWriter fw;
				string taskName = fw.write(tasks[i]["task"]);
				string isDaily = fw.write(tasks[i]["daily"]);
				cout << "[" << i + 1 << "] " << taskName.substr(1, taskName.length() - 3) << (isDaily == "true\n" ? " [daily]" : "") <<  endl;
			}
		}
	}
}

void printHelp()
{
	cout << "-- Commands --" << endl;
	cout << "1) list                       [list all your tasks]" << endl;
	cout << "2) set <task name>            [sets task]" << endl;
	cout << "3) set-daily <task name>      [sets a daily task]" << endl;
	cout << "4) remove <task number>       [removes a task from your list]" << endl;
	cout << "5) finished <task number>     [checks a task off of your list]" << endl;
	cout << "6) finished-not <task number> [unchecks a task off of your list]" << endl;
	cout << "7) exit                       [ ~ E X I T S ~ ]" << endl;
	cout << "--------------" << endl;
}

string configDay(int day)
{
	string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	return days[day];
}

string configDate(int day)
{
	switch(day % 10)
	{
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}

string configMonth(int month)
{
	string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	return months[month - 1];
}

string getDate()
{
	std::time_t t = std::time(0);   // get time now
  std::tm* now = std::localtime(&t);
	cout << "It is a " << configDay(now->tm_wday) << " on the " << now->tm_mday << configDate(now->tm_mday) << " of " << configMonth(now->tm_mon + 1) << " " << (now->tm_year + 1900) << endl;
	return "";
}

string toLower(string data)
{
	transform(data.begin(), data.end(), data.begin(),
			[](unsigned char c){ return tolower(c); });
	return data;
}

void printQuote()
{
	getDate();

	srand(time(0));

	// Reading quotes from file
	ifstream quotes_file("quotes.json", ifstream::binary);
	Json::Value quotes;
	quotes_file >> quotes;

	// 1643
	int size = quotes.size();

	// https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
	// Spawns a random number between 0 and size, not including size
	Json::Value quote = quotes[rand() % size];

	cout << string(50, '*') << endl;

	Json::FastWriter fw;
	string output = fw.write(quote["text"]);
	//output = "\"The two most powerful warriors are patience and time.\"\n"; // Leo Tolstoy
	string currWord = "";
	string currSentence = "";
	// i = 1, output.length() - 2 to eliminate quotation marks
	for (int i = 1; i < output.length() - 2; i++)
	{
		if (output[i] == ' ' || i == output.length() - 3)
		{
			if (i == output.length() - 3) currWord += output[i];
			if (currWord.length() + currSentence.length() + 1 > 48)
			{
				// Center aligning
				cout << string((50 - currSentence.length()) / 2, ' ') << currSentence << endl;
				currSentence = "";

				if (i == output.length() - 3)
				{
					currSentence += currWord;
					cout << string((50 - currSentence.length()) / 2, ' ') << currSentence << endl;
				}
			}
			else
			{
				if (i == output.length() - 3)
				{
					currSentence += currWord;
					cout << string((50 - currSentence.length()) / 2, ' ') << currSentence << endl;
				}
			}
			currSentence += currWord += ' ';
			currWord = "";
		}
		else
		{
			currWord += output[i];
		}
	}
	string _author = fw.write(quote["author"]);
	// Remove first and last 2 (\n and ") characters from _author
	_author = _author.substr(1, _author.length() - 3);
	string author = "- " + _author;
	cout << string((50 - author.length()), ' ') << author << endl;
	// cout << string(quote[""])[4] << endl;
	cout << string(50, '*') << endl;
	cout << endl;
	listTasks(1);
}
