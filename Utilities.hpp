#include <bits/stdc++.h>
#define ll long long int
using namespace std;

class Utils
{
public:
    string fileType, fileName;
    void setter(string input)
    {
        bool nameFinished = false;
        for (auto &ch : input)
        {
            if (ch == '.')
            {
                nameFinished = true;
            }
            else
            {
                if (!nameFinished)
                {
                    fileName += ch;
                }
                else
                {
                    fileType += ch;
                }
            }
        }
    }
};