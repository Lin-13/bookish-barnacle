#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<stdio.h>
#include"stl_point.h"
#include"windows.h"
using namespace std;


int pointNum;
int messNum;

int main(int argc, char** argv) {
	//ifstream f_stl("D:\\Document\\pointfile.txt",ifstream::in);
	//ofstream f_asc("D:Document\\test.asc",ofstream::out);
	cout << "data file " << argv[1] << endl;
	cout << "target file " << argv[2] << endl;
	ifstream f_stl(argv[1],ifstream::in);
	ofstream f_asc(argv[2],ofstream::out);
	vector<g_mess> triangle;
	vector<g_point> points;
	vector<_2points> g_crosslines;
	vector<_2points> g_crosslines_linked;
	vector<line> lines;
	double min_z = 99999, max_z = -99999;
	int thick=10;

	f_stl >> pointNum;
	cout << pointNum;
	for (int i = 0; i < pointNum; i++) {
		if (f_stl.fail())
			return -1;
		g_point temp;
		f_stl >> temp;
		//cout << temp;
		if (min_z > temp.y) min_z = temp.y;
		if (max_z < temp.y) max_z = temp.y;
		points.push_back(temp);
		
	}
	cout << " point get" << endl;
	f_stl >> messNum;
	cout << messNum;
	Sleep(2000);
	for (int i = 0; i < messNum; i++) {
		if (f_stl.fail())
			return -1;
		g_mess temp;
		f_stl >> temp;
		if (f_stl.fail()) return -1;
		//cout << temp;
		triangle.push_back(temp);

	}
	cout<<" triangle get"<<endl;
	
	if ((argv[3] != 0)&&(*argv[3] != 0)) {
		istringstream thick_get(argv[3]);
		thick_get >> thick;
	}
	cout << "thick:" << thick << endl;
	
	for (int height = (int)(min_z+1); height < (int)max_z; height +=  thick) {
		long pointNum=0;
		cout << "caculating height" << height <<" " << ends;
		for (auto j = triangle.begin(); j != triangle.end();j++) {
			_2points ar;//ar保存了穿过该三角形的两个点
			line_through_surface(points[j->p1], points[j->p2], height, ar);
			line_through_surface(points[j->p2], points[j->p3], height, ar);
			line_through_surface(points[j->p1], points[j->p3], height, ar);
			if (ar.point.size() == 2) {
				ar.mask = 0;
				g_crosslines.push_back(ar);
				pointNum++;
			}
		}
		cout << ",this height has " << pointNum << " _2dots" << endl;
		//lines.push_back(g_crosslines);
		//long unlinked_2dots = static_cast<long>(g_crosslines.size());
		line_link(g_crosslines,lines,pointNum);
	}
	cout << "printing" << endl;
	for (auto i = lines.begin(); i != lines.end();i++) {
		printLine(f_asc, *i);
	}
	
	f_stl.close();
	f_asc.close();
	cout << "done" << endl;
	return 0;
}