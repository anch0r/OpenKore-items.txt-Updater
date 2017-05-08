/****** OpenKore items.txt updater ******/
/*  This tool is an updater for items.txt                 */
/*  you need the decompiled itemInfo.lua file as source   */
/*  and this tool will generate a new up-to-date items.txt*/
/*  see readme for more information                       */
/*              author:anch0r                             */

#include "main.h"

int readitemInfoLua()
{
	fstream fs;
	try
	{
		fs.open("itemInfo.lua", ios::in);
		if (!fs) throw 0;
	}
	catch (int IOException)
	{
		cerr << "itemInfo.lua does not exist.";
		return IOException;
	}

	regex itemNumberFilter("(^\\[)([0-9]+)(\\])");  //regex filter for item number
	regex itemNameFilter("\"([^\"]*)\"");			//regex filter for item name
	smatch itemNumberStr;
	smatch itemNameStr;
	string tempStr = "";
	string regexStr = "";
	string resultStr = "";
	bool changeFlag = false;
	vector<string> resultSet;

	/*start fetching item number & name from itemInfo.lua*/
	while (getline(fs, regexStr, '='))
	{
		if (regex_search(regexStr, itemNumberStr, itemNumberFilter))
		{
			resultStr = itemNumberStr[2].str(); //itemNumberStr[2]: the pure digit part of smatch
			tempStr = itemNumberStr[2].str();
			changeFlag = true;
		}

		if (regex_search(regexStr, itemNameStr, itemNameFilter))
		{
			if (!changeFlag)continue;  //avoid redundant matching
			else
			{
				/*move down 3 times to get the string of identified item*/
				for (int i = 0; i != 3; ++i) getline(fs, regexStr, '=');
				regex_search(regexStr, itemNameStr, itemNameFilter);
			}
			resultStr.append("#");
			resultStr.append(itemNameStr[1].str()); //itemNameStr[1]: the pure item name part of smatch
			resultStr.append("#");
			resultStr.append("\n");
			resultSet.push_back(resultStr);
			cout << "processing..." << resultStr;
			changeFlag = false;
		}

	}
	fs.close();
	try
	{
		fs.open("items.txt", ios::out);
		if (!fs) throw 0;
	}
	catch (int IOException)
	{
		cerr << "cannot create items.txt file";
		return IOException;
	}
	for (vector<string>::iterator i = resultSet.begin(); i != resultSet.end(); ++i)
	{
		cout << "writing entry:" << *i;
		fs << *i;
	}
	fs.close();
	return 1;
}

int main(int argc, char* argv[])
{
	readitemInfoLua();
	return 0;
}