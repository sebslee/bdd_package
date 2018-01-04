/********************************************//**
* \file   Manager.h
* \brief  Header file for Class Manager.
* \author Tobias Ludwig, Sebastian Lee Barrera, Olga Wagner
* \date   2016-12-07
***********************************************/
#ifndef MANAGER_H
#define MANAGER_H
//***********************************************
/////////////////////////////////////////////////
#include "ManagerInterface.h"
#include <vector>
#include <unordered_map>
#include "hash_defs.h"
struct triplet{
  int a ;
  int b ;
  int c ;
  bool operator==(const triplet &other) const
  { return (a == other.a
            && b == other.b
            && c == other.c);
  }
} ;

struct dupla {
  int a;
  int b;
  bool operator==(const dupla &other) const { return (a == other.a && b== other.b); }
};


namespace std {
  template <>
  struct hash<dupla>
  {  
       std::size_t operator()(const triplet& k) const { 
      unsigned h = 2166136261;
      h = (h * 16777619) ^ k.a;
      h = (h * 16777619) ^ k.b; 
      return h;}
  };


  template <>
  struct hash<triplet>
  {
    std::size_t operator()(const triplet& k) const
    {

    #ifdef FNV
      unsigned h = 2166136261;
      h = (h * 16777619) ^ k.a;
      h = (h * 16777619) ^ k.b;
      h = (h * 16777619) ^ k.c;
      return h;
    #endif

    #ifdef SAX
      unsigned h = 0;
      h ^= (h << 5) + (h >> 2) + k.a;
      h ^= (h << 5) + (h >> 2) + k.b;
      h ^= (h << 5) + (h >> 2) + k.c;
      return h;
    #endif

    #ifdef DJB
      unsigned h = 0;
      h = 33 * h + k.a;
      h = 33 * h + k.b;
      h = 33 * h + k.c;
      return h;
    #endif

    #ifdef OAT
        unsigned int h=0;
        h += k.a;
        h += (h << 10);
        h ^= (h >> 6);
        h += k.b;
        h += (h << 10);
        h ^= (h >> 6);
        h += k.c;
        h += (h << 10);
        h ^= (h >> 6);

        h += (h << 3);
        h ^= (h >> 11);
        h += (h << 15);
        return h;
    #endif
    }
  };

}


//-----------------------------------------------
#define CACHE_SIZE 791990
#define NEG_CACHE_SIZE 2017 //keep this stuff as small as possible :)
#define XOR_CACHE_SIZE 1000 
//-----------------------------------------------
using namespace std;
//-----------------------------------------------
namespace ClassProject {

  struct ite_result{
    unsigned int i;
    unsigned int t;
    unsigned int e;
    unsigned int result;
  };

  class  BDD_NODE {
  public:
    BDD_NODE(BDD_ID index, BDD_ID top_var, BDD_ID low, BDD_ID high, std::string user_formula);
    BDD_NODE(){};
    BDD_ID index;
    BDD_ID top_var;
    BDD_ID low;
    BDD_ID high;
    std::string user_formula;
//    BDD_NODE* next; //! to be used on hashing...
  } ;
  //-----------------------------------------------
  class Manager: public ManagerInterface {

    public:
          #ifdef CACHE_STATS
          unsigned int cache_hits = 0;
          #endif
          unsigned int global_idx=1; //! global index. 0 and 1 reserved for constants. Increment everytime we add a node or a variable.

	  ite_result CACHE_TABLE [CACHE_SIZE];
   
          dupla NEG_CACHE[NEG_CACHE_SIZE];
          
          triplet XOR_CACHE[XOR_CACHE_SIZE];
          
	  std::vector<BDD_NODE> BDD;

          std::unordered_map<triplet, BDD_ID> unique_table;

          //std::unordered_map<triplet, BDD_ID> cache_table;

           Manager();

           ~Manager();
         
           virtual void PrintBDD(BDD_ID idx);          

	   unsigned int Hash_Function_Cache (unsigned int i, unsigned t , unsigned int e );

           BDD_ID find_or_add(BDD_ID top_var, BDD_ID high, BDD_ID low);

           virtual BDD_ID createVar(const std::string &label);

           virtual const BDD_ID &True();

           virtual const BDD_ID &False();

           virtual bool isConstant(const BDD_ID f);

           virtual bool isVariable(const BDD_ID x);

           virtual size_t topVar(const BDD_ID f);

           virtual BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);

           virtual BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x);

           virtual BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x);

           virtual BDD_ID coFactorTrue(const BDD_ID f);

           virtual BDD_ID coFactorFalse(const BDD_ID f);

           virtual BDD_ID and2(const BDD_ID a, const BDD_ID b);

           virtual BDD_ID or2(const BDD_ID a, const BDD_ID b);

           virtual BDD_ID xor2(const BDD_ID a, const BDD_ID b);

           virtual BDD_ID neg(const BDD_ID a);

           virtual BDD_ID nand2(const BDD_ID a, const BDD_ID b) ;

           virtual BDD_ID nor2(const BDD_ID a, const BDD_ID b) ;

           virtual BDD_ID coFactorFalse_ite(const BDD_ID f, BDD_ID x);

           virtual BDD_ID coFactorTrue_ite(const BDD_ID f, BDD_ID x);
  
// Missing bench methods...

           std::string getTopVarName(const BDD_ID &root);
           void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);
           void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);

  };

}
/////////////////////////////////////////////////
//***********************************************
#endif
