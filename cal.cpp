/////////////////////////////////////////////////////////
//  ece4709 advanced programming 
//	symbolic calculator project
//  
//  jiangbin 2004/9/23 student ID: 19213123
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// file name: cal.cpp
// main file of the project,
// 1.read all formula from a file, 
// 2.store it in vector,
// 3.then put one formula from vector into parse tree
// 4.calculate the tree, 
// 5.output the result
// 6.clear the tree for next formula
// 7.do next formula in the vector untill all done.
/////////////////////////////////////////////////////////
#include <iostream>
#include <cmath> // <math.h> both OK :>
#include <string>
#include <fstream>
#include <vector>

#include "tree.h"
using namespace std;


int main()
{
	vector<string> m_v;
	string line;
	ifstream m_f("formula.txt");
	ofstream m_f1("result.txt");
	
	while(getline(m_f,line))//input the formula from a text file
	{
		if(line!="")
		m_v.push_back(line); 
	}
	tree<float> mtree;
	for(int i=0;i<m_v.size();i++)
	{	
		line=m_v[i];
		mtree.setFormula(line);// put the formula into the tree
		mtree.parsetree();     // parse it
		mtree.calculatetree(); // calculate the result
		cout<<line<<"="<< mtree.getResult()<<endl; //output the result
		m_f1<<line<<"="<< mtree.getResult()<<endl;
		mtree.clear(); // prepare to parse next formula
	}

	return 0;
}