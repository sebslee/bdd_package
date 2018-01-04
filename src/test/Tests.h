
#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"
#include <iostream>
using namespace std;
//Manager Test fixture...

class ManagerTestFixture: public testing::Test {
 protected:
  ClassProject::Manager* manager;
 public:
  ManagerTestFixture(){
    manager= new ClassProject::Manager();
  }
};

//Test initial indexes for True and False nodes are created at construction of the manager class...
TEST_F(ManagerTestFixture, True_False_Test){
  EXPECT_EQ(0,manager->False()); //0 is fixed index for FALSE node...
  EXPECT_EQ(1,manager->True()); // 1 is fixed index for TRUE node...
}

//Test is Constant and is Variable. Add a random variable, save the node then check if our method returns if the node index represents a variable... Check the constants for the fixed nodes 1 and 0.
TEST_F(ManagerTestFixture, IsConstantVar_Test){

  ClassProject::BDD_ID idx;
  idx = manager-> createVar("a"); //create var
  EXPECT_EQ(1,manager->isVariable(idx));
  EXPECT_EQ(1,manager->isConstant(0));
  EXPECT_EQ(1,manager->isConstant(1));
  ASSERT_FALSE(manager->isConstant(idx));
  ASSERT_FALSE(manager->isVariable(manager->False()));
  ASSERT_FALSE(manager->isVariable(manager->True()));
}


//Test top var method...

TEST_F (ManagerTestFixture , Top_Var_Test){
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  a = manager->createVar("a");
  b = manager->createVar("b");
  //redundancy case...
  EXPECT_EQ (a, manager->topVar(a));
  //A has higher ranking, therefore the top node of the resulting BDD should have a as top var..
  EXPECT_EQ (a , manager->topVar(manager->or2(a,b)));
}

//ITE
TEST_F (ManagerTestFixture, ITE_Terminal_Cases){
// terminal cases of recursion
//  a) ite(1, f, g) = ite(0, g, f) = ite(f, 1, 0) = ite(g, f, f) = f
//  b) ite(f, 0, 1) = ~f

  ClassProject::BDD_ID f;
  ClassProject::BDD_ID g;
  ClassProject::BDD_ID not_f;
  ClassProject::BDD_ID res;

  f = manager->createVar("f");
  g = manager->createVar("g");
  not_f = manager->neg(f);
  EXPECT_EQ (f, manager->ite(manager->True(), f , g));
  EXPECT_EQ (f , manager->ite(manager->False(), g , f));
  EXPECT_EQ (f, manager->ite(f, manager->True(), manager->False()));
  EXPECT_EQ (f, manager->ite (g,f,f));
  res= manager->ite(f,0,1);
  EXPECT_EQ (res, not_f);

}

TEST_F (ManagerTestFixture , ITE_AND_TEST) {
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID result;
  a=manager->createVar("a");
  b=manager->createVar("b");
  result=manager->ite(a,b,0); // Simple and test...
  EXPECT_EQ(a, manager->BDD[result].top_var);
  EXPECT_EQ(b, manager->BDD[result].high);
  EXPECT_EQ(0, manager->BDD[result].low);
}

TEST_F (ManagerTestFixture , ITE_OR_TEST) {
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID result;
  a=manager->createVar("a");
  b=manager->createVar("b");
  result=manager->ite(a,1,b); // Simple OR test...
  EXPECT_EQ(a, manager->BDD[result].top_var);
  EXPECT_EQ(b, manager->BDD[result].low);
  EXPECT_EQ(1, manager->BDD[result].high);
}

TEST_F (ManagerTestFixture , ITE_XOR_TEST) {
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID not_b;
  ClassProject::BDD_ID result;
  a=manager->createVar("a");
  b=manager->createVar("b");
  not_b = manager->find_or_add(b , 0 , 1); // create not B
  result=manager->ite(a,not_b,b); // Simple XOR test...
  EXPECT_EQ(a, manager->BDD[result].top_var); // top var should be a...
  EXPECT_EQ(not_b, manager->BDD[result].high);
  EXPECT_EQ(b, manager->BDD[result].low);
}

//Testing individual functions...
TEST_F (ManagerTestFixture , NAND_test){
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID f;
  a = manager->createVar("a");
  b = manager->createVar("b");
  f = manager->nand2(a,b);
  EXPECT_EQ(manager->False(), manager->coFactorTrue(manager->coFactorTrue(f)));
  EXPECT_EQ(manager->True(), manager->BDD[f].low);
}

TEST_F (ManagerTestFixture , NOR_test){
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID f;
  a = manager->createVar("a");
  b = manager->createVar("b");
  f = manager->nor2(a,b);
  EXPECT_EQ(manager->False(), manager->BDD[f].high);
  EXPECT_EQ(manager->True(), manager->coFactorFalse(manager->coFactorFalse(f)));
}
//Function tests...
TEST_F (ManagerTestFixture , Function_test_1){
// f = (a | b) & c

  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID c;
  ClassProject::BDD_ID a_or_b;
  ClassProject::BDD_ID a_or_b_and_c;
  ClassProject::BDD_ID temp;

  a = manager->createVar("a");
  b = manager->createVar("b");
  c = manager->createVar("c");
  a_or_b = manager->or2(a, b);
  a_or_b_and_c = manager->and2(manager->or2(a,b), c);

  EXPECT_EQ (c , manager->coFactorTrue(a_or_b_and_c, a));
  temp = manager->coFactorFalse(a_or_b_and_c, a);
  EXPECT_EQ (manager->BDD[temp].high , c);
  EXPECT_EQ (manager->BDD[temp].low , manager->False());
  EXPECT_EQ (manager->BDD[temp].top_var , b );

}


TEST_F (ManagerTestFixture , Function_test_2){

  // f = (a + b) (c * d)

  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID c;
  ClassProject::BDD_ID d;
  ClassProject::BDD_ID f;

  a = manager -> createVar("a");
  b = manager -> createVar("b");
  c = manager -> createVar("c");
  d = manager -> createVar("d");

  f = manager -> and2 (manager->or2(a,b) , manager->and2(c,d));

  EXPECT_EQ (manager->True(), manager->coFactorTrue(manager->coFactorTrue(manager->coFactorTrue(f))));
  EXPECT_EQ (manager->False(), manager->coFactorFalse(manager->coFactorFalse(f)));

}

TEST_F (ManagerTestFixture , Function_3){
  // f = (a*b)+(a*~b) = a
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID f;
  std::unordered_map<triplet , ClassProject::BDD_ID>::const_iterator j;
  a = manager->createVar("a");
  b = manager->createVar("b");
  f = manager->or2(manager->and2(a,b), manager->and2(a , manager->neg(b)));
  EXPECT_EQ (f , a);
  f = manager->or2(manager->and2(a,b), manager->and2(a , manager->neg(b))); 
//      for( const auto& n : manager->cache_table ) {
//        std::cout <<  " i : " << n.first.a << " t : "<< n.first.b << " e : " << n.first.c <<   " Value " << n.second << "\n";
//    }

}

TEST_F  (ManagerTestFixture , CofactorTrueTest){
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID c;
  ClassProject::BDD_ID f;
  ClassProject::BDD_ID res;
  a = manager->createVar("a");
  b = manager->createVar("b");
  c = manager->createVar("c");
  f = manager->and2(manager->or2(a,b), c);
  //manager->PrintBDD(f);
//  f = manager->and2(a,b);
  res = manager->coFactorTrue(f, c);
//  manager->PrintBDD(res);
  //cout << "BDD TOP VAR " >> manager->BDD[res].top_var >> "\n";
//  cout << "\n" << "BDD TOP VAR " << manager->BDD[4].top_var;
//  cout << "\n" << "BDD HIGH " << manager->BDD[4].high;
//  cout << "\n" << "BDD LOW " << manager->BDD[4].low;
//manager->PrintBDD(manager->or2(a,b));
//  EXPECT_EQ(manager->True(), manager->coFactorTrue(manager->coFactorTrue(res)));
EXPECT_EQ(manager->or2(a,b) , res);
//EXPECT_EQ(manager->and2(a,b), manager->coFactorFalse2(f,c));
}

TEST_F (ManagerTestFixture , CofactorFalseTest){
  ClassProject::BDD_ID a;
  ClassProject::BDD_ID b;
  ClassProject::BDD_ID c;
  ClassProject::BDD_ID f;
  a = manager->createVar("a");
  b = manager->createVar("b");
  c = manager->createVar("c");
  f = manager->and2(manager->or2(a,b), c);
  EXPECT_EQ(manager->False(), manager->coFactorFalse(f,c));
}

#endif
