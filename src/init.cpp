#include "init.h"

#include "groebner.h"
#include "monom.h"
#include "polynom.h"
#include "timer.h"

#include <fstream>
#include <iostream>
#include <sstream>


namespace
{
    void readVariables(std::ifstream& fin)
    {
        char c = '0';
        std::string tmpString;

        while (c != ';' && !fin.eof())
        {
            fin >> c;
            if (c == ',' || c == ';')
            {
                Monom::independ->add(tmpString.c_str());
                tmpString.clear();
            }
            else
            {
                tmpString += c;
            }
        }
    }

    void readPolynoms(std::ifstream& fin, std::vector<Polynom*>& polynoms)
    {
        std::stringstream tmpStream;
        char c = '0';
        std::string tmpString;

        while (c != ';' && !fin.eof())
        {
            fin >> c;
            if (c == ',' || c == ';')
            {
                tmpStream << tmpString << std::endl;
                tmpString.clear();
            }
            else
            {
                tmpString += c;
            }
        }

        Polynom tmpPolynom;
        while (!tmpStream.eof())
        {
            tmpStream >> tmpPolynom;
            polynoms.push_back(new Polynom(tmpPolynom));
        }
    }
}

void init(const char* filename)
{
    std::ifstream fin(filename);
    Timer timer;

    readVariables(fin);
    Monom::init();

    std::vector<Polynom*> pl;
    readPolynoms(fin, pl);

    std::vector<Polynom*> answer;
    readPolynoms(fin, answer);

    timer.start();
    GroebnerBasis bg(pl);
    timer.stop();
    std::cout << timer << std::endl;

    bool isCorrect = true, found = false;
    // check result
    if (bg.length() < answer.size())
    {
        isCorrect = false;
    }
    else
    {
        auto anIt = answer.begin();
        while (anIt != answer.end())
        {
            found = false;
            for (int i = 0; i < bg.length(); ++i)
            {
                if (*bg[i] == **anIt)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                isCorrect = false;
                std::cout << "The mistake is here: " << **anIt << std::endl;
                break;
            }
            ++anIt;
        }
    }
    // check is complete

    if (isCorrect)
    {
        std::cout << "The answer is CORRECT" << std::endl;
    }
    else
    {
        std::cout << "The answer is WRONG" << std::endl;
    }
}
