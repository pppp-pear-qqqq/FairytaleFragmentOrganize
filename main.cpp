#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

//namespace std
//{
//	using u8ifstream = basic_ifstream<char8_t, char_traits<char8_t>>;
//	using u8regex = basic_regex<char8_t>;
//};

struct Fragment
{
	std::string name;
	std::string cost;
	std::string target;
	std::string effect;
	std::string num;
	std::string rank;
};

int process(std::string input, std::string output, bool eraseComment)
{
	std::ifstream ifs(input);
	if(ifs.fail())
		throw std::exception("�t�@�C�����J���܂���ł���");
	std::ofstream ofs(output);
	if(ofs.fail())
		throw std::exception("�t�@�C�����쐬�ł��܂���ł���");

	std::string line;

	std::cout << (const char*)u8"���O\t\t�R�X�g\t�Ώ�\t����\t\t\t��\t�����N" << std::endl;

#ifdef _DEBUG
	int count = 0;
#endif // _DEBUG

	while(std::getline(ifs, line))
	{
		// �^�C�v�͂킩���@����2����
		// ������[]�Ŋ����Ă�@���ĂȂ����Ƃ�����
		// ���O�́A.+�̃t���O�����g�̌`
		// �R�X�g�ł���΁A�y�z�Ŋ���ꂽ�������ŏ��ɂ���
		// ���ʂ�[]�Ŋ���ꂽ�����@����������͐��������̕\�L

#ifdef _DEBUG
		++count;
#endif // _DEBUG

		Fragment fragment;

		// �ŏ��ɑS�p�𔼊p�ɒu�������ق�������
		line = std::regex_replace(line, std::regex((const char*)u8R"((�@))"), " ");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�O))"), "0");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�P))"), "1");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�Q))"), "2");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�R))"), "3");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�S))"), "4");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�T))"), "5");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�U))"), "6");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�V))"), "7");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�W))"), "8");
		line = std::regex_replace(line, std::regex((const char*)u8R"((�X))"), "9");
		line = std::regex_replace(line, std::regex(R"(a)"), "A");
		line = std::regex_replace(line, std::regex(R"(b)"), "B");
		line = std::regex_replace(line, std::regex(R"(c)"), "C");
		line = std::regex_replace(line, std::regex(R"(s)"), "S");
		// ���łɐF�X�؂蕪����
		line = std::regex_replace(line, std::regex((const char*)u8"(�y)"), " ");
		line = std::regex_replace(line, std::regex((const char*)u8"(�z)"), " ");
		line = std::regex_replace(line, std::regex(R"(\])"), "] ");
		line = std::regex_replace(line, std::regex(R"(\)(\d))"), ") $1");
		line = std::regex_replace(line, std::regex((const char*)u8R"(���΂�(\d))"), (const char*)u8"���΂� $1");
		line = std::regex_replace(line, std::regex(R"((\d)[ABCS])"), "$1 $2");
		line = std::regex_replace(line, std::regex(R"( +)"), " ");

		boost::algorithm::trim(line);

		std::vector<std::string> buf;
		boost::split(buf, line, boost::is_any_of(" \t"));
		for(auto& s : buf)
		{
			if(std::regex_match(s, std::regex((const char*)u8R"(.+�̃t���O�����g)")))
			{
				fragment.name = s;
			}
			else if(std::regex_match(s, std::regex(R"((\+*[SMH]P\d+)+)")))
			{
				fragment.cost = s;
			}
			else if(std::regex_match(s, std::regex(R"(\[.+\])")))
			{
				fragment.target = s.substr(0, s.size() - 1).substr(1);
			}
			else if(std::regex_match(s, std::regex((const char*)u8R"(\d+(��)*)")))
			{
				fragment.num = std::regex_replace(s, std::regex((const char*)u8"��"), "");
			}
			else if(std::regex_match(s, std::regex(R"(\w+)")))
			{
				fragment.rank = s;
			}
			else
			{
				fragment.effect = s;
			}
		}
		if(fragment.rank.empty() && !fragment.cost.empty())
		{
			if(fragment.cost == "SP60" || fragment.cost == "MP60" || fragment.cost == "HP10" || fragment.cost == "SP30+MP30")
				fragment.rank = "C";
			else if(fragment.cost == "SP100" || fragment.cost == "MP100" || fragment.cost == "HP20" || fragment.cost == "SP60+MP60")
				fragment.rank = "B";
		}
		std::cout << fragment.name << "\t" << fragment.cost << "\t" << fragment.target << "\t" << fragment.effect << "\t" << fragment.num << "\t" << fragment.rank << std::endl;
		ofs << fragment.name << "," << fragment.cost << "," << fragment.target << "," << fragment.effect << "," << fragment.num << "," << fragment.rank << std::endl;
	}
	ifs.close();
	ofs.close();
}

#include <Windows.h>

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1024);
	try
	{
#ifndef _DEBUG
		if(argc == 1)
			throw std::exception("�t�@�C�����w�肳��Ă��܂���");
		std::string input = argv[1];
#else
		std::string input = "Fragment.txt";
#endif // _DEBUG

		std::string output = "output.csv";
		bool eraseComment = false;

		for(int i = 2; i < argc; ++i)
		{
			std::string buf = argv[i];
			size_t t = buf.find_first_of('=');
			if(t != std::string::npos)
			{
				auto id = buf.substr(0, t);
				if(id == "output")
				{
					output = buf.substr(t + 1);
				}
				else if(id == "erasecomment")
				{
					if(buf.substr(t) == "true")
					{
						eraseComment = true;
					}
				}
			}
			else if(buf == "erasecomment")
			{
				eraseComment = true;
			}
		}
		
		process(input, output, eraseComment);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}