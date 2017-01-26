#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <string.h>
#include <limits>
#include <iostream>
#include <map>
#include <set>

class SuffixTree {
  private:
    class ComparatorChar {
      public:
        bool operator()(const char x, const char y) { return x<y; };
    };
    class Node {
      public:
        size_t start;
        size_t end;
        Node* suffixLink;
        std::map<char, Node*, ComparatorChar> children;
        size_t suffixIndex;

      public:
        Node(size_t aStart, size_t aSuffixIndex, size_t aEnd);
    };
  
  private:
    const char* T;
    Node* root;
    Node* active;
    Node* suffixLink;
    size_t activeEdge;
    size_t activeLength;
    size_t index;
    size_t remainder;

  private:
    SuffixTree::Node* newNode(size_t start, size_t aSuffixIndex, size_t end = std::numeric_limits<std::size_t>::max());
    void printNode(std::ostream& os, const Node* node, const size_t offset = 0, std::set<size_t> links = std::set<size_t>(), const bool last = false);

  public:
    SuffixTree(const char* aT);
    ~SuffixTree();

    void prolong(size_t j);
    void print(std::ostream& os);
};

#define getLength( node ) (std::min(node->end, this->index) - node->start)

SuffixTree::Node::Node(size_t aStart, size_t aSuffixIndex, size_t aEnd)
  : start(aStart), end(aEnd), suffixLink(0), suffixIndex(aSuffixIndex)
{
}

void SuffixTree::printNode(std::ostream& os, const Node* node, const size_t offset, std::set<size_t> links, const bool last)
{
  size_t length = 1;
  if (node != root) {
    //os << std::dec << std::setw(4) << std::setfill(' ') << (node->start) << ' ' << std::setw(4) << std::setfill(' ') << (node->suffixIndex);
    size_t i;
    for (i = 0; i < offset; ++i) {
      if (links.find(i) == links.end()) {
        os << " ";
      } else {
        os << u8"│";
      }
    }
    if (last) {
      os << u8"└";
      links.erase(i);
    } else {
      os << u8"├";
      links.insert(i);
    }
    for (size_t i = node->start; i < node->start + (length = getLength(node)); ++i) {
      os << T[i];
    }
    //os << '@';
    if (node == active) os << u8" ←";
    if (node->children.begin() == node->children.end()) {
      os << " (" << node->suffixIndex << ")";
    } else {
      os << " (" << node->suffixIndex << ")?";
    }
  } else {
    os << /*std::setw(10) <<*/ " ε";
  }
  //for (auto &kv : node->children) {
    //Node* child = kv.second;
  os << std::endl;
  for (auto i = node->children.begin(); i != node->children.end();) {
    Node* child = (*i).second;
    //char c = (*i).first;
    //os << "(" << c << ") ";
    printNode(os, child, offset + length, links, ++i == node->children.end());
    //os << std::endl;
  }

  //os << std::endl;
}

SuffixTree::SuffixTree(const char* aT)
  : T(aT), activeEdge(0), activeLength(0), index(0), remainder(0)
{
  root = active = this->newNode(-1, -1);
  suffixLink = NULL;
}

SuffixTree::~SuffixTree()
{
}

void SuffixTree::print(std::ostream& os)
{
  os << "(" << activeEdge << ", " << activeLength << ", " << remainder << ")" << std::endl;
  printNode(os, root);
}

SuffixTree::Node* SuffixTree::newNode(size_t start, size_t suffixIndex, size_t end) {
  return new SuffixTree::Node(start, suffixIndex, end);
}

void SuffixTree::prolong(size_t j) {
  for (;index<j;++index) {
    //Získám znak c z textu T na indexu index
    char c = T[index];
    //Resetuju suffix link
    suffixLink = NULL;
    //Inkrementuju remainder
    remainder++;                                                
    while (remainder > 0) {                                    
      //Resetuju aktivní hranu, pokud není nastavena aktivní délka
      if (!activeLength) activeEdge = index;                
      //Zkusím najít hranu vedoucí z aktivního uzlu pro daný symbol
      auto got = active->children.find(T[activeEdge]);
      //Hrana neexistuje
      if (got == active->children.end()) { 
        //Vytvořím nový uzel
        active->children.insert(std::make_pair(T[activeEdge], newNode(index, index - remainder + 1)));
        //Upravím suffix link
        if (suffixLink) suffixLink->suffixLink = active;
        suffixLink = active;
      //Hrana existuje
      } else {        
        //Vytáhnu si ji
        Node* next = got->second; 
        size_t length = getLength(next);                    
        if (activeLength >= length) {                      
          activeEdge += length; 
          activeLength -= length;
          active = next;
          continue;
        }
        if (T[next->start + activeLength] == c) {
          activeLength++;
          //Upravím suffix link
          if (suffixLink) suffixLink->suffixLink = active;
          suffixLink = active;
          break;
        }
        //Vytvořím uzel pro první část
        Node* split = newNode(next->start, next->start, next->start + activeLength);
        //Nahradím jím aktuální uzel
        got->second = split;
        //Z nového uzlu vytvořím větvení na nový znak a uložím si ho
        split->children.insert(std::make_pair(c, newNode(index, index - remainder + 1)));
        //Zároveň vezmu starý uzel a posunuho do rozdělovací pozice
        next->start += activeLength;
        //Nastavím ho tam
        split->children.insert(std::make_pair(T[next->start], next));
        //Upravím suffix link
        if (suffixLink) suffixLink->suffixLink = split;
        suffixLink = split;
      }
      remainder--;
      if (active == root && activeLength > 0) {
        activeLength--;
        activeEdge = index - remainder + 1;
      } else {
        active = active->suffixLink ? active->suffixLink : root;
      }
    }
    //std::cout << "(" << activeEdge << ", " << activeLength << ", " << remainder << ")" << std::endl;  
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2) return EXIT_FAILURE;
  int n = strlen(argv[1]);
  std::cout << std::dec << std::setw(4) << "String:";
  for (size_t i=0;i<n;++i) {
    std::cout << std::dec << std::setw(4) << std::setfill(' ') << argv[1][i];
  }
  std::cout << std::endl;
  std::cout << std::dec << std::setw(4) << "Length:" << std::setw(4) << std::setfill(' ') << n << std::endl;
  SuffixTree ST(argv[1]);
  ST.prolong(n);
  std::cout << std::endl;
  ST.print(std::cout);
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
