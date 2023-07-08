/********************************************
** File:   squeue.cpp
** Project: CMSC 341 Project 4, Spring 2022
** Author:  Syed Husain
** Date:    4/16/22
** E-mail:  ax18210@umbc.edu
** Desc:  This file makes the Squeue object to run by mytest.cpp
** Course/Section : CMSC 341 
**/

#include "squeue.h"

SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType){
    // Preconditions: None
    // Postconditions: Initializes variables
    
    m_heap = nullptr;          
    m_size = 0;    
    m_priorFunc = priFn;
    m_heapType = heapType;

}

SQueue::~SQueue(){
    // Preconditions: None
    // Postconditions: The Squeue tree is deallocated by deleting all the nodes and it childrens

    //checks if the tree is no empty and then calls clear to dellocate memory
    if(m_heap != nullptr && m_size != 0){
        clear();
    }

}

SQueue::SQueue(const SQueue& rhs){
    // Preconditions: None
    // Postconditions: Create a copy squeue tree of the passed Squeue tree

    //Initializes the variable by copying host's member
    m_size = rhs.m_size;
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.m_priorFunc;
    m_heap = rhs.m_heap;

    //Calls the heleper function and copy all it root and childrens of the host
    m_heap = CopySQueue(rhs.m_heap);

}

SQueue& SQueue::operator=(const SQueue& rhs){
    // Preconditions: None
    // Postconditions: Create a copy squeue tree of the passed Squeue tree

    //check for self assignement
    if(this != &rhs){

        //Fees all dynamically allocated data members of the hos
        this->clear();
        
        //Initializes the variable by copying host's member
        m_size = rhs.m_size;
        m_heapType = rhs.m_heapType;
        m_priorFunc = rhs.m_priorFunc;
        m_heap = rhs.m_heap;

        //Calls the heleper function and copy all it root and childrens of the host
        m_heap = CopySQueue(rhs.m_heap);
    }

    //if same host then return it
    return *this;

}


Node* SQueue::CopySQueue(Node * root){
    // Preconditions: None
    // Postconditions: Create a copy squeue tree of the passed Squeue tree

    //return null when tree is empty
    if(root == nullptr){
        return nullptr;
    }

    //create a newNode pointer
    Post pos;
    Node * newNode = new Node(pos);

    //Traverse over the past heap and copies all it roots and childrens
    newNode->m_post = root->m_post;
    newNode->m_left = CopySQueue(root->m_left);
    newNode->m_right = CopySQueue(root->m_right);

    //return teh copied skew heap
    return newNode;
}

void SQueue::insertPost(const Post& post) {
    // Preconditions: None
    // Postconditions: Insert the post in the skew heap tree

    //checks if the post past is invalid and return
    if(post.m_postID > MAXPOSTID || post.m_postID < MINPOSTID){
        return;
    }

    //check if tree is empty and sets the root
    if(m_heap == nullptr){
        Post pos;
        m_heap = new Node(pos);
        m_heap->m_post = post;
        m_heap->m_left = nullptr;
        m_heap->m_right = nullptr;
        
    }else{
        //create new skew heap with just one post and merges it with the skew heap
        Post pos;
        Node * aPost = nullptr;
        aPost = new Node(pos);
        aPost->m_post = post;
        aPost->m_left = nullptr;
        aPost->m_right = nullptr;
        m_heap = mergeHelp(m_heap, aPost);

    }

    //increment the size by 1
    m_size += 1;

}

Post SQueue::getNextPost() {
    // Preconditions: None
    // Postconditions: Remove the root node and return the highest priority post from the queue. 

    //check if the heap is empty and return out of range error
    if(m_heap == nullptr){
        throw out_of_range("SQueue::getNextPost() Attempting to remove from an empty object!!");
    }

    //create a point to the heap's root
    Node * temp = m_heap;

    //merges the left and right of the root
    m_heap = mergeHelp(temp->m_left, temp->m_right);

    //set the post
    Post nextPost = temp->getPost();

    //deletes the root and decrements the size by 1
    delete temp;
    m_size -= 1;

    //return the priority post
    return nextPost;

}

void SQueue::mergeWithQueue(SQueue& rhs) {
    // Preconditions: None
    // Postconditions: Merges the host queue with the rhs

    //throw domain_error when the user attempts to merge queues with different priority

    //if(rhs.m_heapType != m_heapType){
    if(rhs.m_priorFunc != m_priorFunc){
        throw domain_error("mergeWithQueue: Attemption to merge queue with different priority");
    }

    //checks against self assignment
    if(this != &rhs){  
        //Set a temp and copy the host tree
        Node *temp = CopySQueue(rhs.m_heap); 

        //increment the size
        m_size = m_size + rhs.m_size;

        //calls the merge function and merge heap then empties the host rhs
        m_heap = mergeHelp(temp, m_heap);
        rhs.clear();
    }


}

Node* SQueue::mergeHelp(Node * aPost1, Node * aPost2){
    // Preconditions: None
    // Postconditions: Merges the host queue with the rhs
     
    //if aPost1 is null return aPost2
    if (aPost1 == nullptr && aPost2 != nullptr)
        return aPost2;

    //if aPost2 is null return aPost1 else return null
    if (aPost2 == nullptr && aPost1 != nullptr)
        return aPost1;

    if (aPost2 == nullptr && aPost1 == nullptr)
        return nullptr;

    //if heap is a max skew then check is host node is less than current heap node and swaps
    if(m_heapType == MAXHEAP){
        if(m_priorFunc(aPost1->m_post) < m_priorFunc(aPost2->m_post))
            swap(aPost1, aPost2);
    }

    //if heap is a min skew then check is host node is less than current heap node and swaps
    if(m_heapType == MINHEAP){
        if(m_priorFunc(aPost1->m_post) > m_priorFunc(aPost2->m_post))
            swap(aPost1, aPost2);  
    }

    //swaps the childrens of the root
    swap(aPost1->m_right, aPost1->m_left);

    //Traverse through the host left side of the heap
    aPost1->m_left = mergeHelp(aPost1->m_left, aPost2);

    //return the host node
    return aPost1;
}

void SQueue::clear() {
    // Preconditions: Require a Skew Heap
    // Postconditions: The Skew heap is deallocated by deleting all the nodes and it childrens

    //calls helper function with m_heap to dellocate memory
    clearHelp(m_heap);

}

void SQueue::clearHelp(Node* aPost){
    // Preconditions: Require a Skew Heap
    // Postconditions: The Skew heap is deallocated by deleting all the nodes and it childrens
    
    //if heap is empty returns
    if (aPost == nullptr){
        return;
    }

    //else traverse throught skew heap and delete each iternal node and its child
    clearHelp(aPost->m_left);
    clearHelp(aPost->m_right);
    delete(aPost);

    //reintializes squeue members after deleting
    aPost = nullptr;
    m_size = 0;

}

int SQueue::numPosts() const{
    // Preconditions: None
    // Postconditions: return the m_size of the queue

    return m_size;

}


void SQueue::printPostsQueue() const {
    // Preconditions: Requires a skew heap
    // Postconditions: Prints the contents of the queue using preorder traversal

    //checks if the size is zero and prints it empty
    if (m_size == 0) {
        cout << "Empty skew heap.\n" ;
    }else{
        //calls the helper fucntion with m_heap and displaces all it internal node and its children
        printPostHelp(m_heap);
        cout << endl;
    }
  
}

void SQueue::printPostHelp(Node *pos) const{
    // Preconditions: Requires a skew heap
    // Postconditions:  Prints the contents of the queue using preorder traversal

    //check if skew heap isn't empty
    if ( pos != nullptr ) {

        //print priority post and uses operator<< to output Post's data
        cout << "[" << m_priorFunc(pos->m_post) << "] " << pos->m_post << endl;

        //traverse through the right and left subtree of the skew heap
        printPostHelp(pos->m_left);
        printPostHelp(pos->m_right);
    }

}

ostream& operator<<(ostream& sout, const Post& post) {
    // Preconditions: None
    // Postconditions:  Prints the contents of the queue 

    sout << "Post ID: " << post.getPostID() 
    << ", number of likes: " << post.getNumLikes()
    << ", interest level: " << post.getInterestLevel() 
    << ", connection level: " << post.getConnectLevel()
    << ", post time: " << post.getPostTime();
    
    return sout;
}

prifn_t SQueue::getPriorityFn() const {
    // Preconditions: None
    // Postconditions:  return the m_priorFunc

    return m_priorFunc;
  
}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    // Preconditions: None
    // Postconditions: Sets a new priority function and rebuilds the heap

    //set the squeue's member variable
    m_priorFunc = priFn;
    m_heapType = heapType;

    //check the heaptype and calls the helper function to rebuild the heap accordingly
    if(heapType == MAXHEAP){
        maxHeap(m_heap);
        
    }

    if(heapType == MINHEAP){
        minHeap(m_heap);
    }
                      
}


void SQueue::maxHeap(Node*root){
    // Preconditions: None
    // Postconditions: Rebuilds the Min heap into a Max heap

    //returns after the end of skew heap
    if(root == nullptr){
        return;
    }

    //traverse throught the right and left subtree
    maxHeap(root->m_left);
    maxHeap(root->m_right);

    //checks if any of the left child of the node is greater than its parent root
    if(root->m_left != nullptr && m_priorFunc(root->m_left->m_post) > m_priorFunc(root->m_post)){

        //swap the nodes if its greater
        swap(root->m_post,root->m_left->m_post);

        //checks max heap after swap
        maxHeap(root); 
    }

    //checks if any of the right child of the node is greater than its parent root
    if(root->m_right != nullptr && m_priorFunc(root->m_right->m_post) > m_priorFunc(root->m_post)){

        //swap the nodes if its greater
        swap(root->m_post,root->m_right->m_post);

        //checks max heap after swap
        maxHeap(root); 
    }


}

void SQueue::minHeap(Node*root){
    // Preconditions: None
    // Postconditions: Rebuilds the Max heap into a Min heap

    //returns after the end of skew heap
    if(root == nullptr){
        return;
    }

    //traverse throught the right and left subtree
    minHeap(root->m_left);
    minHeap(root->m_right);

    //checks if any of the left child of the node is less than its parent root
    if(root->m_left != NULL && m_priorFunc(root->m_left->m_post) < m_priorFunc(root->m_post)){

        //swaps node
        swap(root->m_post,root->m_left->m_post);

        //check min head after swap
        minHeap(root); 
    }

    //checks if any of the right child of the node is less than its parent root
    if(root->m_right != NULL && m_priorFunc(root->m_right->m_post) < m_priorFunc(root->m_post)){

        //swaps node
        swap(root->m_post,root->m_right->m_post);

        //check min head after swap
        minHeap(root); 
    }


}

HEAPTYPE SQueue::getHeapType() const{
    // Preconditions: None
    // Postconditions: returns the heap type

    return m_heapType;
}

void SQueue::dump() const{
    // Preconditions: Requires a skew heap
    // Postconditions: Prints the contents of the queue using preorder traversal

    //checks if the size is zero and prints it empty
    if (m_size == 0) {
        cout << "Empty skew heap.\n" ;
    }else{
        //calls the helper fucntion with m_heap and displaces all it internal node and its children
        dump(m_heap);
        cout << endl;
        system("open file:///Users/syedhusain/Downloads/UMBC/Spring_2022/Cmsc%20341/cmsc341_projects/proj3/tree_viewer.html");
    }

    

}

void SQueue::dump(Node *pos) const {
    // Preconditions: Requires a skew heap
    // Postconditions: Prints the contents of the queue using preorder traversal

    //check if skew heap isn't empty
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left); //first visit the left child

        //print the id and priority of the node
        cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();

        dump(pos->m_right); //then visit the right child
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Node& node) {
    // Preconditions: None
    // Postconditions: Print the node's post

    sout << node.getPost();
    return sout;
}
