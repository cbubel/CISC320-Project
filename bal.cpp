#include "bal.h"
#include "stdio.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
using namespace std;

float variables[26];

bool kill = false;

Node *root;

bool makeTree(char input[]) {
  Node *current = root;

  // current->parent = NULL;
  int armCount = 1;

  if (input[0] != '['){
    cout << "Bad input, the first character should be a \'[\'" << endl;
    return false;
  }

  string numAsString = "";

  // for (int i = 0; i < input.size(); i++) {
  int i = 1;
  int numOpen = 1;
  while(numOpen != 0) {
  // while(i < 13) {
    int charVal = (int) input[i];

    if (charVal > 47 && charVal < 58) { // number from 0 to 9
        // numAsString.append(input[i]);
        numAsString += input[i];
    }
    else {
        if(numAsString != "") {
          // finish
          float val = stof(numAsString);
          if (current->left == 0) {
            current->left = new Node(false, false, true, val + 1, -1);
            current->left->parent = current;
          }
          else if (current->right == 0) {
            current->right = new Node(false, false, false, val + 1, -1);
            current->right->parent = current;
          }
          numAsString = "";
        }
        else {
        }
    }

    if (charVal > 64 && charVal < 91){ // capital letter from A-Z
      if (current->left == 0) {
        current->left = new Node(false, true, true, 0, valToIndex(charVal));
        current->left->parent = current;
      }
      else if (current->right == 0) {
        current->right = new Node(false, true, false, 0, valToIndex(charVal));
        current->right->parent = current;
      }
    }

    else if (charVal == 91 && armCount <= 50) { // [ case
      numOpen++;
        if (current->left == 0){
            current->left = new Node(true, false, true, 0, -1);
            armCount++;
            current->left->parent = current;
            current = current->left;
         }
        else if (current->right == 0){
            current->right = new Node(true, false, false, 0, -1);
            armCount++;
            current->right->parent = current;
            current = current->right;
        }
        else{
            cout << "Brackets do not match, please correct input" << endl;
            return false;
        }
    }

    else if (charVal == 93) { // ] case
      numOpen--;
      current = current->parent;
    }

    else if (charVal != 32 && charVal != 0 && !(charVal > 47 && charVal < 58)) { // invalid character
        cout << "Invalid character, please correct input" << endl;
        return false;
    }
    i++;
  }

}

int valToIndex(int c) {
  return c - 65;
}

int balance(Node *current) {
  // cout << "got into balance" << endl;
  if(kill) {
    return 0;
  }
  // cout << current->index << endl;
  if(current->isKnown && current->leftKnown &&
    current->rightKnown && current->parent == 0) {
    kill = true;
    return 0;
  }
  else if(current->isKnown && current->leftKnown &&
  current->rightKnown && current->parent != 0) {
    if(current->isLeftChild) {
      current->parent->leftExplored = true;
      current->parent->leftKnown = true;
    }
    else {
      current->parent->rightExplored = true;
      current->parent->rightKnown = true;
    }
    current->parent->weight = current->weight * 2 + 1;
    current->parent->isKnown = true;
    balance(current->parent);
  }

  if(current->isBar) {
    if(current->isKnown) {
      if(current->leftKnown) {
        // go right
        if(current->right->isBar) {
          current->right->weight = (current->weight - 1) / 2;
          current->right->isKnown = true;
          balance(current->right);
        }
        else {
          balance(current->right);
        }
      }
      else if(current->rightKnown) {
        // go left
        if(current->left->isBar) {
          current->left->weight = (current->weight - 1) / 2;
          current->left->isKnown = true;
          balance(current->left);
        }
        else {
          balance(current->left);
        }
      }
      else { // not "root"
        // Only need to do left because right will automatically happen after
        if(current->left->isBar) {
          current->left->weight = (current->weight - 1) / 2;
        }
        balance(current->left);
      }
    }
    else {
      if(!current->leftExplored) {
        balance(current->left);
      }
      else if(!current->rightExplored) {
        balance(current->right);
      }
      else {
        if(current->parent != 0) { // has parent
          if(current->isLeftChild) {
            current->parent->leftExplored = true;
          }
          else {
            current->parent->rightExplored = true;
          }
          balance(current->parent);
        }
        else {
          // MANY
          cout << "MANY" << endl;
          kill = true;
          return 1;
        }
      }
    }
  }
  else { // Plate
    if(current->isVar) {
      if(current->parent->isKnown) {
        // write it
        variables[current->index] = (float)(((current->parent->weight) - 1) / 2) - 1;

        if(current->isLeftChild) {
          current->parent->leftKnown = true;
        }
        else {
          current->parent->rightKnown = true;
        }
      }
      else {
        if(current->isLeftChild) {
          current->parent->leftExplored = true;
        }
        else {
          current->parent->rightExplored = true;
        }
      }
      balance(current->parent);
    }
    else { // Constant
      if(current->parent->isKnown) {
        //compare
        if(current->weight == (current->parent->weight - 1) / 2) {
          // good
          if(current->isLeftChild) {
            current->parent->leftExplored = true;
            current->parent->leftKnown = true;
          }
          else {
            current->parent->rightExplored = true;
            current->parent->rightKnown = true;
          }
          balance(current->parent);
        }
        else {
          // NONE
          kill = true;
          cout << "NONE" << endl;
          return -1;
        }
      }
      else { // parent weight not known
        if(current->isLeftChild) {
          current->parent->leftExplored = true;
          current->parent->leftKnown = true;
        }
        else {
          current->parent->rightExplored = true;
          current->parent->rightKnown = true;
        }
        current->parent->weight = 2 * current->weight + 1;
        current->parent->isKnown = true;
        balance(current->parent);
      }
    }
  }
}

void postorder(Node *root) { // A 2 -
  if(root->left != NULL) postorder(root->left);
  if(root->right != NULL) postorder(root->right);

    if(root->isBar) {
      cout << "-" << endl;
    }
    else if(root->isVar) {
      cout << (char) (root->index + 65) << endl;
    }
    else {
      cout << root->weight << endl;
    }

}

// int main(int argc, char *argv[]) {
//   char *input = argv[1];
//   for(int i = 0; i < 26; i++) {
//     variables[i] = -1;
//   }
//   root = new Node(true, false, false, 0, -1);
//   root->parent = NULL;
//   makeTree(input);
//   // postorder(root);
//   balance(root);
//   for(int i = 0; i < 26; i++) {
//     float result = variables[i];
//     if(result != -1) {
//       cout << (char) (i + 65) << " " << result << endl;
//     }
//   }
// }

int main(int argc, char *argv[]) {
  string line;
  ifstream myfile("input.txt");
  if (myfile.is_open()){
    while (getline(myfile,line)){
      cout <<line<<endl;
      for(int i = 0; i < 26; i++) {
        variables[i] = -1;
      }
      char input[1024];
      strcpy(input, line.c_str());
      int j = 0;
      root = new Node(true, false, false, 0, -1);
      root->parent = NULL;
      makeTree(input);
      // postorder(root);
      balance(root);
      for(int i = 0; i < 26; i++) {
        float result = variables[i];
        if(result != -1) {
          cout << (char) (i + 65) << " " << result << endl;
        }
      }
    }
    myfile.close();
  }
  else cout << "Unable to open file";
  return 0;
}
