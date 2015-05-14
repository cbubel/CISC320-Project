// Test case:
// [ [ A B ] 5 ]

int variables[26];

struct Node{
    Node * left;
    Node * right;
    Node * parent;

    bool isBar;
    bool isVar;

    // search properties
    bool leftExplored = false;
    bool rightExplored = false;
    bool leftKnown = false;
    bool rightKnown = false;
    bool isKnown = false;

    bool isLeftChild;
    int weight; // display weight +1 to account for pan weight
    char name;
}

int balance(Node current) {
  if(current.isKnown && current.leftKnown &&
    current.rightKnown && current.parent == NULL) {
    return; // we're done
  }
  else if(current.isKnown && current.leftKnown &&
  current.rightKnown && current.parent != NULL) {
    if(current.isLeftChild) {
      current.parent.leftKnown = true;
    }
    else {
      current.parent.rightKnown = true;
    }
    balance(current.parent);
  }

  if(current.isBar) {
    if(current.isKnown) {
      if(current.leftKnown) {
        // go right
        if(current.right.isBar) {
          current.right.weight = (current.weight - 1) / 2;
          current.right.isKnown = true;
          balance(current.right);
        }
        else {
          balance(current.right);
        }
      }
      else if(current.rightKnown) {
        // go left
        if(current.left.isBar) {
          current.left.weight = (current.weight - 1) / 2;
          current.left.isKnown = true;
          balance(current.left);
        }
        else {
          balance(current.left);
        }
      }
      else { // not "root"
        // Only need to do left because right will automatically happen after
        if(current.left.isBar) {
          current.left.weight = (current.weight - 1) / 2;
        }
        balance(current.left);
      }
    }
    else {
      if(!current.leftExplored) {
        balance(current.left);
      }
      else if(!current.rightExplored) {
        balance(current.right);
      }
      else {
        if(current.parent != NULL) {
          if(current.isLeftChild) {
            current.parent.leftExplored = true;
          }
          else {
            current.parent.rightExplored = true;
          }
          balance(current.parent);
        }
        else {
          return 1; // MANY i think
        }
      }
    }
  }
  else { // Plate
    if(current.isVar) {
      if(current.parent.isKnown) {
        // write it
        variables[current.name.charToIndex] = (current.parent.weight - 1) / 2;

        if(current.isLeftChild) {
          current.parent.leftKnown = true;
        }
        else {
          current.parent.rightKnown = true;
        }
      }
      else {
        if(current.isLeftChild) {
          current.parent.leftExplored = true;
        }
        else {
          current.parent.rightExplored = true;
        }
      }
      balance(current.parent);
    }
    else { // Constant
      if(current.parent.isKnown) {
        //compare
        if(current.weight == (curent.parent.weight - 1) / 2) {
          // good
          if(current.isLeftChild) {
            current.parent.leftExplored = true;
            current.parent.leftKnown = true;
          }
          else {
            current.parent.rightExplored = true;
            current.parent.rightKnown = true;
          }
          balance(current.parent);
        }
        else {
          // NONE
          return -1;
        }
      }
      else { // parent weight not known
        if(current.isLeftChild) {
          current.parent.leftExplored = true;
          current.parent.leftKnown = true;
        }
        else {
          current.parent.rightExplored = true;
          current.parent.rightKnown = true;
        }
        current.parent.weight = 2 * current.weight + 1;
        current.parent.isKnown = true;
        balance(current.parent);
      }
    }
  }
}

int main(int argc, char *argv[]) {

}
