# ListSerialize
Task:

Serialize and deserialize double-linked list with random access links 
pseudo:
  Node
  { 
    Node* next;
    Node* prev;
    Node* rand;
    string data;
  }
  List
  {
    Node* Head;
    Node* Tail;
    int Count;
  }
  
Complexity: less than n^2
