#ifndef CTL_TEST_H
#define CTL_TEST_H

#include <vector>
#include <bdd.h>
#include <ctl.h>

std::vector<bdd> ctl_test_vector()
{
    std::vector<bdd> test{
        ATOM(0),
        ATOM(2),
        EX(ATOM(2)),
        AX(ATOM(1)),
        AG(OR(NOT(ATOM(0)),EF(ATOM(2)))),
        EU(AF(ATOM(0)),EG(ATOM(1))),
        AU(ATOM(0),ATOM(1))
    };
    return test;
}; 

#endif // CTL_TEST_H

