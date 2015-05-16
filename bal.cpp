#include "bal.h"
#include "stdio.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
using namespace std;

// Global Variables
float variables[26]; // array of variable weights, reset in main for each input
bool kill = false; // a killswitch to stop the program if no single solution
Node *root; // the root node of the tree, reset in main for each input

/*
  This function is responsible for making a tree that accurately represents the
  input, it steps through the input one character at a time and creates/assigns
  nodes as needed.
*/
bool makeTree(char input[]) {
  Node *current = root;
  int armCount = 1;
  string numAsString = ""; // string used to parse multiple digit numbers

  if (armCount > 50){
    cout << "Bad input, more than 50 arms is not allowed" << endl;
    kill = true;
    return false;
  }

  if (input[0] != '['){
    cout << "Bad input, the first character should be a \'[\'" << endl;
    kill = true;
    return false;
  }

  int i = 1; // iterator for the while loop below
  int numOpen = 1; // number of left bracket characters '['

  while(numOpen != 0) {
    int charVal = (int) input[i]; // current character's ascii value

    if (charVal > 47 && charVal < 58) { // number from 0 to 9 case
        numAsString += input[i];
    }
    else { //check if the number is done
        if(numAsString != "") {
          // number is done, write it out
          float val = stof(numAsString); // NOTE: C++11 function, get the number
          //assign a constant node as appropriate
          if (current->left == 0) {
            current->left = new Node(false, false, true, val + 1, -1);
            current->left->parent = current;
          }
          else if (current->right == 0) {
            current->right = new Node(false, false, false, val + 1, -1);
            current->right->parent = current;
          }
          numAsString = ""; // reset
        }
        else {
        }
    }

    // other non-number cases
    if (charVal > 64 && charVal < 91){ // capital letter from A-Z case
      // assign a variable node as appropriate
      if (current->left == 0) {
        current->left = new Node(false, true, true, 0, valToIndex(charVal));
        current->left->parent = current;
      }
      else if (current->right == 0) {
        current->right = new Node(false, true, false, 0, valToIndex(charVal));
        current->right->parent = current;
      }
    }
    else if (charVal == 91 && armCount <= 50) { // '[' case
      numOpen++;
      //assign new bar node to left or right as appropriate
        if (current->left == 0){
            current->left = new Node(true, false, true, 0, -1);
            armCount++;
            current->left->parent = current;
            current = current->left; // new current, step down the tree
         }
        else if (current->right == 0){
            current->right = new Node(true, false, false, 0, -1);
            armCount++;
            current->right->parent = current;
            current = current->right; // new current, step down the tree
        }
        else{
            cout << "Brackets do not match, please correct input" << endl;
            kill = true;
            return false;
        }
    }

    else if (charVal == 93) { // ] case
      numOpen--;
      current = current->parent; // step up the tree
    }

    // this check allows for spaces to be in the input
    else if (charVal != 32 && charVal != 0 && !(charVal > 47 && charVal < 58)) {
        cout << "Invalid character, please correct input" << endl;
        return false;
    }
    i++;
  }

}

/*
  This is the function that computes the correct values for the input variables.
  Eg, the actual algorithm and not just input handling. It is A LOT of branching
  and conditional statements, but we believe the variable and property names
  make it easy enough to read. The output for no/many solutions is done directly
  from this function. More on this in the README.
*/
void balance(Node *current) {
  if(kill) return;

  if(current->knowsSelf && current->leftKnown &&
    current->rightKnown && current->parent == 0) {
    // everything's known below the root, so we're done, success!
    kill = true;
  }
  else if(current->knowsSelf && current->leftKnown &&
  current->rightKnown && current->parent != 0) {
    //everthing below this node is known, but it has a parent, so update it
    if(current->isLeftChild) {
      current->parent->leftExplored = true;
      current->parent->leftKnown = true;
    }
    else {
      current->parent->rightExplored = true;
      current->parent->rightKnown = true;
    }
    current->parent->weight = current->weight * 2 + 1; // pass weight to parent
    current->parent->knowsSelf = true;
    balance(current->parent); // step up the tree
  }

  if(current->isBar) {
    if(current->knowsSelf) {
      if(!current->rightKnown) {
        if(current->right->isBar) {
          current->right->weight = (current->weight - 1) / 2; // pass weight
          current->right->knowsSelf = true;
          balance(current->right); // go right
        }
        else {
          balance(current->right); // go right
        }
      }
      else if(!current->leftKnown) {
        if(current->left->isBar) {
          current->left->weight = (current->weight - 1) / 2; // pass weight
          current->left->knowsSelf = true;
          balance(current->left); // go left
        }
        else {
          balance(current->left); // go left
        }
      }
      else { // leftKnown = rightKnown = false
        // there are 2+ variables below current, neiher of them are known, but
        // current knows itself, just go left and right will work itself out
        if(current->left->isBar) {
          current->left->weight = (current->weight - 1) / 2; // pass weight
        }
        balance(current->left); // go left
      }
    }
    else { // current does not know itself
      if(!current->leftExplored) {
        balance(current->left); // go left
      }
      else if(!current->rightExplored) {
        balance(current->right); // go right
      }
      else { // both left and right are explored, but no context yet
        if(current->parent != 0) { // has parent, so update it
          if(current->isLeftChild)current->parent->leftExplored = true;
          else current->parent->rightExplored = true;
          balance(current->parent); // step up
        }
        else { // this is the root, and there are all varible below it
          // MANY case
          cout << "MANY" << endl;
          kill = true;
        }
      }
    }
  }
  else { // current is not a bar, it is either a constant or a variable
    if(current->isVar) {
      if(current->parent->knowsSelf) {
        // write this variable's weight to the array
        variables[current->index] = (float)(((current->parent->weight) - 1) / 2) - 1;
        // update the parent accordingly
        if(current->isLeftChild) current->parent->leftKnown = true;
        else current->parent->rightKnown = true;
      }
      else { // current's parent is not known, update the parent accordingly
        if(current->isLeftChild) current->parent->leftExplored = true;
        else current->parent->rightExplored = true;
      }
      balance(current->parent);
    }
    else { // current is a constant, and we get some context
      if(current->parent->knowsSelf) {
        // compare current's weight to parent
        if(current->weight == (current->parent->weight - 1) / 2) {
          // everything checks out, update the parent
          if(current->isLeftChild) {
            current->parent->leftExplored = true;
            current->parent->leftKnown = true;
          }
          else {
            current->parent->rightExplored = true;
            current->parent->rightKnown = true;
          }
          balance(current->parent); // step up
        }
        else { // two already known values were compared and are incorrect
          // NONE case
          kill = true;
          cout << "NONE" << endl;
        }
      }
      else { // parent's weight not known and current is a constant
        // update parent accordingly
        if(current->isLeftChild) {
          current->parent->leftExplored = true;
          current->parent->leftKnown = true; // because this is a constant
        }
        else {
          current->parent->rightExplored = true;
          current->parent->rightKnown = true; // because this is a constant
        }
        current->parent->weight = 2 * current->weight + 1; // pass up weight
        current->parent->knowsSelf = true;
        balance(current->parent); // step up
      }
    }
  }
}

/*
  This is just a convenience function we wrote for the sake of clarity. It
  takes in a characters ascii value and kicks out the corresponding position in
  our array called variables.
*/
int valToIndex(int c) {
  return c - 65;
}

/*
  A tiny postorder traversal function we wrote in order to determine if the tree
  was building correctly.
*/
void postorder(Node *current) {
  if(current->left != NULL) postorder(current->left);
  if(current->right != NULL) postorder(current->right);
    if(current->isBar) cout << "-" << endl;
    else if(current->isVar) cout << (char) (current->index + 65) << endl;
    else cout << current->weight << endl;
}


int main(int argc, char *argv[]) {
  // reset variables array to -1
  for(int i = 0; i < 26; i++) {
    variables[i] = -1;
  }

  string line; // current line of the file
  char input[1024]; // char array buffer

  ifstream myfile("input.txt"); // open the file input.txt
  if (myfile.is_open()){
    while (getline(myfile,line)) { // while there is still a line in the file
      if (line == "" ); // do nothing
      else if (line == "[]") break; // end reading
      else { // we have actual input

        kill = false; // reset the killswitch
        strcpy(input, line.c_str()); // copy line into input[]
        root = new Node(true, false, false, 0, -1);
        root->parent = NULL; // make a new root

        makeTree(input);
        balance(root);

        // print the output
        for(int i = 0; i < 26; i++) {
          float result = variables[i];
          if(result != -1) {
            cout << (char) (i + 65) << " " << result << endl;
          }
          variables[i] = -1; // reset after printing
        }
      }
    }
    myfile.close();
  }
  else cout << "Unable to open file";
  return 0;
}
