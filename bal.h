struct Node{
  Node * parent; // a reference to this node's parent
  Node * left; // a reference to this node's left child
  Node * right; // a reference to this node's right child

  bool isBar; // true iff the node is a bar (arm)
  bool isVar; // true iff the node is a variable
  bool knowsSelf; // true iff the node knows it's own weight

  // search properties
  bool leftExplored; // true iff all nodes to the left have been visited
  bool leftKnown; // true iff all nodes to the left know their weights
  bool rightExplored; // true iff all nodes to the right have been visited
  bool rightKnown; // true iff all nodes to the left know their weights

  bool isLeftChild; // true iff this node is a left child
  float weight; // display weight +1 to account for pan weight
  int index; // the node's index into our char array

  Node(bool b, bool v, bool lc, int w, int i) {
    left = 0;
    right = 0;
    isBar = b;
    isVar = v;
    knowsSelf = false;
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
