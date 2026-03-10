// A C++ program which can be used to compute
// data supporting the conjecture in this paper.
// Each option generates a user-specified output file
// or options to send the output to the concole.
//
// Input Options and output formats
//   Start = srarting value of e
//   End   = ending value of e
//   Input filename = index from list
//	   or fully quafied file name 
//     or ~filename on desktop
//     or g to generate base vector
//   Output f = output file
//     or c for console output
//   Code: 's' = solutions, 'd' = detail, 't' = totals only 
//		or 'i' to display the iterations during a search for solutions
//			followed by the number of leading zeros >= 0
//		or 'c' for convolution 
//			followed by the convolution fold > 0
//	    or 'f' to display the first solution found during iteration
// 
// Option 1: Output file
//	 Each output line consists of the pattern 
//	  {e,n, {p, e-p}}:
// 	 	e is the even integer
//	 	n is the totla number of solutions
//      p is the first prime solution
// Option 2: Total number of solutions
//   Output summary:
//		# solutions found
//
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstring>
#include <iterator>
#include <vector>
#include <random>
#include <numeric>
#include <memory>
#include <ctime>
#include <windows.h>
#include <Lmcons.h>
#include <sys/types.h>
#include "dirent.h"
#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

#include <wchar.h>
#include <locale.h>

#include <locale> 

using namespace std;

ofstream fout;
ifstream fin;
vector<string> files;

class Plotter
{
public:
	Gnuplot gp;

	Plotter() : gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"")
	{}

	Plotter& set_title(std::string_view title, std::string_view font)
	{
		if (font == "")
			gp << "set title '" << title << "'\n";
		else
			gp << "set title '" << title << "'" << " font '," << font << "'\n";
		return *this;
	}

	Plotter& set_xlabel(std::string_view label, std::string_view font)
	{
		if (font == "")
			gp << "set xlabel '" << label << "'\n";
		else
			gp << "set xlabel '" << label << "'" << " font '," << font << "'\n";
		return *this;
	}

	Plotter& set_ylabel(std::string_view label, std::string_view font)
	{
		if (font == "")
			gp << "set ylabel '" << label << "'\n";
		else
			gp << "set ylabel '" << label << "'" << " font '," << font << "'\n";
		return *this;
	}

	void set_grid(bool g)
	{
		if (g)
			gp << "set grid \n";
		else
			gp << "unset grid \n";
	}

	void plot_points(std::vector < boost::tuple<double, double>>& v)
	{
		gp << "plot '-' with points pt 7 ps .5 lc rgb 'black' \n";
		gp.send(v);
	}

	void plot_lines(std::vector < boost::tuple<double, double>>& v)
	{
		gp << "plot '-' with lines lc rgb 'black' \n";
		gp.send(v);
	}

	void plot_vector(std::vector<double>& v)
	{
		gp << "plot '-' with lines lc rgb 'black' \n";
		gp.send(v);
	}

};

double cmax(double a, double b)
{
	if (a < b) return b;
	return a;
}

double cmin(double a, double b)
{
	if (a < b) return a;
	return b;
}

vector<double> convolve(vector<double> v, vector<double> ker)
{
	int i, j;
	vector<double> convolution;

	convolution.clear();

	int leftsize = v.size();
	int rightsize = ker.size();

	for (i = 1; i < leftsize + rightsize; i++)
	{
		double sum = 0;
		for (j = cmax(0, i - rightsize); j < cmin(i, leftsize); j++)
		{
			sum += v[j] * ker[i - j - 1];
		}
		convolution.push_back(sum);
	}
	return convolution;
}

bool is_prime(long n)
{
	long i;
	bool is_prime = true;

	// loop to check if n is prime
	for (i = 2; i <= n/2; ++i)
	{
		if (n % i == 0)
		{
			is_prime = false;
			break;
		}
	}
	return is_prime;
}

string username()
{
	char buffer[UNLEN + 1]{};
	DWORD len = UNLEN + 1;
	if (GetUserNameA(buffer, &len)) return buffer;
	else return "";
}

std::string getFileContent(const std::string& path)
{
	std::ifstream file(path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

int listFiles(const char* dir)
{
	int end, index = 0;
	string code;
	DIR* dr;
	struct dirent* en;

	dr = opendir(dir);
	if (dr)
	{
		std::cout << endl;
		while ((en = readdir(dr)) != NULL)
		{
			code = en->d_name;
			size_t found = code.find("10-");
			if (found != string::npos)
			{
				index++;
				files.push_back(code);
				std::cout << "\t" << index << ": " << code << endl;
			}
		}
		closedir(dr);
		std::cout << endl;
		std::cout << "Enter index: ";
		std::cin >> end;
		if (end >= 1 && end <= index)
		{
			return end;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

string GetCurrentDirectory()
{
	string s;
	char buffer[MAX_PATH];
	const char* str;
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	s = std::string(buffer).substr(0, pos);
	return s + "\\";
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

void help1()
{
	cout << " Input Options and output formats" << endl;
	cout << "   Start = srarting value of e" << endl;
	cout << "   End   = ending value of e" << endl;
	cout << "   Input filename = index from list" << endl;
	cout << "	or fully quafied file name " << endl;
	cout << "	or ~filename on desktop" << endl;
	cout << "	or g to generate base vector" << endl;
	cout << "   Output f = output file" << endl;
	cout << "	or c for console output" << endl;
	cout << "   Code: 's' = solutions, 'd' = detail, 't' = totals only " << endl;
	cout << "         'v' = verify solutions, 'l' = limits only" << endl;
	cout << "	or 'i' to display the iterations during a search for solutions" << endl;
	cout << "		followed by the number of leading zeros >= 0" << endl;
	cout << "	or 'c' for convolution " << endl;
	cout << "		followed by the convolution fold > 0" << endl;
	cout << "	or 'f' to display the first solution found during iteration" << endl;
}

void help2()
{
	cout << " Option 1: Output file" << endl;
	cout << "	Each output line consists of the pattern " << endl;
	cout << "	  {e,n, {p, e-p}}:" << endl;
	cout << " 	 	e is the even intege" << endl;
	cout << "	 	n is the totla number of solutions" << endl;
	cout << "	p is the first prime solution" << endl;
	cout << " Option 2: Formatted solutions depending on code" << endl;
	cout << "	a, c, d, e, f, g, i, l, m, p, s, t, u, v " << endl;
}

int main()
{
	Plotter plt;

	string data = GetCurrentDirectory().c_str();
	string base = "C:\\Users\\" + username() + "\\Desktop\\";
	string filename, first, dir,cmd;
	string code, r1 = "";
	string diff = "";
	int start, end, e, k, count, index, pos,fold, ndx,match,matched;
	bool detail = false;
	bool geometry = false;
	bool modular = false;
	bool diffs = false;
	bool total = false;
	bool solutions = false;
	bool verify = false;
	bool convolution = false;
	bool iterate = false;
	bool lims = false;
	bool firstonly = false;
	bool fileopen = false;
	bool plot = false;
	bool solution_vector = false;
	bool induction_step = false;
	bool gaussian_primes = false;
	bool exception = false;
	double ratio,min,max;
	std::vector<double> v1;
	std::vector<double> v;
	std::vector<double> w;
	std::vector<double> ker;
	std::vector<double> conv;
	char buffer[MAX_PATH];
	ifstream infile; 
	ofstream outfile;

	v1.resize(10 ^ 10);
	v.resize(10 ^ 10);
	w.resize(10 ^ 10);
	ker.resize(10 ^ 10);
	conv.resize(10 ^ 10);

	/*
		setlocale(LC_CTYPE, "en_US.utf8");
		wchar_t two = 0x00B2;
		wchar_t sub_k = 0x2096;
		wprintf(L"C%lc\n", sub_k);
	*/


	while (true)
	{
		code = "";
		count = 0;
		diffs = false;
		detail = false;
		geometry = false;
		modular = false;
		solutions = false;
		verify = false;
		convolution = false;
		iterate = false;
		total = false;
		firstonly = false;
		fileopen = false;
		plot = false;
		lims = false;
		solution_vector = false;
		induction_step = false;
		gaussian_primes = false;
		exception = false;
		fold = 1;
		match = -1;
		matched = 0;

		v1.clear();
		v.clear();
		w.clear();
		ker.clear();
		conv.clear();

		std::cout << "\nEnter ? or exit or starting value of e: ";
		std::cin >> cmd;
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		if (cmd == "exit") return 0;

		if (cmd == "help" || cmd == "?")
		{
			system("CLS");
			help1();
			int ch = _getch();
			system("CLS");
			help2();
			continue;
		}

		start = stoi(cmd);
		std::cout << "\nEnter ending value of e: ";
		std::cin >> end;
		if (start > end)
		{
			std::cout << "\nStart > End" << endl;
			std::cin.get();
			return 0;
		}

		k = (end - 6) / 2 + 1;

		if (end > pow(10.0, 9))
		{
			std::cout << "\nEnd > 10^9" << endl;
			std::cin.get();
			return 0;
		}

		if (start > end)
		{
			std::cout << "\nStart > End" << endl;
			std::cin.get();
			return 0;
		}

		if (end < pow(10.0, 6))
		{
			filename = data + "10-6.txt";
		}

		if (1000000 <= end && end < 10000000)
		{
			filename = data + "10-7.txt";
		}

		if (10000000 <= end && end < 100000000)
		{
			filename = data + "10-8.txt";
		}

		if (100000000 <= end)
		{
			filename = data + "10-9.txt";
		}

		cout << filename << endl;

		infile.open(filename);
		r1 = getFileContent(filename);
		r1.erase(remove(r1.begin(), r1.end(), ' '), r1.end());
		r1.erase(remove(r1.begin(), r1.end(), '\n'), r1.end());
		std::cout << r1.size() << " bytes read from " << filename << endl;
		
		k = (end - 6) / 2 + 1;
		for (int i = 1; i <= 2 * k; i += 2)
			v1.push_back(r1[i] - 48);

		if (2 * k > r1.size())
		{
			std::cout << 2 * k << " > " << r1.size() << endl;
			exit(1);
		}

		infile.close();

		/*
		{
			index = listFiles(GetCurrentDirectory().c_str());	

			if (index == 0)
			{
				std::cout << "\nEnter an index to select a file from the list" << endl;
				std::cout << "  [ or a fully qualified INPUT file name or g to generate" << endl;
				std::cout << "  [e.g. c:\\\\Users\\\\" + username() + "\\\\Desktop\\\\filename.txt]" << endl;
				std::cout << "  [or ~filename uses filename on the Desktop]" << endl << endl << ": ";
				std::cin >> code;
				std::cout << endl;
			}
			else
				code = data + files[index - 1];

			if (toupper(code[0]) != 'G')
			{
				size_t found = code.find("~");
				if (found != string::npos)
					filename = base + code.substr(found + 1);
				else
					filename = code;

				std::cout << filename << endl;

				infile.open(filename);

				if (infile)
				{
					r1 = getFileContent(filename);
					r1.erase(remove(r1.begin(), r1.end(), ' '), r1.end());
					r1.erase(remove(r1.begin(), r1.end(), '\n'), r1.end());

					//		if(detail)
					std::cout << r1.size() << " bytes read from " << filename << endl;

					if (2 * k > r1.size())
					{
						std::cout << 2 * k << " > " << r1.size() << endl;
						exit(1);
					}

					for (int i = 1; i <= 2 * k; i += 2)
						v1.push_back(r1[i] - 48);

					infile.close();
				}
				else
				{
					std::cout << "<" << filename << "> not found." << endl;
					return(0);
				}
			}
			else
			{
				for (int i = 1; i <= k; i++)
				{
					if (is_prime(2 * i + 1))
						v1.push_back(1);
					else
						v1.push_back(0);
					if (i % 1000)
						std::cout << "                   " << '\r' << 2 * i;
				}
				std::cout << endl;
			}
		}
		*/

		std::cout << "\nEnter an 'f' for output file or 'c' for console" << endl;
		std::cin >> code;
		std::cout << endl;

		if (toupper(code[0]) == 'F')
		{
			filename = base + "G-" + std::to_string(start) + "-" + std::to_string(end) + ".dat";
			outfile.open(filename);
			fileopen = true;
		}

		std::cout << "Enter one or more of the following:" << endl;
		std::cout << "\t'a' for absolute detail" << endl;
		std::cout << "\t'c' for convolution" << endl;
		std::cout << "\t'd' for differences only" << endl;
		std::cout << "\t'e' verify induction step" << endl;
		std::cout << "\t'f' for first solution" << endl;
		std::cout << "\t'g' Goldbach's comet" << endl;
		std::cout << "\t'i' to iterate" << endl;
		std::cout << "\t'l' for max/min limits only" << endl;
		std::cout << "\t'm' modular form" << endl;
		std::cout << "\t'p' Gaussian primes" << endl;
		std::cout << "\t's' for all solutions" << endl;
		std::cout << "\t't' for total solutions only" << endl;
		std::cout << "\t'u' solution vector only" << endl;
		std::cout << "\t'v' verify solution" << endl;
		std::cout << "\t'x' exceptions only" << endl;
		std::cout << "\t'y' Geometry" << endl;
		std::cin >> code;

		std::transform(code.begin(), code.end(), code.begin(), ::toupper);

		if (code.find('P') != std::string::npos)
		{
			std::cout << "  Gaussian primes" << endl << endl;
			gaussian_primes = true;
		}

		if (code.find('T') != std::string::npos)
		{
			std::cout << "  Total number of solutions" << endl << endl;
			total = true;
		}

		if (code.find('Y') != std::string::npos)
		{
			std::cout << "  Geometry" << endl << endl;
			geometry = true;
		}

		if (code.find('G') != std::string::npos)
		{
			std::cout << "  Goldbach's comet" << endl << endl;
			plot = true;
		}

		if (code.find('E') != std::string::npos)
		{
			std::cout << "  Induction step: e -> e+2" << endl << endl;
			induction_step = true;
		}	

		if (code.find('X') != std::string::npos)
		{
			std::cout << "  Induction step exceptions only" << endl << endl;
			induction_step = true;
			exception = true;
		}

		if (code.find('D') != std::string::npos)
		{
			std::cout << "  Differences only: d = q-p" << endl << endl;
			diffs = true;
		}

		if (code.find('I') != std::string::npos)
		{
			std::cout << " Iterate to first solution" << endl << endl;
			iterate = true;
			std::cout << "\nEnter # leading zeros: ";
			std::cin >> match;
			if (match < 0)
			{
				std::cout << "\nError ***** # leading zeros must be >= 0." << endl;
				std::cin.get();
				continue;
			}
			else
			{
				std::cout << "C(k) = ";
				for (int i = 0; i < match; i++)
				{
					cout << "0";
				}
				std::cout << "1..." << endl << endl;
			}
		}

		if (code.find('F') != std::string::npos)
		{
			std::cout << "  First solution" << endl << endl;
			firstonly = true;
		}

		if (code.find('L') != std::string::npos)
		{
			std::cout << "  Limits" << endl << endl;
			lims = true;
		}

		if (code.find('U') != std::string::npos)
		{
			std::cout << "  C and H vectors" << endl << endl;
			solution_vector = true;
		}

		if (code.find('A') != std::string::npos)
		{
			std::cout << "  Absolute detail" << endl << endl;
			detail = true;
		}

		if (code.find('M') != std::string::npos)
		{
			std::cout << "  Modular form" << endl << endl;
			modular = true;
		}

		if (code.find('V') != std::string::npos)
		{
			std::cout << "  Verify solution" << endl << endl;
			verify = true;
		}

		if (code.find('S') != std::string::npos)
		{
			std::cout << "  All solutions" << endl << endl;
			solutions = true;
		}

		if (code.find('C') != std::string::npos)
		{
			std::cout << "  Convolution" << endl << endl;
			plot = true;
			convolution = true;
			if (plot)
			{
				std::cout << "\nEnter convolution fold: ";
				std::cin >> fold;
				if (fold < 1)
				{
					std::cout << "\nError ***** Convolution fold must be > 0." << endl;
					std::cin.get();
					continue;
				}
			}
		}

		if (gaussian_primes)
		{
			e = start;
			long p1 = 0;
			long p2 = 0;
			count = 0;

			if (e <= 4) break;

			k = (e - 6) / 2 + 1;
			int l = (e - 4) / 2 + 1;

			std::vector < boost::tuple<double, double>> pts;

			clock_t start = clock(); // Start the clock

			for (int i = 0; i < floor((k + 1) / 2); i++)
			{
				if (i % 10000 == 9999)
					std::cout << " " << i + 1 << "\r";

				if (v1[i] * v1[k - i - 1])
				{
					if (is_prime(2 * i + 3))
					{
						p1 = 2 * i + 3;
						if (p1 == 5)
							continue;
					}
					for (int j = i - 1; j >= 0 && j <= i + 1; j += 2)
					{
						if (v1[j] * v1[l - j - 1])
						{
							if (is_prime(2 * j + 3))
								p2 = 2 * j + 3;
							if ((is_prime(p2 + 2) && p1 == p2 + 2) || (is_prime(p2 - 2) && p1 == p2 - 2))
							{
								if (is_prime(e - p1) && is_prime(e + 2 - p2))
								{
									v.push_back(p1);
									w.push_back(p2);
									pts.push_back(boost::make_tuple(p1, p2));
									count++;
								}
							}
						}
					}
				}
			}
			cout << "count: " << count << endl;			
			plt.set_grid(true);
			plt.plot_points(pts);
			continue;
		}

		if (induction_step)
		{
			int start_count = -1;
			int exception_count = 0;

			for (e = start; e <= end; e += 2)
			{
				long p1 = 0;
				long p2 = 0;
				count = 0;
				string flag = "";

				if (e <= 4) break;

				k = (e - 6) / 2 + 1;
				int l = (e - 4) / 2 + 1;

				clock_t start = clock(); // Start the clock

				for (int i = 0; i < floor((k + 1) / 2); i++)
				{
					if (i % 10000 == 9999)
						std::cout << " " << i + 1 << "\r";

					if (v1[i] * v1[k - i - 1])
					{
						if (is_prime(2 * i + 3))
						{
							p1 = 2 * i + 3;
							if (p1 == 5)
								continue;
						}
						for (int j = i - 1; j >= 0 && j <= i + 1; j += 2)
						{
							if (v1[j] * v1[l - j - 1])
							{
								if (is_prime(2 * j + 3))
									p2 = 2 * j + 3;
								if ((is_prime(p2 + 2) && p1 == p2 + 2) || (is_prime(p2 - 2) && p1 == p2 - 2))
								{
									if (is_prime(e - p1) && is_prime(e + 2 - p2))
									{
										count = count + 1;
										if (!total && count == 1)
										{
											flag = "";
											int diff = (e + 2 - 2 * p2) - (e - 2 * p1);
											if (e % 6 == 2 && diff != 6)
											{
												flag = "*";
												exception_count++;
											}
											if ((e % 6 == 0 || e % 6 == 4) && diff != -2)
											{
												flag = "*";
												exception_count++;
											}
											if (!exception)
											{
												if (fileopen)
												{
													outfile << to_string(e) + ": " + to_string(p1) << " : " << to_string(e - p1) << " (" << e - 2 * p1 << ") ";
													outfile << flag << " ( " << e + 2 - 2 * p2 - (e - 2 * p1) << " ) " << to_string(p2) << " : " << to_string(e + 2 - p2) << endl;
												}
												std::cout << to_string(e) + ": " + to_string(p1) << " : " << to_string(e - p1) << " (" << e - 2 * p1 << ") ";
												std::cout << flag << " ( " << e + 2 - 2 * p2 - (e - 2 * p1) << " ) " << to_string(p2) << " : " << to_string(e + 2 - p2) << endl;
											}
											else
											{
												std:cout << "  " << e << "\r";
												if (flag == "*")
												{
													if (fileopen)
													{
														outfile << to_string(e) + ": " + to_string(p1) << " : " << to_string(e - p1) << " (" << e - 2 * p1 << ") ";
														outfile << flag << " ( " << e + 2 - 2 * p2 - (e - 2 * p1) << " ) " << to_string(p2) << " : " << to_string(e + 2 - p2) << endl;
													}
													std::cout << to_string(e) + ": " + to_string(p1) << " : " << to_string(e - p1) << " (" << e - 2 * p1 << ") ";
													std::cout << flag << " ( " << e + 2 - 2 * p2 - (e - 2 * p1) << " ) " << to_string(p2) << " : " << to_string(e + 2 - p2) << endl;
												}
											}
											i = floor((k + 1) / 2);
											break;
										}
									}
								}
							}
						}

					}
				}
				if (count > 1)
				{
					cout << "Total number of twin prime solutions for " << e << ": " << count << endl;
					if (fileopen)
						outfile << "Total number of twin prime solutions for " << e << ": " << count << endl;
				}
				clock_t end = clock(); // Stop the clock

				double elapsed_mins = double(end - start) /(60 *  CLOCKS_PER_SEC);
				if (elapsed_mins > 1)
					std::cout << "\t Elapsed time: " << elapsed_mins << " minutes" << std::endl;

				if (count == 0)
				{
					if (fileopen)
						outfile << to_string(e) + " has no solution. ";
					std::cout << to_string(e) + " has no solution. ";
				}
			}
			std::cout << endl << "Number of exceptions = " << exception_count << endl;
			continue;
		}

		if (lims)
		{
			min = 1;
			max = 0;
			for (e = start; e <= end; e += 2)
			{
				if (e <= 4) break;
				k = (e - 6) / 2 + 1;

				w.assign(v1.begin(), v1.begin() + k);
				v = w;
				reverse(v.begin(), v.end());
				count = std::inner_product(w.begin(), w.end(), v.begin(), 0);
				ratio = 4 * floor((count + 1) / 2) / e;
				if (ratio < min) min = ratio;
				if (ratio > max) max = ratio;
				pos++;
				if (pos == 501)
				{
					std::cout << e << "\r";
					pos = 1;
				}
			}
			std::cout << "{" << e-2 << "," << min << "," << max << "}" << endl;
			if (fileopen)
				outfile << "{" << e-2 << "," << min << "," << max << "}" << endl;
			if (pos > 0)
				std::cout << "               " << endl;
			continue;
		}

		count = 0;
		if (total || geometry || modular || plot)
		{
			std::vector < boost::tuple<double, double>> pts;
			std::vector < boost::tuple<double, double>> mpts;
			pos = 0;

			for (e = start; e <= end; e += 2)
			{
				int sols = 0;
				if (e <= 4) break;
				k = (e - 6) / 2 + 1;

				w.assign(v1.begin(), v1.begin() + k);
				v = w;
				reverse(v.begin(), v.end());
				count = std::inner_product(w.begin(), w.end(), v.begin(), 0);
				pts.push_back(boost::make_tuple(e, floor((count + 1) / 2)));
				if (fileopen)
				{
					pos++;
					if (pos == 501)
					{
						std::cout << e << "\r";
						pos = 1;
					}
					if (!modular)
					{
						outfile << "{" << e << "," << floor((count + 1) / 2) << "," << count / floor(e / 4) << "}" << endl;
					}
				}
				else if (!plot && !modular)
					std::cout << "Total solutions for e = " << e << " : " << floor((count + 1) / 2) << endl;

				if (modular)
				{
					mpts.push_back(boost::make_tuple(e, e-floor((count + 1) / 2)));
					if (!fileopen)
						std::cout << e << " , " << e - floor((count + 1) / 2) << endl;
					else
						outfile << e << " , " << e - floor((count + 1) / 2) << endl;
				}
				if (geometry)
				{
					for (int i = 0; i < floor((k + 1) / 2); i++)
					{
						if (v1[i] * v1[k - i - 1])
						{
							if (sols == 0)
							{
								sols = 1;
								std::cout << "e = " << e << ": {" << 2 * i + 3;
							}
							else
							{
								std::cout << " ; " << 2 * i + 3;
							}
						}
					}
					std::cout << "}" << endl;
					continue;
				}
			}
			if (pos > 0)
				std::cout << "               " << endl;

			if (plot && !modular)
			{
				plt.set_title("Goldbach`s Comet", "20");
				plt.set_xlabel("e", "14");
				plt.set_ylabel("# solutions", "14");
				plt.set_grid(true);
				plt.plot_points(pts);
				//				std::iota(scaled.begin(), scaled.end(), 0);
				std::cin.get();
				std::cin.get();
				continue;
			}

			if (modular)
			{
				plt.set_title("Modular Forms", "20");
				plt.set_xlabel("e", "14");
				plt.set_ylabel("e - # sols", "14");
				plt.set_grid(true);
				plt.plot_points(mpts);
				std::cin.get();
				std::cin.get();
				continue;
			}
		}

		if (detail || verify || iterate || firstonly || solution_vector)
		{
			count = 0;
			for (e = start; e <= end; e += 2)
			{
				std::vector < boost::tuple<double, double>> w;

				if (!solution_vector && e % 100000 == 99999)
					std::cout << " " << e << '\r';

				if (e <= 4) break;

				k = (e - 6) / 2 + 1;

				if (match < 0 && !solution_vector)
				{
					std::cout << "k = " << k << " / k-min = " << floor((k + 1) / 2) << endl;
				}

				if (!fileopen && e > start && match < 0) cout << endl;

				/*
				for (int i = 0; i < k; i++)
				{
					if (v1[i] != 0)
						cout << 1;
					else
						cout << 0;
				}
				cout << endl;
				for (int i = 0; i < k; i++)
				{
					if (v1[k - i - 1] != 0)
						cout << 1;
					else
						cout << 0;
				}
				cout << endl;
				*/
				if (solution_vector)
				{
					if (fileopen)
					{
						outfile << "{" << e << ", ";
					}
					else
					{
						std::cout << "{" << e << ", ";
					}
					for (int i = 0; i < k; i++)
					{
						if (v1[i] * v1[k - i - 1] != 0)
							if (fileopen)
								outfile << 1;
							else
								std::cout << 1;
						else
							if (fileopen)
								outfile << 0;
							else
								std::cout << 0;
					}
					if (fileopen)
					{
						outfile << "}";
					}
					else
					{
						std::cout << "}";
					}

					std::vector<int> numbers = {};
					if (fileopen)
					{
						outfile << " : {";
					}
					else
					{
						std::cout << " : {";
					}

					int j = 0;
					int offset = 0;
					if (k % 2 == 1)
					{
						offset = 1;
						if (fileopen)
						{
							outfile << v1[k / 2];
						}
						else
						{
							std::cout << v1[k / 2];
							numbers.push_back(v1[k / 2]);
						}
					}	
					for (int i = k / 2 - 1; i-j >= 0 ; i++)
					{
						if (v1[i - j] * v1[i + 1 + offset] == 1)
						{
							numbers.push_back(1);
							if (fileopen)
							{
								outfile << 1;
							}
							else
							{
								std::cout << 1;
							}
						}
						else
						{
							numbers.push_back(0);
							if (fileopen)
							{
								outfile << 0;
							}
							else
							{
								std::cout << 0;
							}
						}
						j = j + 2;
					}
					if (fileopen)
					{
						outfile << "}" << endl;
					}
					else
					{
						std::cout << "}";
					}
					/*
					for (auto it = numbers.begin(); it != numbers.end(); ++it) 
					{
						std::cout << *it << "";
					}
					*/
//					std::cout << std::endl;
					continue;
				}
				for (int i = 0; i < floor((k + 1) / 2); i++)
				{
					if (i == 0 && detail)
					{
						std::cout << "s(->) = ";
						for (int j = 0; j < k; j++)
						{
							std::cout << v1[j];
						}
						std::cout << endl;

						std::cout << "s(<-) = ";
						for (int j = 0; j < k; j++)
						{
							std::cout << v1[k - j - 1];
						}
						std::cout << endl << endl;

					}
					if (v1[i] * v1[k - i - 1])
					{
						if (match >= 0 && i == match)
						{
							matched++;
							std::cout << e << endl;
							break;
						}
						if (verify)
						{
							if (!is_prime(2 * i + 3))
							{
								std::cout << 2 * i + 3 << " is not a prime" << endl;
								return(1);
							}
							if (!is_prime(start - (2 * i + 3)))
							{
								std::cout << start - (2 * i + 3) << " is not a prime" << endl;
								return(1);
							}

						}

						if (detail)
						{
							if (e > start) std::cout << endl;
							for (int j = i; j <= k - i - 1; j++)
							{
								std::cout << v1[j];
							}
							std::cout << endl;

							for (int j = i; j <= k - i - 1; j++)
							{
								std::cout << v1[k - j - 1];
							}
							std::cout << endl;

							for (int j = i; j <= k - i - 1; j++)
							{
								std::cout << v1[j] * v1[k - j - 1];
							}
							std::cout << endl;
						}

						count++;
						if (fileopen)
							outfile << "{" << e << "," << i + 1 << ",\{" << 2 * i + 3 << "," << e - (2 * i + 3) << "\}\}" << endl;
						else if (match < 0)
						{
							std::cout << "Solution found at level: " << i + 1 << " [" << e << "] - \{" << 2 * i + 3 << "," << e - (2 * i + 3) << "\}" << endl << endl;
						}
						if (firstonly)
						{
							break;
						}

						if (match < 0 && 2 * i + 3 == start - (2 * i + 3))
						{
							std::cout << "s[" << i + 1 << "]: ";
							for (int j = i; j < k - i; j++)
							{
								std::cout << v1[j];
							}
							std::cout << endl << endl;
						}
					}
					else
					{
						if (!fileopen && !firstonly && match < 0)
							std::cout << i + 1 << " <" << v1[i] << "> - failed" << endl;
					}
				}
			}
			std::cout << endl;
			if (!solution_vector)
			{
				if (match < 0)
				{
					std::cout << "Total solutions: " << count << endl << endl;
				}
				else
				{
					std::cout << "Total containig " << 2 * match + 3 << ": " << matched << endl << endl;
				}
			}
		}
		else if (!total && !geometry)
		{
			for (e = start; e <= end; e += 2)
			{
				diff = "";
				count = 0;
				if (end > 1000 && e % 1000 == 999)
					std::cout << "                   " << '\r' << e;

				if (e <= 4) break;

				k = (e - 6) / 2 + 1;

				if (!fileopen)
				{
					if (detail)
					{
						for (int i = 0; i < k; i++)
						{
							std::cout << v1[i];
							if (v1[i] * v1[k - i - 1]) break;
						}
						std::cout << endl;

						for (int i = 0; i < k; i++)
						{
							std::cout << v1[k - i - 1];
							if (v1[i] * v1[k - i - 1]) break;
						}
						std::cout << endl;

						for (int i = 0; i < k; i++)
						{
							std::cout << v1[i] * v1[k - i - 1];
							if (v1[i] * v1[k - i - 1]) break;
						}
						std::cout << endl;
					}

					if (solutions)
						std::cout << endl << "Solutions for e = " << e << endl << endl;
				}

				for (int i = 0; i < floor((k + 1) / 2); i++)
				{
					if (v1[i] * v1[k - i - 1])
					{
						if (verify)
						{
							if (!is_prime(2 * (i + 1) + 1))
							{
								std::cout << 2 * (i + 1) + 1 << " is not a prime" << endl;
								return(1);
							}
							if (!is_prime(e - (2 * (i + 1) + 1)))
							{
								std::cout << e - (2 * (i + 1) + 1) << " is not a prime" << endl;
								return(1);
							}

						}

						count++;
						if (!fileopen && solutions)
						{
							std::cout << "( " << 2 * (i + 1) + 1 << " , " << 2 * (k - i) + 1 << " ) : d = " << (2 * (k - i) - 2 * (i + 1)) << endl;
						}
						if (count == 1)
						{
							first = "{" + std::to_string(2 * (i + 1) + 1) + " , " + std::to_string(2 * (k - i) + 1) + "}";
						}
						if (diff == "")
						{
							diff = to_string(e) + " , " + to_string(2 * (k - i) - 2 * (i + 1));
						}
						else 
						{
							diff = diff + " , " + to_string(2 * (k - i) - 2 * (i + 1));
						}
					}
				}

				if (!fileopen && !modular && !total)
				{
					if (diff != "")
					{
						std::cout << "D-set at e = " << diff << endl;
					}
					else
					{
						std::cout << endl;
						std::cout << "Total for e = " << e << " : " << count << " ; first = " << first << endl << endl;
					}
				}
				if (convolution)
				{
					if (plot) 
					{
						double sum = 0.0;
						for (int i = 0; i < k; i++)
						{
							v.push_back(v1[i]);
							ker.push_back(v1[i]);
							reverse(ker.begin(), ker.end());
						}
						for (int i = 0; i < ker.size(); i++)
						{
							sum += ker[i];
						}
						for (int i = 0; i < ker.size(); i++)
						{
							ker[i] = ker[i] / sum;
						}

						for (int i = 1; i <= fold; i++)
						{
							v = convolve(v, ker);
						}

						plt.set_title("","").set_title("Convolution - Fold<" + to_string(fold) + ">", "20");
						plt.set_xlabel("e", "14");
						plt.set_ylabel("y", "14");
						plt.set_grid(true);
						plt.plot_vector(v);
						std::cin.get();
						std::cin.get();
					}
				}
				if (fileopen)
				{
					outfile << "{" + diff+ "}"  << endl;
				}
			}
		}

		if (fileopen)
		{
			std::cout << "<" << filename << "> created." << endl;
			outfile.close();
		}
	}
}

