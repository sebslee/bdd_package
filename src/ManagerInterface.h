/********************************************//**
* \file   Manager.h
* \brief  Header file for Class ManagerInterface.
* \author Tobias Ludwig (2016-03-01), Sebastian Lee Barrera, Olga Wagner
* \date   2016-12-07
***********************************************/
#ifndef PROJECT_MANAGERINTERFACE_H
#define PROJECT_MANAGERINTERFACE_H
//***********************************************
/////////////////////////////////////////////////
#include <string>
#include <set>
//-----------------------------------------------
namespace ClassProject {

    typedef size_t BDD_ID;

    class ManagerInterface {
    public:
        virtual BDD_ID createVar(const std::string &label) = 0;

        virtual   const BDD_ID &True() = 0;

        virtual   const BDD_ID &False() = 0;

        virtual   bool isConstant(const BDD_ID f) = 0;

        virtual   bool isVariable(const BDD_ID x) = 0;

        virtual   size_t topVar(const BDD_ID f) = 0;

        virtual BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) = 0;

        virtual   BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x) = 0;

        virtual   BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x) = 0;

        virtual   BDD_ID coFactorTrue(const BDD_ID f) = 0;

        virtual   BDD_ID coFactorFalse(const BDD_ID f) = 0;

        virtual   BDD_ID and2(const BDD_ID a, const BDD_ID b) = 0;

        virtual   BDD_ID or2(const BDD_ID a, const BDD_ID b) = 0;

        virtual   BDD_ID xor2(const BDD_ID a, const BDD_ID b) = 0;

        virtual   BDD_ID neg(const BDD_ID a) = 0;

        virtual   BDD_ID nand2(const BDD_ID a, const BDD_ID b) = 0;

        virtual   BDD_ID nor2(const BDD_ID a, const BDD_ID b) = 0;
    };
}
/////////////////////////////////////////////////
//***********************************************
#endif //PROJECT_MANAGERINTERFACE_H
