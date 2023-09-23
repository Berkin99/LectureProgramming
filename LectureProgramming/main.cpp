#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>

using namespace std;

class Lecture;
class Program;
class ProgramManager;


class Lecture {
	int day;
	float startTime;
	float endTime;
public:
	Lecture(int d,float start, float end) {
		day = d;
		startTime = start;
		endTime = end;
	}

	float * getTimes() {
		float* times = new float[2];
		times[0] = startTime;
		times[1] = endTime;
	}
	float getStartTime() { return startTime; }
	float getEndTime() { return endTime; }
	float getDuration() { return endTime - startTime; }
	int getDay() { return day; }
};

class Program {
	string programName;
	int credit;
	int nofl;
	int intersections;
	vector<Lecture> lectures;
	public:
	Program() {
		programName = "unknown";
		credit = 0;
		nofl = 0;
		intersections = 0;
	}
	Program(string name, int c) {
		programName = name;
		credit = c;
		nofl = 0;
		intersections = 0;
	}
	void addLecture(Lecture l) { lectures.push_back(l); nofl++; }
	vector<Lecture> getLectures() {return lectures;}
	string getName() { return programName;}
	int getCredit() { return credit;}
	int getNofl() { return nofl; }
	int getIntersections() { return intersections; }
	void setIntersections(int p) { intersections = p; }
	void incIntersections() { intersections++; }
};

class ProgramManager {
	vector<Program> programs;
	vector<Program> maxPrograms;
	vector<Program> maxCreditPrograms;

	int nofp;
	public:
	ProgramManager() {
		nofp = 0;
	}
	void textRead(string textaddr);
	bool isIntersect(Program a, Program b);
	int calculateIntersections();
	void calculateMaxPrograms();
	void calculateMaxCredit();

	void addProgram(Program p) {
		for (int i=0; i < nofp; i++) {
			if (programs[i].getName() == p.getName()) {
				cout << p.getName() << " is already in the list." << endl;
				return;
			}
		}
		programs.push_back(p);
		nofp++;
	}

	vector<Program> getPrograms() { 
		return programs; 
	}
	vector<Program> getMaxPrograms() {
		return maxPrograms;
	}
	vector<Program> getMaxCreditPrograms() {
		return maxCreditPrograms;
	}
	void printPrograms(vector<Program> prog , bool printLectures=false) {
		size_t len = prog.size();
		int totalCredit = 0;
		for (int i = 0; i < len; i++)
		{
			printProgram(prog[i],printLectures);
			totalCredit += prog[i].getCredit();
		}

		cout << "Total Credit : " << totalCredit << endl;
		cout << "Total Programs : " << len << endl;

	}

	void printProgram(Program pr , bool lectures = false) {
		int nfl = pr.getNofl();
		cout << pr.getName() << "\n" << pr.getCredit() << endl;
		cout << "Program Lectures: " << nfl << endl;
		cout << "" << endl;
		if (lectures) {
			for (int j = 0; j < nfl; j++) {
				printLecture(pr.getLectures()[j]);
			}
		}
		cout << "" << endl;

	}

	void printLecture(Lecture l) {
		cout << "Day : " << l.getDay() << endl;
		cout << "StartTime: " << l.getStartTime() << endl;
		cout << "EndTime: " << l.getEndTime() << endl;
		cout << "" << endl;
	}
	
};

void  ProgramManager::textRead(string textaddr) {
	fstream file;
	file.open(textaddr, ios::in);

	int iter;
	file >> iter;
	
	for (int i = 0; i < iter; i++) {
		string program_name;
		int program_credit;
		int nl;
		while(program_name==""){ getline(file, program_name); }
		file >> program_credit;
		file >> nl;
		Program temp_pr (program_name, program_credit);
		for (int j = 0; j < nl; j++) {
			string day;
			float st, et;
			while (day == "") { getline(file, day); }
			file >> st;
			file >> et;
			Lecture temp_l (stoi(day), st, et);
			temp_pr.addLecture(temp_l);
		}
		addProgram(temp_pr);
	}

	file.close();

}

bool  ProgramManager::isIntersect(Program p1, Program p2) {

	vector<Lecture> p1l = p1.getLectures();
	vector<Lecture> p2l = p2.getLectures();

	size_t p1len = p1l.size();
	size_t p2len = p2l.size();

	for (int i = 0; i < p1len; i++) {
		for (int j = 0; j < p2len; j++) {

			float lec1s = p1l[i].getStartTime();
			float lec1e = p1l[i].getEndTime();
			int lec1d = p1l[i].getDay();

			float lec2s = p2l[j].getStartTime();
			float lec2e = p2l[j].getEndTime();
			int lec2d = p2l[j].getDay();

			if (lec1d == lec2d) {
				if (lec2s >= lec1s && lec2s < lec1e) return true;
				if (lec1s >= lec2s && lec1s < lec2e) return true;
			}
		}
	}

	return false;
}

int   ProgramManager::calculateIntersections() {
	int intersections = 0;
	size_t len = programs.size();

	for (int i = 0; i < len; i++) {
		programs[i].setIntersections(0);
	}

	for (int i = 0; i < len - 1; i++) {
		for (int j = i+1; j < len; j++) {
			if (isIntersect(programs[i], programs[j])) {
				cout << programs[i].getName() << " and " << programs[j].getName() << " are intersecting!" << endl;
				programs[i].incIntersections();
				programs[j].incIntersections();

				cout << programs[i].getName() << " intersections: " << programs[i].getIntersections() << endl;
				cout << programs[j].getName() << " intersections: " << programs[j].getIntersections() << endl;
				intersections++;
			}
		}
	}
	return intersections;
}

void  ProgramManager::calculateMaxPrograms() {
	//calculate intersections
	vector<Program> temp_p = programs;
	int total_intersections = calculateIntersections();
	cout << "/// Calculating Max Program ///" << endl;
	cout << "Total Intersections : " << total_intersections << endl;


	int maxIntIndex;
	int maxInt;
	size_t len;
	//while intersections!=0
	while (total_intersections != 0) {
		//find max intersect and erase
		maxIntIndex = 0;
		maxInt = 0;
		len = programs.size();
		cout << "Program Size: " << len << endl;
		for (int i = 0; i < len; i++)
		{
			if (programs[i].getIntersections() > maxInt) {
				maxInt = programs[i].getIntersections();
				maxIntIndex = i;
 			}
			else if (programs[i].getIntersections() == maxInt) {
				if (programs[i].getCredit() < programs[maxIntIndex].getCredit()) {
					maxInt = programs[i].getIntersections();
					maxIntIndex = i;
				}
			}
		}
		cout << "Max Int: " << maxInt << ", MaxIntIndex: " << maxIntIndex << endl;
		cout << "Delete " << programs[maxIntIndex].getName() << endl;

		programs.erase(programs.begin() + maxIntIndex);
		total_intersections = calculateIntersections();
		
	}

	maxPrograms = programs;
	programs = temp_p;

}

void  ProgramManager::calculateMaxCredit() {

}

int main(void) {
	ProgramManager pr;
	pr.textRead("lectures.txt");
	pr.calculateMaxPrograms();
	pr.printPrograms(pr.getMaxPrograms());
	return 0;
}
