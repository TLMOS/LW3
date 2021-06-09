#pragma once

void TimeTestTree(int max, int step, int iters);

//Binary tree tests
//Assuming Contains, GetHeight and GetSize work correctly
void TestTreeCreation(std::string prefix = "", bool silent = true);
void TestTreeGetAndThreads(std::string prefix = "", bool silent = true);
void TestTreeInsert(std::string prefix = "", bool silent = true);
void TestTreeRemove(std::string prefix = "", bool silent = true);
void TestTreeEquals(std::string prefix = "", bool silent = true);
void TestTreeStringConversion(std::string prefix = "", bool silent = true);
void TestTreeMap(std::string prefix = "", bool silent = true);
void TestTreeWhere(std::string prefix = "", bool silent = true);
void TestTreeReduce(std::string prefix = "", bool silent = true);
void TestTreeAll(std::string prefix = "", bool silent = true);

//Test all
void TestAll(std::string prefix = "", bool silent = true);

//Tools
void RepeatedTest(void (*f)(std::string, bool), int n, std::string prefix = "", bool silent = true);
void WaitKey();