/////////////////////////////////////////////////////////
//  ece4709 advanced programming 
//	symbolic calculator project
//  
//  jiangbin 2004/9/23 student ID: 19213123
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// file name: tree.h
// In this file, a tree class is defined for parse and
// calculate symbolic formula
/////////////////////////////////////////////////////////
#ifndef _iostream
#define _iostream
#include <iostream>
#endif

#ifndef _cmath
#define _cmath
#include <cmath> // <math.h> both OK :>
#endif

#ifndef _string
#define _string
#include <string>
#endif

//#include <fstream>
using namespace std;
#define pi 3.14159

/////////////////////////////////////////////////////////////////////
// class extsion value, include symbol component and a data component
// T is used to indicate the resolution 
// can be float, double...
//
/////////////////////////////////////////////////////////////////////
//template <typename T>
class ext_value
{
public:
	ext_value()
	{
	
	}
	~ext_value()
	{
	
	}
	void clear()
	{
		dataValue=0;
		symbolValue="";
	}
	double getdataValue()
	{
		return dataValue;
	}
	void setdataValue(double data)
	{
		dataValue=data;
	}
	string getSymbolValue()
	{
		return symbolValue;
	}
	void setSymbolValue(string value)
	{
		symbolValue=value;
	}
	string getValueAsString()
	{
		char strTemp[20];
		string strResult;
		string data_str;
		sprintf(strTemp,"%f",dataValue);
		data_str=strTemp;
		if(dataValue>0) data_str="+"+data_str;
		if(dataValue!=0 && dataValue!=1 && dataValue!=-1)
		{			
			strResult+=data_str+symbolValue;// for example 5*x=5x, both data and symbol
		}
		else if(dataValue==1)
		{
			if(  symbolValue!="")
			{
				strResult+=("+"+symbolValue);// 1*x=x
			}
			else
			{
				strResult+=data_str; //   if symbol=="", only data
			}
		}
		else if(dataValue==-1)
		{
			if(  symbolValue!="")
			{
				strResult+=("-"+symbolValue);// 1*x=x
			}
			else
			{
				strResult+=data_str; //   if symbol=="", only data
			}
		}
		else
		{
			strResult="0";
		}
		return strResult;
	}
	ext_value operator=(ext_value b)
	{
		dataValue=b.dataValue;
		symbolValue=b.symbolValue;
		return b;
	}
	ext_value operator=(string b)
	{
		int bLeftOfPoint;
		const char *pStr=b.c_str();
		double temp=0;
		bLeftOfPoint=1;
		if(*pStr<0x39 && (*pStr>=0x30))
		{
			temp=atof(pStr);
			dataValue=temp;
			symbolValue="";
		}
		else
		{
			dataValue=1;
			symbolValue=b;
		}
		return *this;;
	}
	ext_value operator*(ext_value b)
	{
		ext_value temp=*this; // don't want change the origin value
		temp.dataValue*=b.dataValue;

		if(symbolValue==b.symbolValue && symbolValue!="")
		{
			temp.symbolValue=b.symbolValue+"^2";
		}
		else
		{
			temp.symbolValue+=b.symbolValue;
		}
		if(temp.dataValue==0) b.symbolValue=""; // 0 times anything =0
		return temp;
	}
	ext_value operator/(ext_value b)
	{
		ext_value temp=*this;
		if(b.dataValue==0)
		{
			temp.symbolValue="inf(divid by 0)";
			temp.dataValue=1; // set to 1 so won't display it
		}
		else
		{
			temp.dataValue/=b.dataValue;
			
			if(symbolValue==b.symbolValue && symbolValue!="")
			{
				temp.symbolValue="";
			}
			else if(b.symbolValue!="")
			{
				temp.symbolValue+="/"+b.symbolValue;
			}
			else
			{
				temp.symbolValue+=symbolValue;
			}
			if(temp.dataValue==0) b.symbolValue=""; // 0 divid anything not zero =0
			return temp;
		}
	}
	ext_value mexp(ext_value b)
	{
		ext_value temp;
		
		if(b.symbolValue=="")
		{
			temp.dataValue=exp(b.dataValue);
			temp.symbolValue="";
		}
		else if(b.symbolValue=="j")
		{//only calculate exp(j*pi),exp(j*pi/2),exp(-j*pi), exp(-j*pi/2)
			if(b.dataValue==0)
			{
				temp.symbolValue="";
				temp.dataValue=-1;
			}
			else if(b.dataValue==pi/2)
			{
				temp.symbolValue="j";
				temp.dataValue=1;
			}
			else if(b.dataValue==-pi/2)
			{
				temp.symbolValue="j";
				temp.dataValue=-1;
			}
			else if(b.dataValue==pi)
			{
				temp.symbolValue="";
				temp.dataValue=1;
			}
			else
			{
				temp.symbolValue="e^("+b.getValueAsString()+")";
				temp.dataValue =1;
			}
		}
		else
		{	
			temp.symbolValue="e^("+b.getValueAsString()+")";
			temp.dataValue =1;
		}
		
		return temp; //*this
	}
	ext_value msin(ext_value b)
	{
		ext_value temp;	
		if(b.symbolValue =="")
		{
			temp.dataValue=sin(b.dataValue);
			temp.symbolValue="";
		}
		else
		{
			temp.symbolValue="sin("+b.getValueAsString()+")";
			temp.dataValue =1;
		}
		return temp;
	}
private:
	//float
	double	dataValue;
	string symbolValue;	
};

/////////////////////////////////////////////////////////////////////
// class Polynomial_value, model a polynomial
// the value is stored in a vector<ext_value>,
// it can represent, for example 8x^2+5x+6x+7
/////////////////////////////////////////////////////////////////////
class Polynomial_value
{
public:
	
	vector<ext_value> getValue()
	{
		return value;
	}
	string getValueAsString()
	{
		string strResult;
		for(int i=0;i<value.size();i++)
		{
			strResult+=value[i].getValueAsString();
		}
		return strResult;
	}
	void setValue(vector<ext_value> mvalue)
	{
		value=mvalue;
	}
	void clear()
	{
		int i;
		for(i=0;i<value.size();i++)
		{
			value[i].clear();
		}
	}
	Polynomial_value operator=(Polynomial_value b)
	{
		value=b.value;
		return b;
	}
	Polynomial_value operator=(string b)
	{
		ext_value temp;
		temp=b;
		value.clear();
		
		value.push_back(temp);
		
		return *this;
	}
	Polynomial_value operator+(Polynomial_value b)
	{
		int i,j;
		
		// if has same symbol value, keep it as the new symbol value,while add their datavalue (ex: 5x+6x=11x)
		for(i=0;i<value.size();i++)
		{	
			for(j=0;j<b.value.size();j++)
			{
				if(value[i].getSymbolValue()==b.value[j].getSymbolValue())
				{
					value[i].setdataValue(value[i].getdataValue()+b.value[j].getdataValue());
					b.value.erase(b.value.begin()+j,b.value.begin()+j+1);
					j--;
				}				
			}
		}
		for(j=0;j<b.value.size();j++)
		{
			ext_value temp;
			temp=b.value[j];
			value.push_back(temp);//(b.value[j]);
		}
		return *this;
	}
	Polynomial_value operator*(Polynomial_value b)
	{
		int i,j;
		
		vector<ext_value> temp;
		
		for(i=0;i<value.size();i++)
		{	
			for(j=0;j<b.value.size();j++)
			{
				temp.push_back(value[i]*b.value[j]);
			}
		}
		value=temp;
		
		return *this;
	}
	Polynomial_value operator/(Polynomial_value b)
	{
		int i;
		
		vector<ext_value> temp;
		ext_value temp_value;
				
		if(b.value.size()==1)
		{
			for(i=0;i<value.size();i++)
			{
				temp_value=value[i]/(b.value[0]);
				temp.push_back(temp_value);
			}
		}
		else
		{
			temp_value.setdataValue (1);
			temp_value.setSymbolValue (getValueAsString()+"/("+b.getValueAsString()+")");
			temp.push_back(temp_value);
		}
		value=temp;
		return *this;
	}
	Polynomial_value mexp(Polynomial_value b)
	{
		//int i;
		string mstr;
		Polynomial_value temp_polynomial;
		ext_value temp_value;
		if(b.value.size()==1)
		{
			temp_value=temp_polynomial.getValue()[0].mexp(b.value [0]);			
		}
		else
		{
			temp_value.setdataValue (1);
			temp_value.setSymbolValue ("exp("+b.getValueAsString()+")");
		}
		
		temp_polynomial.value.push_back(temp_value);
		return temp_polynomial;
	}
	Polynomial_value msin(Polynomial_value b)
	{
		//int i;
		string mstr;
		Polynomial_value temp_polynomial;
		ext_value temp_value;
		if(b.value.size()==1)
		{
			temp_value=temp_polynomial.getValue()[0].msin(b.value [0]);
		}
		else
		{
			temp_value.setSymbolValue ("exp("+b.getValueAsString()+")");
		}
		
		temp_polynomial.value.push_back(temp_value);
		return temp_polynomial;
	}
private:
	vector<ext_value> value;
};


class sumPolicy
{
	public:
	//template <typename T>
	static Polynomial_value accumunate(Polynomial_value a,Polynomial_value b) // use of policy, ref to lecture notes
	{
		Polynomial_value total;	
		total=a+b;
		return total;
	}
};
class minusPolicy
{
	public:
	//template <typename T>
	static Polynomial_value accumunate(Polynomial_value a,Polynomial_value b) // use of policy, ref to lecture notes
	{
		Polynomial_value temp,total;
		vector<ext_value> m_value;
		int i;
		temp=b;
		m_value=temp.getValue();
		
		for(i=0;i<m_value.size();i++)// minus is equivalent to inverse the sign then doing add
		{							//  should inverse each component of the polynomial
			m_value[i].setdataValue (-m_value[i].getdataValue()); 
		}
		temp.setValue(m_value);
		
		total=a+temp;
		return total;
	}
};
class mulPolicy
{
	public:
	//template <typename T>
	static Polynomial_value accumunate(Polynomial_value a,Polynomial_value b) // use of policy, ref to lecture notes
	{
		Polynomial_value total;	
		total=a*b;
		return total;
	}
};
class dividPolicy
{
	public:
	//template <typename T>
	static Polynomial_value accumunate(Polynomial_value a,Polynomial_value b) // use of policy, ref to lecture notes
	{
		Polynomial_value total;	
		total=a/b;
		return total;
	}
};

//template <typename T>//Policy=sumPolicy>
class Accum
{
public:
	static Polynomial_value accum(Polynomial_value a,Polynomial_value b,string strFunction)
	{
		Polynomial_value temp;
		if(strFunction=="+")				
			temp=sumPolicy::accumunate(a,b);
		else if(strFunction=="-")
		{
			temp=minusPolicy::accumunate(a,b);
		}		
		else if(strFunction=="*")
		{
			temp=mulPolicy::accumunate(a,b);
		}
		else if(strFunction=="/")
		{
			temp=dividPolicy::accumunate(a,b);
		}
		else if(strFunction =="exp")
		{
			temp=temp.mexp(b);
		}
		else if(strFunction =="sin")
		{
			temp=temp.msin(b);
		}
					
		
		return temp;
	}
};
////////////////////////////////////////////////////////////////////////////////
// class: treeNode
//
////////////////////////////////////////////////////////////////////////////////

template <typename T>
class treeNode
{
public:
	treeNode();
	~treeNode();
	treeNode<T>* getLeftChild()
	{
		return pLeftChild;
	};
	treeNode<T>* getRightChild()
	{
		return pRightChild;
	}
	treeNode<T>* getParent()
	{
		return pParent;
	}
	treeNode<T>* createChild(int leftRight) // zero for left, none zero for right
	{
		treeNode<T>* pLNode=new treeNode<T>;
		pLNode->pParent =this;
		if(leftRight==0)
		{
		 pLeftChild=pLNode;
		}
		else
		{
		 pRightChild=pLNode;
		} 
		return pLNode;
	}

	void setFormula(string formula);
	string getFormula();
	void parse();
	Polynomial_value calculate();
	
protected:
	treeNode<T> *pLeftChild,*pRightChild;
	treeNode<T> *pParent;
	string strFormula; // ---->>>  to be parsed
	string strResult; 
	
	//static vector<ext_value<T>> result; //can't do this?
	Polynomial_value	result;//<<<---- after calculation

	static vector<string> bracketContent;
	static int needParse;
private:
	int max_bracketNum;
	string str_math_func;
};

template <typename T>
treeNode<T>::treeNode()
{
	pLeftChild=NULL;
	pRightChild=NULL;
}
template <typename T>
treeNode<T>::~treeNode()
{
	if(pLeftChild!=NULL) pLeftChild->~treeNode();
    if(pRightChild!=NULL) pRightChild->~treeNode();
	bracketContent.clear();
	pLeftChild=NULL;
	pRightChild=NULL;	
}
template <typename T>
void treeNode<T>::setFormula(string formula)
{
	strFormula=formula;
}
template <typename T>
string treeNode<T>::getFormula()
{
	return strFormula;
}
template <typename T>
vector<string> treeNode<T>::bracketContent;
template <typename T>
int treeNode<T>::needParse;

//template <typename T>
//vector<ext_value<T>> treeNode<T>::result;

const string op[3][6]={{"+","-","end"},{"*","/","end"},\
{"sin", "cos", "tg", "ctg", "exp", "end"}};//{"sin(", "cos(", "tg(", "ctg(", "exp(", "end"}}; // sin cos .. should search full word only to avoid symbol such as "asin"
template <typename T>
void treeNode<T>::parse()
{
	int i,j,k;	
	
	if(pLeftChild==NULL && pRightChild==NULL)
	{		
		/////////////////////////////////////////////
		//1. get rid of blank and the ';' first
		/////////////////////////////////////////////
		for(i=0;i<strFormula.size();i++)
		{
			if(strFormula.substr(i,1)==" "||strFormula.substr(i,1)==";") 
			{
				strFormula.erase(i,1);
				i--;
			}
		}	
		/////////////////////////////////////////////
		//2. substitude content in bracket level 1 to $s
		/////////////////////////////////////////////
		int leftBracketPos,rightBracketPos;
		int leftBracketCnt=0,rightBracketCnt=0;
		string substitude;
				
		for(i=0;i<strFormula.size();i++)
		{
			
			if(strFormula.substr(i,1)=="(") // substr should be " " instead of ' '
			{
				if(leftBracketCnt==0)
				{
					leftBracketPos=i;
				}
				leftBracketCnt++;
			}
			if(strFormula.substr(i,1)==")")
			{
				rightBracketCnt++;
				
				if(leftBracketCnt==rightBracketCnt)
				{
					rightBracketPos=i;
					substitude="$s";//+ bracketContent.size();
				//	string test=strFormula.substr(leftBracketPos+1,2);
				//	bracketContent.push_back(test);
					bracketContent.push_back(strFormula.substr(leftBracketPos+1,rightBracketPos-leftBracketPos-1));
					strFormula.replace(leftBracketPos,rightBracketPos-leftBracketPos+1,substitude);
					i-=rightBracketPos-leftBracketPos+1-substitude.length();
					leftBracketCnt=0; // back to the normal precedence level, clear for next (...)	
					rightBracketCnt=0;

				}
			}
		}
		
		/////////////////////////////////////////////////////////////
		//3.parse 3 precedence levels of operator: first level +-, then */ ,then sin cos exp
		/////////////////////////////////////////////////////////////
		for(k=0;(k<2)&&(pLeftChild==NULL)&&(pRightChild==NULL);k++)// if the node has no child, means it has not been parsed yet
		{// do the first two level operator parse +- first, then */
			for(i=0;(i<strFormula.size())&&(pLeftChild==NULL)&&(pRightChild==NULL);i++)
			{// check if char in string from left to right for +-*/
				int length=strFormula.size();
				int length1=strFormula.length();
				for(j=0;op[k][j]!="end";j++)
				{
					if(strFormula.substr(i,1)==op[k][j])
					{
						//if find operator +-*/
						pLeftChild=createChild(0);
						pRightChild=createChild(1);
						pLeftChild->setFormula(strFormula.substr(0,i));
						pRightChild->setFormula(strFormula.substr(i+1,strFormula.length()-i));
						
						str_math_func=op[k][j];
						break;
					}
				}
			}
		}
		if((pLeftChild==NULL)&&(pRightChild==NULL))
		{// do the third level precedence operator parse
			for(j=0;op[2][j]!="end";j++)
			{
				string::size_type pos=strFormula.find(op[2][j]);// find exp sin...
				if(pos!=string::npos)
				{
					i=pos+op[2][j].length()-1;
					//if find 3rd level operator such as exp,sin,cos
					// third level operator only has one operand, store it in right child
					pRightChild=createChild(1);					
					pRightChild->setFormula(strFormula.substr(i+1,strFormula.length()-i));					
					str_math_func=op[2][j];
					break;
					
				}
			}
		}
		
		
		/////////////////////////////////////////////
		// 4.substitude $s(content in bracket) back
		/////////////////////////////////////////////
		if(strFormula=="$s")
		{
			setFormula(bracketContent[0]);//(*bracketContent.begin());
			bracketContent.erase(bracketContent.begin(),bracketContent.begin()+1);
			needParse=1;
			//pTree->setParse();
		}
		if(strFormula=="pi")
		{
			strFormula="3.14159"; //substitude pi in
		}
	}
	
	/////////////////////////////////////////////
	// recusively parse left and right child
	/////////////////////////////////////////////
	if(pLeftChild!=NULL) pLeftChild->parse();
    if(pRightChild!=NULL) pRightChild->parse();
}

////////////////////////////////////////////////////
// calculate the value(in polynomial form) of that node
////////////////////////////////////////////////////
template <typename T>
Polynomial_value treeNode<T>::calculate()
{
	Polynomial_value left,right;
	
	if(pLeftChild==NULL&&pRightChild==NULL)
	{
		if(strFormula!="")
		result=strFormula;
		
		return result;
	}
	if(pLeftChild!=NULL)
	{
		left=pLeftChild->calculate();
	}
	else
	{
		// this means 1.the +or- is prefix, no difference than  
		// 2.it is a function such as exp(..)
		// will handle this in accum, so no extra work needed here
	}
	if(pRightChild!=NULL) 
	{
		right=pRightChild->calculate();
	}
	
	result=Accum::accum(left,right,str_math_func);

	strResult=result.getValueAsString();
	return result;
	
}
////////////////////////////////////////////////////////////////////////////////
// class: tree
//
////////////////////////////////////////////////////////////////////////////////
template <typename T>
class tree: public treeNode<T>
{
public:
	tree()
	{
		pParent=NULL;
	}
	~tree()
	{		
	}
	string getResult()
	{
		return strResult;
	}
	void parsetree()
	{		
		needParse=1;
		while(needParse)
		{
			needParse=0;
			parse();			
		}
	}
	void calculatetree()
	{
		int i;
		float fTemp;
		char strTemp[20];
		string data_str;
		calculate();
		strResult=result.getValueAsString(); // change the result in 
		//polynomial form to string form, and save to strResult
	}
	void clear()
	{
		if(pLeftChild!=NULL)  delete  pLeftChild;
		if(pRightChild!=NULL) delete pRightChild;
		pLeftChild=NULL;
		pRightChild=NULL;
		bracketContent.clear();
		strResult="";
		result.clear();
	}
private:
	string strResult;
};
////////////////////////////////////////////////////////////////////////////////