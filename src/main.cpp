/********************************************//**
* \file   Manager.cpp
* \brief  Class Manager.
* \author Tobias Ludwig, Sebastian Lee Barrera, Olga Wagner
* \date   2016-12-07
***********************************************/
#include <iostream>
#include "Manager.h"
#include "stdio.h"
/////////////////////////////////////////////////
using namespace ClassProject;
 //-----------------------------------------------
int main(int argc, char* argv[])
{
 
  Manager* manager;
  BDD_ID idx;
  manager = new Manager();
  //unsigned int& index=False();
  //index = manager->False();
   
  cout<<"\nINDEX OF FALSE "<< manager->False();
  cout<<"\nString of FALSE: "<< manager->BDD[0].user_formula;
  cout<<"\nString of TRUE: " << manager->BDD[1].user_formula;
  idx=manager->createVar("a");
  cout<<"\nIndex of var a: " << manager->BDD[idx].index <<"\nUser formula: " << manager->BDD[idx].user_formula << "\nTop var: " << manager->BDD[idx].top_var << "\n Low and High: "<< manager->BDD[idx].low << manager->BDD[idx].high << "\n";  
  cout << "Testing TRUE: " << manager->isConstant(manager->BDD[0].index) << "\n";
  cout <<  "Testing isVar: " << manager->isVariable(idx) << "\n"; 
  cout << "Negative testing for isConstant : " << manager->isConstant(idx) << "\n";
}
/////////////////////////////////////////////////
//***********************************************