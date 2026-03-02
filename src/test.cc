#include "BPlusTree.h"
#include<cstring>

using namespace BPlusTree;

int main(){
    BPlus_Tree Tree;
    int ch=1;
    while(ch!=-1){
        std::cout<<"******Options******"<<std::endl;
        std::cout<<"1. insert"<<std::endl;
        std::cout<<"2. search"<<std::endl;
        std::cout<<"3. print data"<<std::endl;
        std::cout<<"4. print tree"<<std::endl;
        std::cout<<"5. remove"<<std::endl;
        std::cin>>ch;

        switch(ch){
            case 1:{
                int key;
                std::string value;
                std::cin>>key;
                std::getline(std::cin,value);
                std::getline(std::cin,value);
                Tree.insert(key,value);
                break;
                   }
            case 2:{
                int key;
                std::cin>>key;
                std::string value=Tree.search(key);
                std::cout<<value<<std::endl;
                break;
                   }
            case 3:{
                Tree.print_data();
                break;
                   }
            case 4:{
                Tree.print_tree();
                break;
                   }
            case 5:{
                int key;
                std::cin>>key;
                Tree.remove(key);
                break;
                   }
            default:
                break;
        }
        if(ch == -1)break;
    }

    return 0;
}
