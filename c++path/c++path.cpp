
#ifndef MY_DEF
#pragma message("-------------------------------------------------")
#endif


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <codecvt>

#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <time.h>

using namespace std;
namespace fs = std::tr2::sys;


std::string  DisplayPathInfo()
{
	string str_path = "/FileSystemTest/../../SubDir3/SubDirLevel2/test.txt";

	fs::path src_path(str_path);
	ostringstream ioss;

	int i = 0;
	ioss << "Displaying path info for: " << src_path << endl;

	for(auto& itr :src_path)
	{
		ioss << "path part: " << i++ << " = " << itr << endl;
	}

	ioss
		<< "root_name     () = " << src_path.root_name     () << endl
		<< "root_path     () = " << src_path.root_path     () << endl
		<< "relative_path () = " << src_path.relative_path () << endl
		<< "parent_path   () = " << src_path.parent_path   () << endl
		<< "filename      () = " << src_path.filename      () << endl
		<< "stem          () = " << src_path.stem          () << endl
		<< "extension     () = " << src_path.extension     () << endl;
	return ioss.str();
}


string LastWriteTimeToLocalTime(const fs::path& file_path)
{
	const auto last = chrono::system_clock::to_time_t
	(
		chrono::system_clock::from_time_t(
			fs::last_write_time(file_path)
		)
	);

	tm timeinfo;
	localtime_s(&timeinfo, &last);
	char buf[128]{};
	asctime_s(buf, 56, &timeinfo);
	// appends '\n'
	return string{ buf };
}

// List files and directories recursively in the path
void IterateFolderRecursively(const fs::path& p)
{
	cout << "Begin iterating " << p.string() << " recursively" << endl;

	for (fs::recursive_directory_iterator it{p}, end; it != end; ++it)
	{
		if (fs::is_regular_file(it->status()))
		{
			cout.fill(' ');
			cout.width(it.level()*4);
			cout
				<< " "
				<< "File: " << it->path().filename() << " : "
				<< LastWriteTimeToLocalTime(it->path());
		}
		else if (fs::is_directory(it->status()))
		{
			cout.width(it.level()*4);
			cout
				<< " "
				<< "Dir: " << it->path().filename()
				<< endl;
		}
	}
}


static void main_cc(int argc, char* argv[])
{
	//cout << DisplayPathInfo() << endl
	//<< "Press Enter to exit" << endl;


	IterateFolderRecursively("E:/_document/_vc/c++path");

	//string str_buf;
	//getline(cin, str_buf);
}


int main( void )
{
	std::ostringstream s;
	s.precision(8);
	s << 0.1234567890123456789012345678901234567890;
	OutputDebugString(s.str().c_str());

	::OutputDebugString("Hello world");

	::setlocale(LC_ALL, "");

	FILE* fp = fopen("test.txt", "rt");
	if(!fp)
		return 0;


	char    r_txt  [128+4] = {};
	char    pmbc   [128+4] = {};
    wchar_t r_wc   [256+4] = {};

	fgets(r_txt, 128, fp);

	std::wstring_convert<std::codecvt_utf8<wchar_t> > myconv;
    std::wstring w_str = myconv.from_bytes(r_txt);

	std::string s_str = myconv.to_bytes(w_str);


	auto w_len = wcslen(w_str.c_str());

    printf( "Convert a wide character to multibyte character:\n" );
    auto ret = wcstombs( pmbc, w_str.c_str(), 128 );

    printf( "  Multibyte character: %s\n\n", pmbc );

    printf( "Convert multibyte character back to a wide "
            "character:\n" );

	fclose(fp);
}

