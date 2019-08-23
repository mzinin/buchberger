#include "igroebner64.h"
#include "monom64.h"
#include "poly64.h"
#include "timer.h"

#include <fstream>
#include <iostream>


void init64(const char* filename)
{
    std::ifstream fin(filename);
    Timer timer;

    // read variables
    int i = 0;
    char s[524288], c = '0';
    while (c != ';')
    {
        fin >> c;
        if (c == ',' || c == ';')
        {
            Monom64::independ->add(s);
            s[0] = '\0';
            i = 0;
        }
        else
        {
            s[i] = c;
            ++i;
            s[i] = '\0';
        }
    }
    // done reading variables

    Monom64::init();

    Poly64 poly;
    std::vector<Poly64*> pl, answer;
    std::vector<Poly64*>::iterator it(pl.begin()), anIt(answer.begin());

    i = 0;
    s[0] = '\0';
    c = '0';

    std::ifstream tmpIn, tmpIn2;
    std::ofstream tmpOut;

    // read initial basis
    while (c != ';')
    {
        fin >> c;
        if (c == ',' || c == ';')
        {
            s[i] = '\n';
            ++i;
            s[i] = '\0';
        }
        else
        {
            s[i] = c;
            ++i;
            s[i] = '\0';
        }
    }
    tmpOut.open("tmp1");
    tmpOut << s;
    tmpOut.close();

    tmpIn.open("tmp1");
    while (!tmpIn.eof())
    {
        tmpIn >> poly;
        it=pl.insert(it, new Poly64(poly));
    }
    tmpIn.close();
    // done reading initial basis

    i = 0;
    s[0] = '\0';
    c = '0';

    // read correct answer
    while (c != ';')
    {
        fin >> c;
        if (c == ',' || c == ';')
        {
            s[i] = '\n';
            ++i;
            s[i] = '\0';
        }
        else
        {
            s[i] = c;
            ++i;
            s[i] = '\0';
        }
    }
    tmpOut.open("tmp2");
    tmpOut << s;
    tmpOut.close();

    tmpIn2.open("tmp2");
    while (!tmpIn2.eof())
    {
        tmpIn2 >> poly;
        anIt = answer.insert(anIt, new Poly64(poly));
    }
    tmpIn2.close();
    // done reading correct answer
    fin.close();

    timer.start();
    IGBasis64 bg(pl);
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
        anIt = answer.begin();
        while (anIt != answer.end())
        {
            found = false;
            for (i = 0; i < bg.length(); ++i)
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
    // check is

    if (isCorrect)
    {
        std::cout << "The answer is CORRECT" << std::endl;
    }
    else
    {
        std::cout << "The answer is WRONG" << std::endl;
    }
}
