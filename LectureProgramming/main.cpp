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
int calculateIntersections(vector<Program> prog);
bool isIntersect(Program p1, Program p2);


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
	vector<Lecture> lectures;
	public:
	int intersections;
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

};

class ProgramManager {
	vector<Program> programs;
	int nofp;
	public:

	ProgramManager() {
		nofp = 0;
	}

	vector<Program> calculateMaxPrograms();
	void textRead(string textaddr);
	
	void addProgram(Program p) {
		for (int i = 0; i < nofp; i++) {
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

	void printPrograms(vector<Program> prog) {
		int len = prog.size();
		for (int i = 0; i < len; i++)
		{
			printProgram(prog[i]);
		}

		cout << "" << endl;

	}

	void printProgram(Program pr) {
		int nfl = pr.getNofl();
		cout << pr.getName() << "\n" << pr.getCredit() << endl;
		cout << "Program Lectures: " << nfl << endl;
		cout << "" << endl;
		for (int j = 0; j < nfl; j++) {
			printLecture(pr.getLectures()[j]);
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

void ProgramManager::textRead(string textaddr) {
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

vector<Program> ProgramManager::calculateMaxPrograms() {
	//calculate intersections
	int total_intersections = calculateIntersections(programs);
	vector<Program> prg = programs;
	
	//while intersections!=0
	while (total_intersections != 0) {
		//find max intersect and erase
		int maxIntIndex = 0;
		int maxInt=0;
		int len = prg.size();
		for (int i = 0; i < len; i++)
		{
			if(prg[i].intersections>maxInt){
				maxInt = prg[i].intersections;
				maxIntIndex = i;
			}
		}

		prg.erase(next(prg.begin(), maxIntIndex));
		//recalc intersections
		total_intersections = calculateIntersections(prg);

	}
	
	
	
	return prg;
}

bool isIntersect(Program p1, Program p2) {

	vector<Lecture> p1l = p1.getLectures();
	vector<Lecture> p2l = p2.getLectures();

	int p1len = p1l.size();
	int p2len = p2l.size();

	for (int i = 0; i < p1len; i++) {
		for (int j = 0; j < p2len; j++) {

			int lec1s = p1l[i].getStartTime();
			int lec1e = p1l[i].getEndTime();
			int lec1d = p1l[i].getDay();

			int lec2s = p2l[j].getStartTime();
			int lec2e = p2l[j].getEndTime();
			int lec2d = p2l[j].getDay();

			if (lec1d == lec2d) {
				if (lec2s >= lec1s && lec2s < lec1e) return true;
				if (lec1s >= lec2s && lec1s < lec2e) return true;
			}
		}
	}

	return false;
}


int calculateIntersections(vector<Program> prog) {
	int intersections = 0;
	int len = prog.size();

	for (int i = 0; i < len; i++) {
		prog[i].intersections = 0;
	}


	for (int i = 0; i < len - 1; i++) {
		for (int j = i+1; j < len; j++) {
			if (isIntersect(prog[i], prog[j])) {
				cout << prog[i].getName() << " and " << prog[j].getName() << " are intersecting!" << endl;
				prog[i].intersections++;
				prog[j].intersections++;
				intersections++;
			}
		}
	}
	return intersections;
}



int main(void) {
	ProgramManager pr;
	pr.textRead("lectures.txt");
	vector<Program> progs = pr.calculateMaxPrograms();
	pr.printPrograms(progs);
	while (1);
	return 0;
}
