/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** main
*/

#include "gtest/gtest.h"

#include <iostream>

const std::string &StringToUpper(std::string &str);
std::vector<std::string> splitString(const std::string &str, const char &delimiter, int max_token = -1) noexcept;
std::string getBinaryDir(bool endSlash = true) noexcept;

TEST(SplitTest, Spaces)
{
	std::vector<std::string> test_1({"oui", "non", "kek"});
	EXPECT_EQ(test_1, splitString("oui non kek", ' '));

	std::vector<std::string> test_2({"oui", "non kek"});
	EXPECT_EQ(test_2, splitString("oui non kek", ' ', 1));
}

TEST(SplitTest, Char)
{
	std::vector<std::string> test_1({"oui", "non", "kek"});
	EXPECT_EQ(test_1, splitString("oui_non_kek", '_'));

	std::vector<std::string> test_2({"oui", "non_kek"});
	EXPECT_EQ(test_2, splitString("oui_non_kek", '_', 1));
}

TEST(SplitTest, Wrong)
{
	std::vector<std::string> test_1({"oui", "non", "non"});
	EXPECT_NE(test_1, splitString("oui non kek", ' '));
}

TEST(GetBinaryDirTest, Good)
{
	// std::string bin_dir = getBinaryDir();
	// EXPECT_EQ(bin_dir, "/home/kylian/repo/CPP_zia_2019/build/bin/");

	// std::string bin_dir_no_endshalsh = getBinaryDir(false);
	// EXPECT_EQ(bin_dir_no_endshalsh, "/home/kylian/repo/CPP_zia_2019/build/bin");
}

TEST(CppVersion, Wich)
{
	if (__cplusplus == 201703L)
		std::cout << "C++17\n";
	else if (__cplusplus == 201402L)
		std::cout << "C++14\n";
	else if (__cplusplus == 201103L)
		std::cout << "C++11\n";
	else if (__cplusplus == 199711L)
		std::cout << "C++98\n";
	else
		std::cout << "pre-standard C++\n";
}

TEST(ToUpper, str)
{
	std::string test_1 = "oui";
	std::string test_2 = "oUi";
	std::string test_3 = "Oui Non";
	EXPECT_EQ("OUI", StringToUpper(test_1));
	EXPECT_EQ("OUI", StringToUpper(test_2));
	EXPECT_EQ("OUI NON", StringToUpper(test_3));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}