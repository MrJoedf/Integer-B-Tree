#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <string>
#include <math.h>
using namespace std;
// ---------------------B TREE NODE -------------------
struct Node{
  int *keys=nullptr; //dynamic array of keys
  Node** C=nullptr; //dynamic array of child nodes
  int numKeys=0;
  
  Node(int degree){
    keys = new int[degree-1];
    C = new Node*[degree];
    t=degree;

    for(int i=0; i<degree-1; i++){
      keys[i]=0;
    }

    for(int j=0; j<degree; j++){
      C[j]=nullptr;
    }
  }

  //PRINT BY LEVEL FUNCTION
  string printLvl(int lvl){

    if(lvl<=0 || this==nullptr){
      return ("Empty");
    }

    string str;
    bool isEmpty=true;
    if(lvl==1 && this!=nullptr){
      isEmpty=false;
      int q = numKeys;
      for(int i=0; i<q; i++){
        str+=to_string(keys[i])+" ";
      } 
    }else{
      for(int k=0; k<numKeys+1; k++){
        if(C[k]!=nullptr){
          Node* temp = C[k];
          isEmpty=false;
          if(temp->printLvl(lvl-1)!="Empty"){
            str+=(temp->printLvl(lvl-1));
          }else{
            return temp->printLvl(lvl-1);
          }
        }
      }
    if(isEmpty==true){
      return ("Empty");
    }
  }
    return str;
  }
};//end of struct node definition

//-------------------------B TREE ------------------------
  struct BTree{
    Node* root=nullptr;
    int split=1; //operates like a boolean, but initialized as int to save space
    int degree=0;
    Node* rightC=nullptr;

    //constructor
    BTree(int deg){
      degree=deg;
    }

    int getHeight(){
        if(root){
          int i=0;
          while(root->printLvl(i+1)!="Empty"){
            i++;
          }
          return i;
        }
      return 0;
    }

    string printHeight(){
      if(root){
        return("Height="+to_string(getHeight()));
      }
      return("The tree has no levels.");
    }
    
    void addNode(Node* parent, int& k, int index){
      int i;
      
      for(i=parent->numKeys; i>index; i--){
        parent->C[i+1]=parent->C[i];
        parent->keys[i]=parent->keys[i-1];
      }
      
      parent->numKeys++;
      parent->C[i+1]=rightC;
      parent->keys[i]=k;
    }
    //SPLIT FUNCTION
    void splitChild(Node* parent, int& k, int index){
     // cout<<"SPLITTING NODE"<<endl;
      Node* right = new Node(parent->t);

      int mid = floor(((parent->numKeys)*1.0)/2);
      int median = parent->keys[mid];

      if(mid < index){
        int i = mid+1;
        int idx = 0;
        
        while((degree-1)>i){
          right->keys[idx]=parent->keys[i];
          right->C[idx+1]=parent->C[i+1];
          i+=1;
          idx+=1;
        }

        cout<<"SPLIT INDEX: "<<idx<<endl;

        parent->numKeys=mid;
        right->numKeys=idx;
   
        addNode(right,k,(index-mid-1));
        k=parent->keys[mid];

        right->C[0]=parent->C[parent->numKeys+1];

      }else if(mid>=index){
        int i=mid;
        int idx=0;

        while((degree-1)>i){
          right->keys[idx]=parent->keys[i];
          right->C[idx+1]=parent->C[i+1];
          i++;
          idx++;
        }

        parent->numKeys=mid;

        addNode(parent, k, index);
        parent->numKeys-=1;
        k=parent->keys[parent->numKeys];

        right->numKeys=idx;
        right->C[0]=parent->C[parent->numKeys+1];        
        }

        rightC=right;
    }

    bool numFound(Node* parent, int k){
       if(parent){
        int i=0;
        while(k>parent->keys[i] && i<parent->numKeys){
          i++;
        }
        if(parent->keys[i]==k){
          return true;
        }

        if(split==0){
          return false;
        }

        return numFound(parent->C[i], k);
      }
        return false; 
    }
    
    //Recursive add to tree function, so cout statements will clog the ticker/console
    void insertIn(Node* parent, int& k){
      split = 0;
      if(parent){
        int i=0;

        if(!numFound(parent, k)){
            if(k>parent->keys[parent->numKeys-1]){
              cout<<parent->keys[parent->numKeys-1]<<endl;
              i=parent->numKeys;
              insertIn(parent->C[i], k);
             
            }else if(k<parent->keys[i]){
              insertIn(parent->C[i], k);
            }else{
              for(i=0; i<parent->numKeys-1; i++){
                if((parent->keys[i+1]>k) && (k>parent->keys[i])){
                  insertIn(parent->C[i+1], k);
                  i++;
                  break;
                }
              }
            }//end of else statement 

            if(split==1){
              if(parent->numKeys<degree-1){
                addNode(parent, k, i);
                split=0;
                return;
              }
              splitChild(parent, k, i);
            }
        }
      }else{
        split = 1;
        rightC=nullptr;
      }
    }

    void insert(int k){
      insertIn(root, k);
      
      if(split==1){
        Node* newHead = new Node(degree);
        newHead->keys[0]=k;
        newHead->numKeys++;

        newHead->C[0]=root;
        newHead->C[1]=rightC;

        root=newHead;
      }
    }
  };

#endif
