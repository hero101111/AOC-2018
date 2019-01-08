//
//  main.cpp
//  aoc1
//
//  Created by Gabriel on 01/12/2018.
//  Copyright © 2018 Gabriel. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <algorithm>
#include <queue>
#include <deque>
#include <stack>
#include <numeric>

using namespace std;

struct Node;

struct Header
{
    
};

struct Node
{
    int name { 0 };
    int childCount { 0 };
    int metadataCount { 0 };
    vector<Node *> children;
    vector<int> metadata;
    
    int GetMetadataSum() const
    {
        return std::accumulate(begin(metadata), end(metadata), 0);
    }
    
    int GetValue() const
    {
        if (childCount == 0)
        {
            if (GetMetadataSum() == 0)
            {
                int k = 0; ++k;
            }
            return GetMetadataSum();
        }
        else
        {
            int t = 0;
            for (int mIndex : metadata)
            {
                if (mIndex >= 1 && mIndex <= children.size())
                {
                    t += children[mIndex - 1]->GetValue();
                }
            }
            if (t == 0)
            {
                int ii = 0; ++ii;
            }
            return t;
        }
    }
};

vector<string> regexMatch(string s, string regex)
{
    std::regex re2(regex);
    std::smatch match2;
    std::regex_search(s, match2, re2);
    vector<string> ret;
    for (int i = 1; i <= match2.size(); ++i)
        ret.push_back(match2.str(i));
    return ret;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ifstream f;
    f.open("day8r.txt");
    string s;
    
    int n = 0, v = 0;
    int childNodes = -1;
    int metadataCount = -1;
    
    map<int, Node> nodes;
    stack<Node *> nodeStack;;
    
    while (f >> v)
    {
        if (childNodes < 0)
        {
            childNodes = v;
            continue;
        }
        else if (metadataCount < 0)
        {
            metadataCount = v;
            
            Node * currentNode = &nodes[nodes.size() + 1];
            currentNode->name = nodes.size();
            currentNode->childCount = childNodes;
            currentNode->metadataCount = metadataCount;
            if (childNodes == 0 && metadataCount > 0)
                f >> v; // advance read
            
            if (!nodeStack.empty())
            {
                Node * stackNode = nodeStack.top();
                if (stackNode->children.size() < stackNode->childCount)
                    stackNode->children.push_back(currentNode);
            }
            
            nodeStack.push(currentNode);
        }
        
        if (nodeStack.size() > 0 && childNodes >= 0 && metadataCount >= 0)
        {
            Node * stackTop = nodeStack.top();
         
            if (stackTop->children.size() == stackTop->childCount)
            {
                if ( stackTop->metadata.size() < stackTop->metadataCount )
                {
                    stackTop->metadata.push_back(v);
                }
                
                if (stackTop->metadata.size() == stackTop->metadataCount)
                {
                    nodeStack.pop();
                    if (!nodeStack.empty())
                    {
                        Node * newTop = nodeStack.top();
                        if (newTop->children.size() < newTop->childCount)
                        {
                            metadataCount = -1;
                            childNodes = -1;
                        }
                    }
                }
            }
            else
            {
                metadataCount = -1;
                childNodes = -1;
            }
        }
        
       // vector<string> stepS = regexMatch(s, R"(Step (\S) must be finished before step (\S) can begin.)");
        
    }
    
    for (auto & node : nodes)
    {
        n += node.second.GetMetadataSum();
    }
    cout << n << endl;
   
    cout << nodes.begin()->second.GetValue() << endl;
    
    ofstream outFile("out.data");
    
    
    return 0;
}
