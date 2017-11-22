// string::find_last_of
#include <algorithm>
#include <iostream>       // std::cout
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include <regex>

#include <stdio.h>
#include <stdlib.h>

void lc_file_name2(const std::string& path)
{
	std::cout << "Splitting: " << path << '\n';
	std::size_t f = path.find_last_of("/\\");
	std::cout << " path: " << path.substr(0, f) << '\n';
	std::cout << " file: " << path.substr(f+1) << '\n';
}

std::string lc_file_dir(const std::string& path, bool has_slash=true)
{
	std::string ret("");
	auto f = path.find_last_of("/\\");
	if(std::string::npos == f)
		return ret;
	if(f)
		ret = path.substr(0, f);
	else
		ret.push_back(path[0]);

	if(has_slash)
	{
		if(ret.length() && ('/' != ret[ret.length()-1] && '\\' != ret[ret.length()-1]))
			ret +='/';
	}
	return ret;
}

std::string lc_file_name(const std::string& path, bool has_extension=true)
{
	std::string ret;
	auto f = path.find_last_of("/\\");
	ret = (std::string::npos == f)? path: path.substr(f+1);
	if(has_extension)
		return ret;
	f = path.find_last_of(".");
	if(std::string::npos == f)
		return ret;
	f? ret.erase(f-1): ret.clear();
	return ret;
}


std::string lc_file_ext(const std::string& path, bool has_dot=true)
{
	auto last_pos = path.find_last_of('.');
	if(last_pos == std::string::npos)
		 return std::string("");
	if(has_dot)
		return path.substr(last_pos);
	return path.substr(last_pos +1);
}


std::vector<int> lc_str_find_string(const std::string& str_src, const std::string& str_find, bool lowcase=true)
{
	std::vector<int> ret;
    if(!str_src.length() || !str_find.length())
        return ret;
	const std::regex rgx_eval(str_find, std::regex_constants::ECMAScript | (lowcase? std::regex_constants::icase: (std::regex_constants::syntax_option_type)0));
	if(!std::regex_search(str_src, rgx_eval))
		return ret;
	auto rgx_bgn = std::sregex_iterator(str_src.begin(), str_src.end(), rgx_eval);
	auto rgx_end = std::sregex_iterator();
	int  rgx_cnt = (int)std::distance(rgx_bgn, rgx_end);

	for(std::sregex_iterator it = rgx_bgn; it != rgx_end; ++it)
	{
		std::smatch matched = *it;
		std::string match_str = matched.str();
		size_t pos = matched.position();
		//std::cout << match_str << " " << pos << '\n';
		ret.push_back(pos);
	}
	return ret;
}

std::vector<int> lc_str_find_string(const char* src_str, const char* find_str, bool lowcase=true)
{
	return lc_str_find_string(std::string(src_str), std::string(find_str), lowcase);
}


void lc_str_shrink_to_null_character(std::string& in_out)
{
	in_out.resize([&]()->size_t
	{
		auto r =  in_out.find_last_not_of('\0');
		return r == std::string::npos ? 0: r;
	}());
}








int main()
{
	const char *fmt = "sqrt(2) = %f";
	size_t sz = _snprintf(NULL, 0, fmt, std::sqrt(2));

	std::string buf;
	buf.resize(sz + 1); // note +1 for null terminator
	_snprintf(&buf[0], buf.size(), fmt, std::sqrt(2));

	std::string str = "/user/desktop/abc/post/";

	int pos = ([](const std::string& str_src, const std::string& str_match)->int
	{
		auto pos = str_src.find(str_match);
		if(pos == std::string::npos)
			return -1;
		return (int)pos;
	}(str, std::string("/abc/")));
	


	std::vector<int> ret(0);

	//std::string str1("/usr/bin/man");
	//std::string str2("\\tst.exe");

	//lc_file_name2(str1);
	//lc_file_name2(str2);

	//std::string str_dir  = lc_file_dir(str1);
	//std::string str_name = lc_file_name(str2);
	//std::string str_ext  = lc_file_ext(str2);

	std::string src = "zasffas.PnGd.pNgadfadfa .pngasdfa s";
	const std::string val = ".png";
	ret = lc_str_find_string(src, val, false);

	src.resize(300);

	int size = src.length();
	lc_str_shrink_to_null_character(src);
	size = src.length();

	_stricmp("adf", "cc");
	return 0;
}
