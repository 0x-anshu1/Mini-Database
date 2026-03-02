#include"BPlusTree.h"

using namespace std;
using namespace BPlusTree;

void BPlus_Tree::insert(int key,std::string value){
    if(m_Root==nullptr){
        m_Root=new Node(true);
        m_Root->keys.push_back(key);
        m_Root->values.push_back(value);
        return ;
    }
    if(m_Root->keys.size()==max_size){
        Node* NewNode=new Node(false);
        NewNode->ch_nodes.push_back(m_Root);
        splitnode(NewNode,0,m_Root);
        m_Root=NewNode;
    }
    insertNotFull(m_Root,key,value);
}

void BPlus_Tree::splitnode(Node*parent, int index,Node*child){
    Node *NewNode=new Node(child->is_leaf);
    int mid=max_size/2;

    if(NewNode->is_leaf){
        for(int i=mid;i<child->keys.size();++i){
            NewNode->keys.push_back(child->keys[i]);
            NewNode->values.push_back(child->values[i]);
        }

        child->keys.resize(mid);
        child->values.resize(mid);

        NewNode->next=child->next;
        child->next=NewNode;

        parent->keys.insert(parent->keys.begin()+index,NewNode->keys[0]);
    }else{
        int promoted_key=child->keys[mid];
        
        for(int i=mid+1;i<child->keys.size();++i){
            NewNode->keys.push_back(child->keys[i]);
        }
        for(int i=mid+1;i<child->ch_nodes.size();++i){
            NewNode->ch_nodes.push_back(child->ch_nodes[i]);
        }

        child->keys.resize(mid);
        child->ch_nodes.resize(mid+1);

        parent->keys.insert(parent->keys.begin()+index,promoted_key);
    }

    parent->ch_nodes.insert(parent->ch_nodes.begin()+index+1, NewNode);
}

void BPlus_Tree::insertNotFull(Node*node, int key, std::string value){
       Node *temp=node;
       if(temp->is_leaf){
           auto it_key=std::lower_bound(temp->keys.begin(),temp->keys.end(),key);
           auto it_value=temp->values.begin()+std::distance(temp->keys.begin(),it_key);

           if(it_key!=temp->keys.end() && *it_key==key){
               *it_value=value;
           }else{
               temp->keys.insert(it_key,key);
               temp->values.insert(it_value,value);

           }
       }else{
           auto it_key=std::lower_bound(temp->keys.begin(),temp->keys.end(),key);
           auto it_ch_node = temp->ch_nodes.begin() + std::distance(temp->keys.begin(),it_key);

           if((*it_ch_node)->keys.size()==max_size){
               splitnode(temp,std::distance(temp->keys.begin(),it_key),*it_ch_node);

               it_key = std::lower_bound(temp->keys.begin(),temp->keys.end(),key);
               it_ch_node = temp->ch_nodes.begin() + std::distance(temp->keys.begin(),it_key);
           }

           insertNotFull(*it_ch_node,key,value);
       }
}

std::string BPlus_Tree::search(int key){
    if(m_Root == nullptr){
        return "No tree found";
    }

    Node *temp=m_Root;
    std::string value;

    while(!temp->is_leaf){
        auto it_key = std::lower_bound(temp->keys.begin(),temp->keys.end(),key);
        auto it_ch_node = temp->ch_nodes.begin()+std::distance(temp->keys.begin(),it_key);

        temp=*it_ch_node;
    }

    auto it_key = std::find(temp->keys.begin(),temp->keys.end(),key);
    if(it_key != temp->keys.end()){
        auto it_value = temp->values.begin()+std::distance(temp->keys.begin(),it_key);
        value = *it_value;
    }else{
        value = "Key not found";
    }

    return value;
}

void BPlus_Tree::print_data(){
    //auto it_ch_node = m_Root->ch_nodes.begin();
    if(m_Root==nullptr){
        std::cout<<"No records to display"<<std::endl;
        return;
    }

    Node *temp=m_Root;

    while(!temp->is_leaf){
        temp=*(temp->ch_nodes.begin());
    }

    print(temp);
}

void BPlus_Tree::print(Node *node){
    for(int i=0;i<node->keys.size();++i){
        std::cout<<node->keys[i]<<" "<<node->values[i]<<std::endl;
    }
    
    if(node->next){
        print(node->next);
    }
}

void BPlus_Tree::print_tree(){
    if(m_Root==nullptr){
        std::cout<<"No records to display"<<std::endl;
        return ;
    }

    std::queue<Node*> que;
    que.push(m_Root);

    while(!que.empty()){
        int n=que.size();

         for(int i=0;i<n;++i){
             Node *temp=que.front();
             que.pop();
             std::cout<<"[ ";
             for(int key:temp->keys){
                 std::cout<<key<<" ";
             }
             std::cout<<" ]";
             if(!temp->ch_nodes.empty()){
                 for(Node *node:temp->ch_nodes){
                     que.push(node);
                 }
             }
         }
         std::cout<<"\n";
    }
}

void BPlus_Tree::remove(Node *node,int key){
    Node *temp = node;
    if(temp->is_leaf){
        auto it_key=std::find(temp->keys.begin(),temp->keys.end(),key);
        if(it_key==temp->keys.end()){
            std::cout<<"Key not found"<<std::endl;
            return ;
        }

        auto it_value=temp->values.begin()+std::distance(temp->keys.begin(),it_key);
        temp->keys.erase(it_key);
        temp->values.erase(it_value);

    }else{
        int idx = std::distance(temp->keys.begin(),std::lower_bound(temp->keys.begin(),temp->keys.end(),key));
        Node *child = node->ch_nodes[idx];

        if(child->keys.size() == (max_size+1)/2-1){
            balancechild(temp,idx);

            idx = std::distance(temp->keys.begin(),std::lower_bound(temp->keys.begin(),temp->keys.end(),key));
            child = temp->ch_nodes[idx];
        }
        remove(child,key);
    }
}

void BPlus_Tree::remove(int key){
    if(m_Root==nullptr){
        std::cout<<"No tree found"<<std::endl;
        return ;
    }
    remove(m_Root,key);

    if(!m_Root->is_leaf && m_Root->keys.empty()){
        Node* old_root = m_Root;
        m_Root = m_Root->ch_nodes[0];
        delete old_root;
    }
}

void BPlus_Tree::balancechild(Node* node,int index){
    Node *child=node->ch_nodes[index];
    int min_key=(max_size+1)/2-1;

    if(index>0 && node->ch_nodes[index-1]->keys.size()>min_key)borrow_left(node,index);
    else if(index<node->ch_nodes.size()-1  && node->ch_nodes[index+1]->keys.size()>min_key)borrow_right(node,index);
    else merge(node,index);
}

void BPlus_Tree::borrow_left(Node *node,int index){
    Node *child = node->ch_nodes[index];
    Node *left_child = node->ch_nodes[index-1];

    if(child->is_leaf){
        child->keys.insert(child->keys.begin(),left_child->keys.back());
        child->values.insert(child->values.begin(),left_child->values.back());

        left_child->keys.pop_back();
        left_child->values.pop_back();

        node->keys[index-1]=child->keys[0];
    }else{
        child->keys.insert(child->keys.begin(), node->keys[index-1]);
        node->keys[index-1] = left_child->keys.back();
        child->ch_nodes.insert(child->ch_nodes.begin(), left_child->ch_nodes.back());

        left_child->keys.pop_back();
        left_child->ch_nodes.pop_back();
    }
}

void BPlus_Tree::borrow_right(Node *node,int index){
    Node *child = node->ch_nodes[index];
    Node *right_child = node->ch_nodes[index+1];

    if(child->is_leaf){
        child->keys.push_back(right_child->keys.front());
        child->values.push_back( right_child->values.front());

        right_child->keys.erase(right_child->keys.begin());
        right_child->values.erase(right_child->values.begin());

        node->keys[index+1]=child->keys.back();
    }else{
        child->keys.push_back(right_child->keys.front());
        node->keys[index-1] = right_child->keys[0];
        child->ch_nodes.push_back( right_child->ch_nodes[0]);

        right_child->keys.erase(right_child->keys.begin());
        right_child->ch_nodes.erase(right_child->ch_nodes.begin());
    }
}

void BPlus_Tree::merge(Node *node, int index){
    Node *child = node->ch_nodes[index];
    Node *right_child = node->ch_nodes[index+1];

    if(child->is_leaf){
        for(size_t i=0 ; i<right_child->keys.size(); ++i){
            child->keys.push_back(right_child->keys[i]);
            child->values.push_back(right_child->values[i]);
        }
        
        child->next = right_child->next;
    }else{
        child->keys.push_back(node->keys[index]);

        for(int key:right_child->keys) child->keys.push_back(key);
        for(Node* ch:right_child->ch_nodes) child->ch_nodes.push_back(ch);
    }

    node->keys.erase(node->keys.begin() + index);
    node->ch_nodes.erase(node->ch_nodes.begin()+index+1);
}
