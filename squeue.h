/********************************************
** File:    squeue.h
** Project: CMSC 341 Project 4, Spring 2022
** Author:  Syed Husain
** Date:    4/16/22
** E-mail:  ax18210@umbc.edu
** Desc:  Header file for squeue object made in squeue.cpp
** Course/Section : CMSC 341 
**/

#ifndef SQUEUE_H
#define SQUEUE_H
#include <stdexcept>
#include <iostream>
#include <string>

using namespace std;
class Grader;   // forward declaration (for grading purposes)
class Tester;   // forward declaration
class SQueue;   // forward declaration
#define DEFAULTPOSTID 100000
const int MINPOSTID = 100001;//minimum post ID
const int MAXPOSTID = 999999;//maximum post ID
const int MINLIKES = 0;//lowest priority
const int MAXLIKES = 500;//highest priority
const int MININTERESTLEVEL = 1;//lowest priority
const int MAXINTERESTLEVEL = 10;//highest priority
const int MINCONLEVEL = 1;//highest priority
const int MAXCONLEVEL = 5;//lowest priority
const int MINTIME = 1;//highest priority
const int MAXTIME = 50;//lowest priority
enum HEAPTYPE {MINHEAP, MAXHEAP};

class Post{
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    friend class SQueue;
    Post(){
        m_postID = DEFAULTPOSTID;m_likes = MINLIKES;
        m_connectLevel = MAXCONLEVEL;m_postTime = MAXTIME;
        m_interestLevel = MININTERESTLEVEL;
    }
    Post(int ID, int likes, int connectLevel, int postTime, int interestLevel){
        if (ID < MINPOSTID || ID > MAXPOSTID) m_postID = DEFAULTPOSTID;
        else m_postID = ID;
        if (likes < MINLIKES || likes > MAXLIKES) m_likes = MINLIKES;
        else m_likes = likes;
        if (connectLevel < MINCONLEVEL || connectLevel > MAXCONLEVEL) m_connectLevel = MAXCONLEVEL;
        else m_connectLevel = connectLevel;
        if (postTime < MINTIME || postTime > MAXTIME) m_postTime = MAXTIME;
        else m_postTime = postTime;
        if (interestLevel < MININTERESTLEVEL || interestLevel > MAXINTERESTLEVEL) m_interestLevel = MININTERESTLEVEL;
        else m_interestLevel = interestLevel;
    }
    int getPostID() const {return m_postID;}
    int getNumLikes() const {return m_likes;}
    int getConnectLevel() const {return m_connectLevel;}
    int getPostTime() const {return m_postTime;}
    int getInterestLevel() const {return m_interestLevel;}
    private:
    int m_postID;           // every post is identified by a unique ID
    int m_likes;            // 0-500, constant variables defined for this
    // m_connectLevel shows how close the relationship is between the poster and the connection who sees the posts
    // 1 indicates that the poster and the connection are directly related
    // 2 indicates that the poster is related to the connection through another person
    int m_connectLevel;     // 1-5, constant variables defined for this
    // m_postTime is an integer indicating in what order messages are posted
    // it can be between 1 and 50, at any time only 50 posts will be processed for a user
    // therefore this value cannot go beyond 50
    // a value of 1 means this was the first message posted
    // a value of 50 means this was the 50th message posted
    int m_postTime;         // 1-50, constant variables defined for this
    // m_interestLevel indicates how interesting is the post for the connection whi sees it
    // a value of 1 expresses the lowest level of interest
    // a value of 10 expresses the highest level of interest 
    int m_interestLevel;    // 1-10, constant variables defined for this
};

class Node {
  public:
  friend class Grader; // for grading purposes
  friend class Tester; // for testing purposes
  friend class SQueue;
  Node(Post post) {  
    m_post = post;
    m_right = nullptr;
    m_left = nullptr;
  }
  Post getPost() const {return m_post;}
  private:
  Post m_post;      // order information
  Node * m_right;   // right child
  Node * m_left;    // left child
};

// Overloaded insertion operators for Post and Node
ostream& operator<<(ostream& sout, const Post& post);
ostream& operator<<(ostream& sout, const Node& node);

// Priority function pointer type
typedef int (*prifn_t)(const Post&);

class SQueue{
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    
    SQueue(prifn_t priFn, HEAPTYPE heapType);
    ~SQueue();
    SQueue(const SQueue& rhs);
    SQueue& operator=(const SQueue& rhs);
    void insertPost(const Post& post);
    Post getNextPost(); // Return the highest priority post
    void mergeWithQueue(SQueue& rhs);
    void clear();
    int numPosts() const; // Return number of posts in queue
    void printPostsQueue() const; // Print the queue using preorder traversal
    prifn_t getPriorityFn() const;
    // Set a new priority function. Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    void dump() const; // For debugging purposes

    private:
    Node * m_heap;          // Pointer to root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // either a MINHEAP or a MAXHEAP

    void dump(Node *pos) const; // helper function for dump

    /******************************************
     * Private function declarations go here! *
    ******************************************/

    Node* insertHelp(Node * aPost, const Post& post); //helper function for insertPost()
    void clearHelp(Node *pos); //helper function for clear()
    void printPostHelp(Node *pos) const; //helper function for printPostsQueue()

    void maxHeap(Node*root); //helper function for setPriorityFn() rebuilding maxheap
    void minHeap(Node*root); //helper function for setPriorityFn() rebuilding minheap

    Node* CopySQueue(Node * root); //helper function for copy constructor()
    Node* mergeHelp(Node * h1, Node * h2); //helper function for mergeWithQueue()

};
#endif
