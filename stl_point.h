#pragma once
#define STL_POINT
#include<iostream>
#include<fstream>
#include<math.h>
#include<windows.h>
using namespace std;
const double EPS = 0.01;
class g_point {
public:
	double x;
	double y;
	double z;
	g_point()=default;
	g_point(double x, double y, double z) :x(x), y(y), z(z) {};
private:
	friend ifstream&  operator >>(std::ifstream&,g_point&);
	friend bool operator==(const g_point& lhs,const g_point& rhs);
	friend bool operator!=(const g_point& lhs, const g_point& rhs);
	friend ofstream& operator<<(ofstream&,const g_point& point);
	friend ostream& operator<<(ostream&,const g_point&);
};
ifstream& operator >>(std::ifstream& file,g_point& it) {
	 file >> it.x;
	 file >> it.y;
	 file >> it.z;
	 return file;
}
bool operator==(const g_point& lhs, const g_point& rhs) {
	if ((fabs(lhs.x - rhs.x) + fabs(lhs.y - rhs.y) + fabs(lhs.z - rhs.z)) <= EPS){
		return 1;
	}
	else return 0;
}
bool operator!=(const g_point& lhs, const g_point& rhs) {
	if ((fabs(lhs.x - rhs.x) + fabs(lhs.y - rhs.y) + fabs(lhs.z - rhs.z)) > EPS)
		return 1;
	else return 0;
}
ofstream& operator<<(ofstream& of,const g_point &it) {
	of << it.x << " "<<it.y <<" " << it.z ;
	return of;
}
ostream& operator<<(ostream& of,const g_point& it) {
	cout << it.x << it.y << it.z ;
	return of;
}
class g_mess {
public:
	int p1;
	int p2;
	int p3;
	friend ifstream& operator >>(std::ifstream&, g_mess&);
	friend ostream& operator<<(ostream&, g_mess&);
};
ifstream& operator>>(ifstream& file, g_mess& it) {
	file >> it.p1;
	file >> it.p2;
	file >> it.p3;
	return file;
}
ostream& operator <<(ostream& os, g_mess& it) {
	os << it.p1 << " " << it.p2 << " " << it.p3 << " " << endl;
	return os;
}
class _2points {
	friend bool operator==(_2points& lhs, _2points& rhs);
public:
	vector<g_point> point;
	int mask;
	//_2points() = default;
	_2points() :point(), mask(0) {};
	int size();
	void swap();
	friend ofstream& operator<<(ofstream&,const _2points);
	friend ostream& operator<<(ostream&,const _2points);
};
int _2points::size() {
	return point.size();
}
void _2points::swap() {
	g_point temp;
	temp =this-> point[1];
	this->point[1] = this->point[0];
	this->point[0] = temp;
}

int operator==(const _2points& lhs,const _2points& rhs) {
	if ((lhs.point[0] == rhs.point[0]) && (lhs.point[1] == rhs.point[1])) return 1;
	else if ((lhs.point[0] == rhs.point[1]) && (lhs.point[1] == rhs.point[0])) return -1;
	else return 0;
}
int is_link(const _2points& lhs, const _2points& rhs,int m) {
	if (lhs.point[m] == rhs.point[0]) return 1;
	if (lhs.point[m] == rhs.point[1]) return -1;
	else return 0;
};
ofstream& operator<<(ofstream& of,const _2points it) {
	of << it.point[0]<<endl<<it.point[1]<<endl;
	return of;
}
ostream& operator<<(ostream& of,const _2points it) {
	of << it.point[0] <<endl<< it.point[1]<<endl;
	return of;
}
using line = vector<_2points>;

bool line_through_surface(const g_point p1, const g_point p2, const double y,_2points& crosspoint) {
	if ((p1.y - y) * (p2.y - y) < 0) {
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		double dz = p1.z - p2.z;
		double x = p2.x + (y - p1.y) / dy * dx;
		double z = p2.z + (y- p1.y) / dy * dz;
		g_point temp(x,y, z);
		crosspoint.point.push_back(temp);
		return 1;
	}
	else return 0;
}
bool not_in_middle(vector<_2points>::iterator i, vector<_2points>& _2dots_arr) {
	int cnt1 = 0;
	int cnt2 = 0;
	for (auto& j : _2dots_arr) {
		if ((i->point[0] == j.point[0]) || (i->point[0] == j.point[1])) {
			cnt1++;
		}
		if ((i->point[1] == j.point[0]) || (i->point[1] == j.point[1])) {
			cnt2++;
		}

	}
	if ((cnt1 >= 1) && (cnt2 >= 1)) {
		return 0;
	}
	else {
		return 1;
	}
}
void line_flip(line& arr) {
	if (arr.size() == 1) {
		arr[0].swap();
		return;
	}
	for (auto i = 0; i < (size_t)((arr.size())/2);i++) {
		arr[i].swap();
		arr[arr.size() - i-1].swap();
		_2points temp;
		temp = arr[i];
		arr[i] = arr[arr.size()-1 - i];
		arr[arr.size() -1- i] = temp;
	}
}
bool line_link(vector<_2points>& _2dots_arr, vector<line>& lines, long& unlinked_2dots) {
	if (_2dots_arr.empty()) return 0;
	line templine;
	long long templine_cnt = 0;
	for (vector<_2points>::iterator i = _2dots_arr.begin(); i != _2dots_arr.end();i++) {
		if ((i->mask == 0)) {
			i->mask = 1;
			templine.push_back(*i);
			unlinked_2dots--;
			templine_cnt++;
			break;
		}
	}
	if (templine_cnt == 0) return 0;
	int not_a_circle = 0;
	//cout << "linking  "<<templine[0] << endl;
	for (int i = 0; i < 2; i++) {
		while ((templine[templine_cnt - 1].point[1] != (templine[0].point[0])) && (unlinked_2dots > 0)) {//找到一条线
			int ret = 0;
			auto shortcut = templine_cnt;
			//cout << "islinking" << endl;
			for (vector<_2points>::iterator i = _2dots_arr.begin(); i != _2dots_arr.end(); ) {
				//for (auto& i:_2dots_arr) {
				if ((i->mask == 0) && ((ret = is_link(templine[templine_cnt - 1], *i, 1)) != 0)) {
					//cout << "is_linking" << endl;
					if (ret == 1) {
						i->mask = 1;
						templine.push_back(*i);
						unlinked_2dots--;
						templine_cnt++;
						//cout << "link reght" << endl;
					
					}
					else if (ret == -1) {
						i->swap();
						i->mask = 1;
						templine.push_back(*i);
						unlinked_2dots--;
						templine_cnt++;
						//cout << "link right" << endl;
						
					}

				}
				i++;
				if (shortcut != templine_cnt) {
					//cout << "linking " << templine[templine_cnt - 1] << endl;
					break;
				}
				if ((i == _2dots_arr.end()) && (shortcut == templine_cnt)) {
					//cout << "count" << endl;
					//line_link(_2dots_arr, lines, unlinked_2dots);
					not_a_circle = 1;
				}
			}
			if (not_a_circle) {
				break;
			}
		}
		if ((not_a_circle) && (i == 0)&&(unlinked_2dots>0)) {
			line_flip(templine);
		}
		else break;
	}
	lines.push_back(templine);
	if (unlinked_2dots>0) {
		//cout << "counting" << endl;
		//Sleep(1000);

		line_link(_2dots_arr, lines, unlinked_2dots);
		return 0;
	}
	else return 0;
}
ofstream& printLine(ofstream& file, const vector<_2points>& lin_to_print) {
	for (auto i = lin_to_print.cbegin(); i != lin_to_print.cend();i++) {
		//file << i;
		if (i == lin_to_print.begin()) {
			file << i->point[0] << endl;
			continue;
		}
		file << i->point[0]<<endl;
	}
	//file << "------------------" << endl;
	return file;
}
