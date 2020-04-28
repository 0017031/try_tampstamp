#pragma once

class UnionFind {
public:
  explicit UnionFind(int N);      /// initialize it with N objects
  void add_union(int p, int q);   /// add connection between p and q
  bool is_connected(int p, int q) /// find wethere p and q are connected
}

