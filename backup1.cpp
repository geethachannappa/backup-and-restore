include <windows.h>
#include <bits/stdc++.h>
using namespace std;

class Backup {
	private:
		string path;
		int number_of_day;
		const string BACKUP_STORE = "C:\\BACKUP";
		const string RESTORE_PATH = "C:\\RESTORE";
		string str[9] = {"#", "1", "12", "13", "134", "15", "156", "17", "178"};
		vector <string> last_full_backup;
		vector <string> last_backup;
		vector <string> Backup_names[10];
	public:
		Backup();
		vector<string> get_filenames(string folder);
		bool Ispathexist(string path);
		bool create_path(string path);
		bool cpy_files(string src, string dest);
		void full_backup(string src, string dest);
		bool search_vector(vector <string> vec, string str);
		void incremental_backup(string src, string dest);
		void differential_backup(string src, string dest);
		void backup(string path);
		void Restore(int Number_of_day);
		int get_number_of_day();
		void help();
};

Backup :: Backup()
{
	number_of_day = 1;
	create_path(RESTORE_PATH);
}

vector<string> Backup :: get_filenames(string folder)
{
    vector<string> names;
    string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
    return names;
}

bool Backup :: Ispathexist(string path)
{
	const DWORD attributes = GetFileAttributes(path.c_str());
    if (INVALID_FILE_ATTRIBUTES != attributes)
    {
    	return true;
    }
    return false;
}

bool Backup :: create_path(string path)
{
	string cmd = "mkdir ";
	system((cmd + path).c_str());
	if(!Ispathexist(path))
	{
		return false;
	}
	return true;
}



bool Backup :: cpy_files(string src, string dest)
{
	string cmd = "xcopy ";
	system((cmd + src + " " + dest).c_str());
}


void Backup :: full_backup(string src, string dest)
{
	cpy_files(src, dest);
	last_full_backup.empty();
	last_backup.empty();
	Backup_names[number_of_day].empty();
	last_full_backup = get_filenames(src);
	last_backup = get_filenames(src);
	Backup_names[number_of_day] = get_filenames(src);
}

bool Backup :: search_vector(vector <string> vec, string str)
{
	for(int i = 0; i < vec.size(); i++)
	{
		if(str == vec[i]) return true;
	}
	return false;
}

void Backup :: incremental_backup(string src, string dest)
{
	Backup_names[number_of_day].empty();
	vector <string> to_check;
	to_check = get_filenames(src);
	for(int i = 0; i < to_check.size(); i++)
	{
		if(!search_vector(last_backup, to_check[i]))
		{
			cpy_files((src + "\\" + to_check[i]), dest);
			Backup_names[number_of_day].push_back(to_check[i]);
		}
	}
}

void Backup :: differential_backup(string src, string dest)
{
	Backup_names[number_of_day].empty();
	last_backup.empty();
	vector <string> to_check;
	to_check = get_filenames(src);
	for(int i = 0; i < to_check.size(); i++)
	{
		if(!search_vector(last_full_backup, to_check[i]))
		{
			cpy_files((src + "\\" + to_check[i]), dest);
			Backup_names[number_of_day].push_back(to_check[i]);
			last_backup.push_back(to_check[i]);
		}
	}
}

void Backup :: backup(string path)
{
	if(Ispathexist(path))
	{
		if(!Ispathexist(BACKUP_STORE))
		{
			if(!create_path(BACKUP_STORE))
			{
				cout << "Something went wrong! Try running command prompt as Administrator\n";
				return;
			}
		}
		string new_file = "";
		new_file += char('0' + number_of_day);
		string new_path = BACKUP_STORE + "\\" + new_file;
		if(!create_path(new_path))
		{
			cout << "Something went wrong! Try running command prompt as Administrator\n";
			return;
		}
		switch(number_of_day)
		{
			case 1 : full_backup(path, new_path);
					 break;
			case 2 :
			case 4 :
			case 6 :
			case 8 : incremental_backup(path, new_path);
					 break;
			case 3 :
			case 5 :
			case 7 : differential_backup(path, new_path);
					 break;
		}
		number_of_day++;
		if(number_of_day > 8) number_of_day -= 8;
	}
}


void Backup :: Restore(int Number_of_day)
{
	string s = str[Number_of_day];
	string dest = "";
	dest += RESTORE_PATH;
	dest += "\\";
	dest += char('0' + Number_of_day);
	if(!Ispathexist(dest))
	{
		if(!create_path(dest))
		{
			cout << "Something went wrong" << endl;
			return ;
		}
	}
	for(int i = 0;  i <  s.size(); i++)
	{
		char c = s[i];
		string src = "";
		src += BACKUP_STORE + "\\";
		src += c;
		cpy_files(src, dest);
	}
}

int Backup :: get_number_of_day()
{
	return number_of_day;
}

void Backup :: help()
{
	cout << "Use" << endl;
	cout << "backup for Backing up" << endl;
	cout << "restore for restoring" << endl;
	cout << "exit - to exit" << endl;
}


int main()
{
	Backup ob;
	int backed = 0;
	string cmd;
	cout << "Type help to get list of commands" << endl;
	while(true)
	{
		cin >> cmd;
		if(cmd == "backup")
		{
			if(backed == 8)
			{
				cout << "All 8 Backups have been used. Restart the program to get 8  chances again." << endl;
				continue;
			}
			string str;
			cout << "Enter the address of folder that contains file to be backed(Format : C:\\\\Folder\\\\File)" << endl;
			cin >> str;
			backed = ob.get_number_of_day();
			ob.backup(str);
		} else
		if(cmd == "restore")
		{
			int num;
			cout << "Enter the day whosw backup to be restored" << endl;
			cin >> num;
			if(num >= ob.get_number_of_day())
			{
				cout << "It is yet not backed up" << endl;
				continue;
			}
			ob.Restore(num);
		} else
		if(cmd == "exit")
		{
			cout << "Process done" << endl;
			break;
		}else
		if(cmd == "help")
		{
			ob.help();
		}
		else
		{
			cout << "Invalid Command" << endl;
		}
	}

	return 0;
}
