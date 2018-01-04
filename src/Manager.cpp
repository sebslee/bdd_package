/********************************************//**
* \file   Manager.cpp
* \brief  Class Manager.
* \author Tobias Ludwig, Sebastian Lee Barrera, Olga Wagner
* \date   2016-12-07
***********************************************/
#include "Manager.h"
#include <iostream>

/////////////////////////////////////////////////
namespace ClassProject {

  BDD_NODE::BDD_NODE(BDD_ID index, BDD_ID top_var, BDD_ID low, BDD_ID high, std::string user_formula){
    //! Constructor for BDD_NODE class.
    this->index = index;
    this->top_var = top_var;
    this->low = low;
    this->high = high;
    this->user_formula = user_formula;
  };
  //-----------------------------------------------
  Manager::Manager() {
    //! Constructor for Manager class.
    BDD_NODE* False;
    BDD_NODE* True;
    //Default False and True nodes...
    False = new BDD_NODE(0,0,0,0, "False");
    True =  new BDD_NODE(1,1,1,1, "True");
    BDD.push_back(*False);
    BDD.push_back(*True);
    //Initialize Cache Table...
    ite_result temp;
    temp.i = 0 ; 
    temp.t = 0 ; 
    temp.e = 0 ; 
    temp.result = 0 ;

    dupla temp_dupla;
    temp_dupla.a = 0 ; 
    temp_dupla.b = 0 ;  
 
    for (unsigned int i = 0 ; i < NEG_CACHE_SIZE ; i ++) 
      NEG_CACHE[i] = temp_dupla;
    for (unsigned int i = 0 ; i < CACHE_SIZE ; i++) {
      CACHE_TABLE[i]  = temp;  
    }
    delete False;
    delete True;
   }
  //-----------------------------------------------
  Manager::~Manager() { //destructor....
    delete &BDD;
    delete &unique_table;
  }

  unsigned int Manager::Hash_Function_Cache (unsigned int i , unsigned int t , unsigned int e  ){
      unsigned int h = 0;
      h = (h * 16777619) ^ i;
      h = (h * 16777619) ^ t;
      h = (h * 16777619) ^ e;
      return (h % CACHE_SIZE);
  }
  //-----------------------------------------------
  const BDD_ID& Manager::True(){
    return BDD[1].index;	//! Return value of a node index.
  }
  //-----------------------------------------------
  const BDD_ID& Manager::False(){
    return BDD[0].index;	//! Return value of a node index.
  }
  //-----------------------------------------------
  BDD_ID Manager::createVar(const std::string &label){
    global_idx++;
    triplet temp;
    temp.a = global_idx;
    temp.b = True();
    temp.c = False();
    unique_table.insert({temp,global_idx});
    BDD.push_back(*new BDD_NODE(global_idx, global_idx, 0 , 1 , label)); //! Push new variable to the BDD.
    return global_idx;
  }
  //-----------------------------------------------
  bool Manager::isConstant (const BDD_ID f){
    return (f==this->True() || f==this->False());	//! If the index of node is 0 or 1 then this node is constant.
  }
  //-----------------------------------------------
  bool Manager::isVariable(const BDD_ID x){
    return (BDD[x].high==this->True() && BDD[x].low==this->False());	//! If value of low node index is 0 and high node index is 1 then the current node is variable.
  }

  BDD_ID Manager::find_or_add (BDD_ID top_var , BDD_ID high , BDD_ID low){
    std::unordered_map<triplet , BDD_ID>::const_iterator j;
    BDD_NODE temp;
    triplet temp_triplet;
    if (high == True() && low == True()) return True();
    if (high == False() && low == False()) return False();
    temp_triplet.a = top_var;
    temp_triplet.b = high;
    temp_triplet.c = low;
    j = unique_table.find(temp_triplet);
   //! Looking for existing node ind the BDD.
    if(j != unique_table.end())
      return j->second;
   //! If not found then pushed new node to BDD with new index.
    global_idx++;
    temp.top_var=top_var;
    temp.high=high;
    temp.low=low;
    temp.index= global_idx;
    unique_table.insert({temp_triplet ,global_idx});
    BDD.push_back(temp);
    return temp.index;
  }

  BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
	//!Terminal cases of recursion...
    if (i == 1) return t;	//! ite(1, f, g) = f
    if (t == e) return t;     //! ite(g, f, f) = f
    if (i == 0) return e;	//! ite(0, g, f) = f
    if (t == 1 && e == 0) return i;	//! ite(f, 1, 0) = f
    ite_result tmp ;
    //!Searching the computed cache table for ite
    tmp = CACHE_TABLE[Hash_Function_Cache(i,t,e)];
    if (tmp.i == i && tmp.t == t && tmp.e == e){
       #ifdef CACHE_STATS
       cache_hit++;
       #endif
       return tmp.result;      
    }
    BDD_ID x; //! Let x be top var of f,g,h.
    //! Get top var based on index, lowest idx from i t e (excluding 0 and 1)    
   x = BDD[i].top_var;  // covered by terminal case i>1
   if ( x > BDD[t].top_var && t > 1 ) 
     x = BDD[t].top_var;
   if (x > BDD[e].top_var  &&  e > 1 )
     x = BDD[e].top_var;
  
    BDD_ID rhigh, rlow, f, g, h;
	//! Recursive ite operation for high branch.
    f = coFactorTrue_ite(i,x);
    g = coFactorTrue_ite(t,x);
    h = coFactorTrue_ite(e,x);
    rhigh = ite(f,g,h);

	//! Recursive ite operation for low branch.
    f = coFactorFalse_ite(i,x);
    g = coFactorFalse_ite(t,x);
    h = coFactorFalse_ite(e,x);
    rlow = ite(f,g,h);

    if (rhigh == rlow) return rhigh;	//! ite(g, f, f) = f
    BDD_ID r = find_or_add(x, rhigh, rlow);
    tmp.i = i; 
    tmp.t = t;
    tmp.e = e;
    tmp.result = r; 
    //!Replacing always cache bucket (no collision chain, replacement last computed)
    CACHE_TABLE[Hash_Function_Cache(i,t,e)] = tmp;
    return r;
  }

  BDD_ID Manager::coFactorTrue_ite(const BDD_ID f, BDD_ID x){
      if (BDD[f].top_var == x)
        return BDD[f].high;	//! Reduce BDD in case that f is high.
      else
        return f;
  }

  BDD_ID Manager::coFactorFalse_ite(const BDD_ID f, BDD_ID x) {
      if (BDD[f].top_var == x)
        return BDD[f].low;	//! Reduce BDD in case that f is low.
      else
        return f;
  }

  size_t Manager::topVar(const BDD_ID f){
    return BDD[f].top_var;	//!	Return top variable of node with id f.
  }

  BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b){
    return ite(a,b,this->False());	//! BDD construction for and operation.
  }

  BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b){
    return ite(a,this->True(),b);	//! BDD construction for or operation.
  }

  BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b){
    return ite(a, neg(b) , b );	//! BDD construction for exclusive or operation.
  }

  BDD_ID Manager::neg(const BDD_ID a){
   
    if(isVariable(a)) return find_or_add(BDD[a].top_var, BDD[a].low , BDD[a].high); //! Reversing order if variable, avoid lookup...

    if(NEG_CACHE[  (a * 16777619 ^ a) % NEG_CACHE_SIZE].a == a) //!Search the neg_cache first!
      return NEG_CACHE[  (a * 16777619 ^ a) % NEG_CACHE_SIZE].b ;
    
    BDD_ID neg_res;

    neg_res = ite(a,0,1);

    dupla tmp_dupla;
 
    tmp_dupla.a =  a ; 
    tmp_dupla.b = neg_res;
    
    NEG_CACHE[ (a * 16777619 ^ a) % NEG_CACHE_SIZE] = tmp_dupla; //! No replacement policy. Replacing everytime the hash function is fullfiled....
    return neg_res; 
  }

  BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b) {
    return neg(and2(a,b));	//! Negation and2.
  }

  BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b){
    return neg (or2(a,b));	//! Negation or2.
  }

  BDD_ID Manager::coFactorTrue(const BDD_ID f){
    return BDD[f].high;	//! Reduce BDD in case that f is high.
  }

  BDD_ID Manager::coFactorFalse(const BDD_ID f){
    return BDD[f].low;	//! Reduce BDD in case that f is low.
  }

  BDD_ID Manager::coFactorTrue(BDD_ID idx , BDD_ID x){
    if(idx == False() || idx == True()) return idx;
    else if (BDD[idx].top_var == x) return BDD[idx].high;
    BDD_ID high = coFactorTrue(BDD[idx].high, x);
    BDD_ID low = coFactorTrue(BDD[idx].low , x);
    return find_or_add (BDD[idx].top_var , high, low);
  }

  BDD_ID Manager::coFactorFalse(BDD_ID idx , BDD_ID x){
    if(idx == False() || idx == True()) return idx;
    else if (BDD[idx].top_var == x) return BDD[idx].low;
    BDD_ID high = coFactorFalse(BDD[idx].high, x);
    BDD_ID low = coFactorFalse(BDD[idx].low , x);
    return find_or_add (BDD[idx].top_var , high, low);
  }

  void Manager::PrintBDD(BDD_ID idx){
    cout << "\nNODE ID " << idx << "\nTOP VAR : " << BDD[idx].top_var << "\nHIGH: " << BDD[idx].high << "\nLOW: " << BDD[idx].low << "\n";
    if (idx == True() || idx == False()) return;
    else {
      PrintBDD(BDD[idx].high);
      PrintBDD(BDD[idx].low);
    }
  }

//Missing bench methods...
  std::string Manager::getTopVarName(const BDD_ID& root){
    //  return mVarNameMap[topVar(root)];
    return BDD[root].user_formula;
  }
  void Manager::findNodes(const BDD_ID& root, std::set<BDD_ID>& nodes_of_root){
      std::pair<std::set<BDD_ID>::iterator,bool> ret = nodes_of_root.insert(root);
      if(!ret.second) {return;}
      if(isConstant(root)) {return;}
      findNodes(coFactorTrue(root),nodes_of_root);
      findNodes(coFactorFalse(root),nodes_of_root);
  }
  void Manager::findVars(const BDD_ID& root, std::set<BDD_ID>& vars_of_root) {
      if(isConstant(root)) {return;}
      std::set<BDD_ID> nodes_of_root;
      findNodes(root, nodes_of_root);
      for( std::set<BDD_ID>::iterator it=nodes_of_root.begin();
  	 it!=nodes_of_root.end();
  	 ++it) {
  	vars_of_root.insert(topVar(*it));
      }
      //remove constants again
      vars_of_root.erase(0);
      vars_of_root.erase(1);
  }

}



/////////////////////////////////////////////////
//***********************************************
