#ifndef COMPILETOC_H
#define COMPILETOC_H

#include <iostream>
#include <string>
#include <vector>
#include "commonheader.h"

using namespace std;


class CompileToC
{
public:
    CompileToC();

    string compileToCIter(ParseTree_t *pTree);
    string compileToCSetNotationIter(ParseTree_t *pTree);
    string start(ParseTree_t *pTree, string typ);
    string deSugar(ParseTree_t *pTree);

    string type;

};

#endif // COMPILETOC_H
