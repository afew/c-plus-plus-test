#if defined(_MSC_VER) && _MSC_VER >= 1400
  #pragma warning(disable: 4996)
#endif


#include <vector>
#include <set>
using namespace std;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <Windows.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <direct.h>


std::vector<string >	lst_files;
std::vector<string >	lst_folders;


void gatherAllFiles(const char* sDir)
{
	char			sSearchPath[MAX_PATH*2]={0};
	_finddata_t		fd = {0,};
	long			handle =0;
	int				result=1;

	sprintf(sSearchPath,"%s/*.*", sDir);
	handle= _findfirst(sSearchPath, &fd);

	if (handle == -1)
		return;


	while (result != -1)
	{
		// Folder
		if( (fd.attrib & 0x10) && '.' != fd.name[0])
		{
			char	sub_path[MAX_PATH*2]={0};
			sprintf(sub_path, "%s/%s", sDir, fd.name);

			lst_folders.push_back(sub_path);
			gatherAllFiles(sub_path);
		}


		// File
		if( (fd.attrib & 0x20) && '.' != fd.name[0])
		{
			char	full_path[MAX_PATH*2]={0};
			sprintf(full_path, "%s/%s", sDir, fd.name);

			lst_files.push_back(full_path);
			//printf("%3d File name: %s\n", cnt , full_path);
		}

		result= _findnext(handle, &fd);
	}

	_findclose(handle);
}





int main(int argc, char** argv)
{
	if( 3 > argc)
		gatherAllFiles("d:/eppengine_2_0/epp_aos_packing/pm/assets");
	else
		gatherAllFiles(argv[1]);



	for(auto it = lst_folders.begin(); it != lst_folders.end(); ++it)
	{
		printf("Folder list: %s\n", it->c_str());
	}

	for(auto it = lst_files.begin(); it != lst_files.end(); ++it)
	{
		printf("File list: %s\n", it->c_str());
	}

	lst_folders.clear();
	lst_folders.shrink_to_fit();


	lst_files.clear();
	lst_files.shrink_to_fit();

	return 0;
}

