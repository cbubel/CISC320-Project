
struct Node{
  Node * parent;
  Node * left;
  Node * right;

  bool isBar;
  bool isVar;
  bool isKnown;

  // search properties
  bool leftExplored;
  bool leftKnown;
  bool rightExplored;
  bool rightKnown;

  bool isLeftChild;
  float weight; // display weight +1 to account for pan weight
  int index;

  Node(bool b, bool v, bool lc, int w, int i) {
    // parent = &p;
    left = 0;
    right = 0;
    isBar = b;
    isVar = v;
    isKnown = false;
    leftExplored = false;
    leftKnown = false;
    rightExplored = false;
    rightKnown = false;
    isLeftChild = lc;
    weight = w;
    index = i;
  }

};

int balance(Node current);

int valToIndex(int c);
