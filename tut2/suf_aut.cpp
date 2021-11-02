#include <vector>
#include <map>
#include <set>
#include <iostream>

//suffix automaton construction in O(n * log |Σ|) time
//Author : Michal Dvořák (dvora125)

struct state
{
  state(int len, int link) : m_len(len), m_link(link) { }
  int m_len;
  int m_link;
  std::map<char, int> m_transitions = {};
  bool m_final = false;

  std::string m_name; //just for printing
};

struct automaton
{
  automaton(const char* str)
  {
    construct(str);
  }

  void construct(const char* str)
  {
    //Start with the initial state.
    //Suffix link is undefined (-1) and length is 0 - the empty string.
    m_states.emplace_back(0, -1);
    m_states[0].m_name = "0";
    m_last = 0;
    for(; *str; str++)
      extend(*str);
    //Start from the last state and jump on suffix links,
    //These correspond to the suffixes of the original string - mark them as final states
    for(int p = m_last; p != -1; p = m_states[p].m_link)
      m_states[p].m_final = true;
  }

  void extend(char c)
  {
    //Add new state
    int cur = m_states.size();
    m_states.emplace_back(m_states[m_last].m_len + 1, -1);
    m_states[cur].m_name = std::to_string(m_states[cur].m_len);

    //Jump on suffix links while there is no transition on the actual character (c)
    int p = m_last;
    while(p != -1 && m_states[p].m_transitions.count(c) == 0)
    {
      m_states[p].m_transitions[c] = cur;
      p = m_states[p].m_link;
    }
    //If there was no such, the suffix link of the current state is the initial state
    if(p == -1)
      m_states[cur].m_link = 0;
    else
    {
      //Otherwise, set q = δ(p,c)
      int q = m_states[p].m_transitions[c];
      //If q.length is exactly p.length - 1, then sl(cur) = q
      if(m_states[p].m_len + 1 == m_states[q].m_len)
        m_states[cur].m_link = q;
      else
      {
        //Otherwise, split the state q into two, copy link and transition
        //and set length to be p.length + 1
        int clone = m_states.size();
        m_states.push_back(m_states[q]);
        m_states[clone].m_len = m_states[p].m_len + 1;
        m_states[clone].m_name = m_states[q].m_name + "'"; //add a dash to the name
        //Now jump on suffix links and while there is a transition on c
        //redirect the transition to the new state
        while(p != -1 && m_states[p].m_transitions[c] == q)
        {
          m_states[p].m_transitions[c] = clone;
          p = m_states[p].m_link;
        }
        //Direct suffix links from q and cur to clone state
        m_states[q].m_link = m_states[cur].m_link = clone;
      }
    }
    m_last = cur;
  }

  void print_states() const
  {
    std::cout << "Q = ";


    auto it = m_states.begin();
    std::cout << it->m_name;
    for(++it; it != m_states.end(); ++it)
      std::cout << ", " << it->m_name;
    std::cout << "}" << std::endl;
  }

  void print_symbols() const
  {
    std::set<char> symbols;
    for(const state& st: m_states)
      for(const auto& pr: st.m_transitions)
        symbols.insert(pr.first);
    std::cout << "Σ = {";
    auto it = symbols.begin();
    std::cout << *it;
    for(++it; it != symbols.end(); ++it)
      std::cout << ", " << *it;
    std::cout << "}" << std::endl;
  }

  void print_transitions() const
  {
    std::cout << "δ:" << std::endl;

    for(const state& st: m_states)
      for(const auto& pr: st.m_transitions)
        std::cout << "δ(" << st.m_name << ", " << pr.first << ") = " << m_states[pr.second].m_name << std::endl;
  }

  void print_initial_state() const
  {
    std::cout << "q_0 = 0" << std::endl;
  }

  void print_final_states() const
  {
    std::cout << "F = {";
    auto it = m_states.begin();
    std::cout << it->m_name; //initial state is always final
    for(++it; it != m_states.end(); ++it)
      if(it->m_final)
        std::cout << ", " << it->m_name;
    std::cout << "}" << std::endl;
  }

  void print() const
  {
    print_states();
    print_symbols();
    print_transitions();
    print_initial_state();
    print_final_states();
  }

  std::vector<state> m_states;
  int m_last = 0;
};


int main()
{
  std::string str;
  std::cout << "Enter string:" << std::endl;
  std::cin >> str;
  automaton a(str.c_str());
  a.print();

  return 0;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
