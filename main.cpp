/*This file isn't completed yet.*/
#include "Wizard.h"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
using namespace Wizard;

class Analysis
{
public:
	vector<string> SplitStr(const string& s, const char& delim)
	{
		vector<string> tokens;
		size_t start = 0, end = 0;
		while((end=s.find(delim, start))!=string::npos)
		{
			tokens.push_back(s.substr(start, end-start));
			start=end+1;
		}
		tokens.push_back(s.substr(start));
		return tokens;
	}
	string StrSegment(const string& s, const char& fr, const char& to)
	{
		int f=s.find(fr), t=s.find(to);
		string rt=NULL;
		for(int i=f+1; i<t; i++)
			rt+=s[i];
		return rt;
	}
	vector<string> SplitFunc(const string& s)
	{
		vector<string> strs=SplitStr(s, '(');					//erase '('
		string name=strs[0];
		vector<string> poses=SplitStr(str[1].erase(str[1].size()-1), ',');	//erase ')' and ','
		int npos=poses.size();
		if(npos==2||npos==3)
			return poses;
		else return NULL;
	}
	static struct VarPoint
	{
		const int max_v=100;
		Point s[max_v]={};
		bool free[max_v]={};
		string name=NULL;
		int vi=0;
		void append(const Point& p, const bool& type, const char& n)
		{
			s[vi].x=p.x;
			s[vi].y=p.y;
			free[vi]=type;
			name[vi]=n;
			vi++;
		}
	}varp; 
	Analysis(string anal)
	{
		vector<string> strs=SplitStr(anal, ' ');
		for(int i=0; i<strs.size(); i++)
		{
			string type=(SplitStr(strs[i], '('))[0];
			if(type=="Point")
				AnalPoint(strs[i].erase(0, strs[i].find("Point")+4));
			else if(type=="intersect"||type==""/*TODO*/)
			{
				SplitFunc()
			}
		}
	}
	~Analysic(){}
	void AnalPoint(const string& anal)
	{
		for(int i=0; i<anal.size(); i++)
		{
			if(anal[i]==' '||anal[i]=='('||anal[i]==')'||anal[i]==',')continue;
			if(anal[i]=='f')
			{
				string fp=StrSegment(anal, 'f', ',');
				for(int j=0; j<fp.size(); j++)
				{
					if('A'<=fp[j]&&fp[j]<='Z')
					{
						Point fpoint(HilbertSt::st1());
						var.append(fpoint, true, fp[j]);
					}
					else continue;
				}
			}
			if(anal[i]=='c')
			{
				string cp=StrSegment(anal, 'c', ')');
				for(int j=0; j<cp.size(); j++)
				{
					if('A'<=cp[j]&&cp[j]<='Z')
					{
						Point cpoint();
						var.append(cpoint, false, cp[j]);
					}
					else continue;
				}
			}
		}
		return;
	}
	void AnalFunc(const string& anal)
	{
		
	}
}

int main(int argc, char* argv[])
{
	//./wizard --condition "Point(fABD,cCE) intersect(BC,AC,C) intersect(AC,BD,E) parallel(AB,CD) parallel(BC,AD)" --to-prove "midpoint(E,AC) midpoint(E,BD)"
	int opt;
	const char *condition = NULL;
	const char *to_prove = NULL;
	static struct option long_options[]=
	{
		{"condition", required_argument, NULL, 'c'},
		{"to-prove", required_argument, NULL, 'p'},
		{NULL, 0, NULL, 0}
	};
	while((opt=getopt_long(argc, argv, "c:p:", long_options, NULL))!=-1) 
	{
		switch(opt)
		{
		case 'c':
			condition = optarg;
			break;
		case 'p':
			to_prove = optarg;
			break;
		default:
			fprintf(stderr, "Usage: %s --condition <condition> --to-prove <to_prove>\n", argv[0]);
			return 1;
		}
	}
	
	string cond=condition;
	string prov=to_prove;
	

	return 0;
}
