#pragma once
#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

namespace BPlusTree{
    const int max_size=4;
    struct Node{
        bool is_leaf;
        std::vector<int> keys;
        std::vector<std::string> values;
        std::vector<Node*> ch_nodes;
        Node *next=nullptr;
        Node(bool leaf) : is_leaf(leaf){
            keys.reserve(max_size);
            if(is_leaf) values.reserve(max_size);
            else ch_nodes.reserve(max_size+1);
        }
    };
    class BPlus_Tree{
    private:
        Node *m_Root;
        void print(Node *node);
        void splitnode(Node*parent,int index,Node*child);
        void insertNotFull(Node*node, int key,std::string value);

        void merge(Node *node,int index);
        void borrow_left(Node *node,int index);
        void borrow_right(Node *node,int index);
        void balancechild(Node *node,int index);
        void remove(Node *node,int key);
    public:
        BPlus_Tree():m_Root(nullptr){};

        //Insert
        void insert(int key,std::string value);

        //Search
        std::string search(int key);

        //Print
        void print_data();
        void print_tree();

        //Delete
        void remove(int key);
    };
}
