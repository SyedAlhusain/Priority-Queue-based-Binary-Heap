/********************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 3, Spring 2022
** Author:  Syed Husain
** Date:    3/26/22
** E-mail:  ax18210@umbc.edu
** Desc:  This file test the Squeue object made in squeue.cpp
** Course/Section : CMSC 341 
**/

#include "squeue.cpp"
#include <random>

// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post &post);// works with a MAXHEAP
int priorityFn2(const Post &post);// works with a MINHEAP

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:

    //testing heap property
    bool MinHeapProperty(Node * root, int i, int n, prifn_t m_priorFunc); //function for testing min heap property
    bool MaxHeapProperty(Node * root, int i, int n, prifn_t m_priorFunc); //function for testing max heap property

    //test for testing all the test cases of squeue object 
    bool SqueueAllTest();

    //test insertion
    bool MinHeapInsertNormal();
    bool MaxHeapInsertNormal();

    //test removal
    bool MinHeapRemoveNormal();
    bool MaxHeapRemoveNormal();
    bool RemoveErrorCase();
    bool RemoveEdgeCase();

    //test rebuild
    bool MaxPriorityRebuild();
    bool MinPriorityRebuild();
    
    //test copy constructor
    bool CopyConstNormalCase();
    bool CopyConstEdgeCase();

    //test assignment operator
    bool AssignConstNormalCase();
    bool AssignConstEdgeCase();

    //test merge
    bool MergeEdgeCase();
    bool MergeErrorCase();

    //testing time insertion
    bool InsertTime();
    bool RemoveTime();

    bool final();

};

int priorityFn1(const Post &post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post &post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}

bool Tester::MinHeapProperty(Node * root, int i, int n, prifn_t m_priorFunc){
    // Preconditions: Requires a skew heap
    // Postconditions: Checks if the heap is a valid Min heap

    //if no child of the parent is less than it parent root return true
    if (root == nullptr) {
        return true;
    }

    //checks if any of the left and child right are less than their parent and return false if they are
    if ( ( root->m_left && m_priorFunc(root->m_left->m_post) < m_priorFunc(root->m_post)) || ( root->m_right && m_priorFunc(root->m_right->m_post) < m_priorFunc(root->m_post)) ) {
        return false;
    }

    //Recursviely traverse through the sub trees of the skew and check each parent root
    return MinHeapProperty(root->m_left, 2 * i + 1, n, m_priorFunc) && MinHeapProperty(root->m_right, 2 * i + 2, n, m_priorFunc);
}


bool Tester::MaxHeapProperty(Node * root, int i, int n, prifn_t m_priorFunc){
    // Preconditions: Requires a skew heap
    // Postconditions: Checks if the heap is a valid Max heap

    //if no child of the parent is greater than it parent root return true
    if (root == nullptr) {
        return true;
    }

    //checks if any of the left and child right are greater than their parent and return false if they are
    if ( ( root->m_left && m_priorFunc(root->m_left->m_post) > m_priorFunc(root->m_post)) || ( root->m_right && m_priorFunc(root->m_right->m_post) > m_priorFunc(root->m_post)) ) {
        return false;
    }

    //Recursviely traverse through the sub trees of the skew and check each parent root
    return MaxHeapProperty(root->m_left, 2 * i + 1, n, m_priorFunc ) && MaxHeapProperty(root->m_right, 2 * i + 2, n, m_priorFunc);
}


int main(){

    Tester tester;

    if(tester.SqueueAllTest() == true)
        cout << "\nThe Program worked successfully :) All the tests were passed!!" << endl;
    else
        cout << "\nThe Program failed :( All the tests did not passed succesfully !!" << endl;

    tester.final();

}

bool Tester::SqueueAllTest(){
    // Preconditions: None
    // Postconditions: Tests all the function to see if the program runs succesfully

    Tester tester;

    cout << "\nTesting insertPost()(...), Min Heap Normal Case:\n\n";
    if (tester.MinHeapInsertNormal() == true){
        cout << "\tNormal Case of Min heap insertPost() passed!\n";
    }else{
        cout << "\tNormal Case of Min heap insertPost() failed!\n";
        return false;
    }

    cout << "\nTesting insertPost()(...), Max Heap Normal Case:\n\n";
    if (tester.MaxHeapInsertNormal() == true){
        cout << "\tNormal Case of Max heap insertPost() passed!\n";
    }else{
        cout << "\tNormal Case of Max heap insertPost() failed!\n";
        return false;
    }

    cout << "\nTesting getNextPost()(...), Min Heap Normal Case:\n\n";
    if (tester.MinHeapRemoveNormal() == true){
        cout << "\tNormal Case of Min heap getNextPost() passed!\n";
    }else{
        cout << "\tNormal Case of Min heap getNextPost() failed!\n";
        return false;
    }

    cout << "\nTesting getNextPost()(...), Max Heap Normal Case:\n\n";
    if (tester.MaxHeapRemoveNormal() == true){
        cout << "\tNormal Case of Max heap getNextPost() passed!\n";
    }else{
        cout << "\tNormal Case of Max heap getNextPost() failed!\n";
        return false;
    }

    cout << "\nTesting getNextPost()(...), Heap Error Case:\n\n";
    if (tester.RemoveErrorCase() == true){
        cout << "\tError Case of Heap getNextPost() passed!\n";
    }else{
        cout << "\tError Case of Heap getNextPost() failed!\n";
        return false;
    }


    cout << "\nTesting getNextPost()(...), Heap Edge Case:\n\n";
    if (tester.RemoveEdgeCase() == true){
        cout << "\tEdge Case of Heap getNextPost() passed!\n";
    }else{
        cout << "\tEdge Case of Heap getNextPost() failed!\n";
        return false;
    }

    cout << "\nTesting setPriorityFn()(...), Min Heap Normal Case:\n\n";
    if (tester.MinPriorityRebuild() == true){
        cout << "\tNormal Case of Min heap setPriorityFn() passed!\n";
    }else{
        cout << "\tNormal Case of Min heap setPriorityFn() failed!\n";
        return false;
    }

     cout << "\nTesting setPriorityFn()(...), Max Heap Normal Case:\n\n";
    if (tester.MaxPriorityRebuild() == true){
        cout << "\tNormal Case of Max heap setPriorityFn() passed!\n";
    }else{
        cout << "\tNormal Case of Max heap setPriorityFn() failed!\n";
        return false;
    }

    cout << "\nTesting mergeWithQueue(...), Error case:\n\n";
    if (tester.MergeErrorCase() == true){
        cout << "\tError case of mergeWithQueue passed!\n";
    }else{
        cout << "\tError case of mergeWithQueue failed!\n";
        return false;
    }

    cout << "\nTesting mergeWithQueue(...), Edge case:\n\n";
    if (tester.MergeEdgeCase() == true){
        cout << "\tEdge case of mergeWithQueue passed!\n";
    }else{
        cout << "\tEdge case of mergeWithQueue failed!\n";
        return false;
    }

    cout << "\nTesting copy constructor()(...), Normal case:\n\n";
    if (tester.CopyConstNormalCase() == true){
        cout << "\tNormal case of copy constructor Squeue() passed!\n";
    }else{
        cout << "\tNormal case of copy constructor Squeue() failed!\n"; 
        return false;
    }

    cout << "\nTesting copy constructor()(...), Edge case:\n\n";
    if (tester.CopyConstEdgeCase() == true){
        cout << "\tEdge case of copy constructor Squeue() passed!\n";
    }else{
        cout << "\tEdge case of copy constructor Squeue() failed!\n"; 
        return false;
    }

    cout << "\nTesting assingment operator SQueue& operator=()(...), Normal case:\n\n";
    if (tester.AssignConstNormalCase() == true){
        cout << "\tNormal case of assingment SQueue& operator=() passed!\n";
    }else{
        cout << "\tNormal case of assingment SQueue& operator=() failed!\n";
        return false;
    }

    cout << "\nTesting assingment operator SQueue& operator=()(...), Edge case:\n\n";
    if (tester.AssignConstEdgeCase() == true){
        cout << "\tEdge case of assingment SQueue& operator=() passed!\n";
    }else{
        cout << "\tEdge case of assingment SQueue& operator=() failed!\n";
        return false;
    }

    cout << "\nTesting insertPost()(...), Time Insertion:\n\n";
    if (tester.InsertTime() == true){
        cout << "\tTime Insertion of insertPost() passed!\n";
    }else{
        cout << "\tTime Insertion of insertPost() failed!\n";
        return false;
    }


    cout << "\nTesting  getNextPost()(...), Time Extraction:\n\n";
    if (tester.RemoveTime() == true){
        cout << "\tTime Extraction of getNextPost() passed!\n";
    }else{
        cout << "\tTime Extraction of  getNextPost() failed!\n";
        return false;
    }


    return true;
}


bool Tester::final(){
    // Preconditions: None
    // Postconditions: Return true if the skew heap follows min heap property after insertion else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //set the queue with min heap priority
    SQueue queue1(priorityFn2, MINHEAP);

    //insert 300 post into the heap
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    queue1.dump();


    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows min heap property 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    return true;

}

bool Tester::MinHeapInsertNormal(){
    // Preconditions: None
    // Postconditions: Return true if the skew heap follows min heap property after insertion else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //set the queue with min heap priority
    SQueue queue1(priorityFn2, MINHEAP);

    //insert 300 post into the heap
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows min heap property 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    return true;

}

bool Tester::MaxHeapInsertNormal(){
    // Preconditions: None
    // Postconditions: Return true if the skew heap follows min heap property after insertion else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //set the queue with max heap priority
    SQueue queue1(priorityFn1, MAXHEAP);


    //insert 300 post into the heap
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows max heap property 
    if(tester.MaxHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    return true;

}

bool Tester::MinHeapRemoveNormal(){
    // Preconditions: None
    // Postconditions: Return true if all removals happen in the min heap order else return false 

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //array containting all the removal
    int priority_arr[10000];

    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows min heap property 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //Iterates over the skew heap and remove each node one by one
    for (int i=0;i<300;i++){
        Post post = queue1.getNextPost();
        priority_arr[i] = priorityFn2(post);

    }

    //return false if any of the post priority was greater than it previous else returns true
    for (int i=0;i<299;i++){
        if(priority_arr[i] > priority_arr[i + 1])
            return false;

    }

    return true;

}
bool Tester::MaxHeapRemoveNormal(){
    // Preconditions: None
    // Postconditions: Return true if all removals happen in the max heap order else return false 

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows max heap property else return true
    if(tester.MaxHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //array containting all the removal
    int priority_arr[10000];

    //Iterates over the skew heap and remove each node one by one
    for (int i=0;i<300;i++){
        Post post = queue1.getNextPost();
        priority_arr[i] = priorityFn1(post);
    }

    //return false if any of the post priority was greater than it previous else returns true
    for (int i=0;i<299;i++){
        if(priority_arr[i] < priority_arr[i + 1])
            return false;
    }

    return true;

}

bool Tester::MaxPriorityRebuild(){
    // Preconditions: None
    // Postconditions: Return true if min skew heap is rebuild into max heap else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //create a squeque with min heap priority
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap doesn't follows min heap property 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //set the priority to max heap
    queue1.setPriorityFn(priorityFn1, MAXHEAP);
    
    //returns false if the skew heap still has min heap property after the rebuild 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == true){
        return false;
    }

    //returns false if the skew heap doesn't follows max heap property after rebuild 
    if(tester.MaxHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //return true after passing all the property test
    return true;

}
bool Tester::MinPriorityRebuild(){    
    // Preconditions: None
    // Postconditions: Return true if max skew heap is rebuild into min heap else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //create a squeque with min heap priority
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //set the vairable to check heap property  
    Tester tester;
    int i = 0;

    //returns false if the skew heap is not max heap before rebuild
    if(tester.MaxHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //set the priority to min heap
    queue1.setPriorityFn(priorityFn2, MINHEAP);

    //returns false if the skew heap still has max heap property after the rebuild   
    if(tester.MaxHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == true){
        return false;
    }

    //returns false if the skew heap doesn't follows min heap property 
    if(tester.MinHeapProperty(queue1.m_heap, i, queue1.m_size, queue1.m_priorFunc) == false){
        return false;
    }

    //return true after passing all the property test
    return true;
}

bool Tester::CopyConstNormalCase(){
    // Preconditions: None
    // Postconditions: Return true if copy constructor copies an object succesfully else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //Create queue2 with the copy of queue 1
    SQueue queue2(queue1);

    //return false if they both have same memory address and don't have equal size
    if( queue1.m_heap == queue2.m_heap || queue1.m_size !=  queue2.m_size)
        return false;

    //array containing the priority value of queue1 and 2
    int queue1_priority[10000];
    int queue2_priority[10000];

    //iterate and remove each node from queue1
    for (int i=0;i<300;i++){
        Post post = queue1.getNextPost();
        queue1_priority[i] = priorityFn1(post);
    }

    //iterate and remove each node from queue1
    for (int i=0;i<300;i++){
        Post post = queue2.getNextPost();
        queue2_priority[i] = priorityFn1(post);
    }

    //check if all the node matches 
    for (int i=0;i<300;i++){
        if(queue1_priority[i] != queue2_priority[i]){
            //return false if there's a mismatch else return true
            return false;

        }
    }

    return true;
}


bool Tester::CopyConstEdgeCase(){
    // Preconditions: None
    // Postconditions: Return true if copy constructor copies an edge case object succesfully else return false

    SQueue queue1(priorityFn1, MAXHEAP);

    SQueue queue2(queue1);

    if(queue2.m_size != queue1.m_size || queue2.m_size != 0 ){
        return false;
    }

    return true;
}

bool Tester::AssignConstEdgeCase(){
    // Preconditions: None 
    // Postconditions: Return true if copy constructor copies an edge case object succesfully else return false

    SQueue queue1(priorityFn1, MAXHEAP);

    SQueue queue2=(queue1);

    if(queue2.m_size != queue1.m_size || queue2.m_size != 0){
        return false;
    }

    return true;

}

bool Tester::AssignConstNormalCase(){
    // Preconditions: None
    // Postconditions: Return true if copy constructor copies an object succesfully else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //Create queue 2 and assigns copy of queue1
    SQueue queue2=(queue1);

    //return false if they both have same memory address and don't have equal size
    if( queue1.m_heap == queue2.m_heap || queue1.m_size !=  queue2.m_size)
        return false;

    //array containing the priority value of queue1 and 2
    int queue1_priority[10000];
    int queue2_priority[10000];

    //iterate and remove each node from queue1
    for (int i=0;i<300;i++){
        Post post = queue1.getNextPost();
        queue1_priority[i] = priorityFn1(post);
    }

    //iterate and remove each node from queue1
    for (int i=0;i<300;i++){
        Post post = queue2.getNextPost();
        queue2_priority[i] = priorityFn1(post);
    }

    //check if all the node matches 
    for (int i=0;i<300;i++){
        if(queue1_priority[i] != queue2_priority[i]){
            //return false if there's a mismatch else return true
            return false;

        }
    }

    return true;

}


bool Tester::RemoveErrorCase(){
    // Preconditions: None
    // Postconditions: Return true when dequeue an empty queue throws an out_of_range exception else return false

    //create an empty queue
    SQueue queue1(priorityFn1, MAXHEAP);

    try {
        //tries to dequeue the queue
        queue1.getNextPost();
    }
    //if the function catches the exception return true, else return false
    catch (out_of_range const&) {
        return true;
    }

    return false;
}

bool Tester::RemoveEdgeCase(){
    // Preconditions: None
    // Postconditions: Return true if it successfully remove the only node in the empty else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);

    //insert just one post into an queue
    Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
    queue1.insertPost(aPost);

    //remove the only node in the queue
    queue1.getNextPost();

    //return true if the root node was succesfully removed else return false
    if(queue1.m_size > 0){
        return false;
    }



    return true;

}

bool Tester::MergeErrorCase(){
    // Preconditions: None
    // Postconditions: Return true if merging queues with different priority functions throws a domain_error exception else retun fase

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //create a max skew heap with 300 posts
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //create a min skew heap with 300 posts
    SQueue queue2(priorityFn2, MINHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue2.insertPost(aPost);
    }

    try {
        //tries to merge heap with different priority
        queue1.mergeWithQueue(queue2);
    }
    //if the function catches the exception return true, else return false
    catch (domain_error const&) {
        return true;
    }

    return false;

}

bool Tester::MergeEdgeCase(){
    // Preconditions: None
    // Postconditions: Return true if empty queue is succesfully merged with a normal queue else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //create an empty queue
    SQueue queue1(priorityFn1, MAXHEAP);

    //create an queue with 300 post
    SQueue queue2(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue2.insertPost(aPost);
    }

    int size = queue2.m_size;

    queue1.mergeWithQueue(queue2);

    //checks if host queue is emptied after merge
    if(queue2.m_size > 0){
        return false;
    }

    //checks if both don't have same pointer or size
    if( queue1.m_heap == queue2.m_heap || queue1.m_size ==  queue2.m_size){
        return false;
    }

    //checks if all node are inserted in queue1
    if(size != queue1.m_size){
        return false;
    }

    return true;
}


bool Tester::InsertTime(){
    // Preconditions: None
    // Postconditions: Return true if insertion is performed in amortized logarithmic time else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //stores the clock ticks while running the program
    clock_t start, stop;
    clock_t start2, stop2;

    //queue sizes to calculate ratio
    int queue1_size = 1000;
    int queue2_size = 2000;

    //variable to count time difference
    int T1; //time for remove 1000 nodes from a tree
    int T2; //time for remove 2000 nodes from a tree

    //start the clock before insertion of queue1 and then stop it after
    start = clock();

    //insert 1000 node for queue1 skew heap
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<queue1_size;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    stop = clock();

    //calculates the difference to find the number of clock ticks the algorithm took for insert 1000 node
    T1 = stop - start;

     //start the clock before insertion of queue2 and then stop it after
    start2 = clock();

    //insert 2000 node for queue2 skew heap
    SQueue queue2(priorityFn1, MAXHEAP);
    for (int i=0;i<queue2_size;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue2.insertPost(aPost);
    }

    stop2 = clock();

    //calculates the difference to find the number of clock ticks the algorithm took for insert 2000 node
    T2 = stop2 - start2;

    //calc the ratio difference of queue sizes
    float ratio_size = (float)queue2_size/queue1_size;

    //calc the time difference of inserting 1000 vs 2000 nodes
    float final_ratio;
    final_ratio = (float)T2/(ratio_size * T1);

    if(final_ratio > (1 - 0.4) && final_ratio < (1.1 + 0.4) ){
        return true; //return true if it inside range, else return false
    }

    return false;

}

bool Tester::RemoveTime(){
    // Preconditions: None
    // Postconditions: Return true if extraction is performed in amortized logarithmic time else return false

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);

    //stores the clock ticks while running the program
    clock_t start, stop;
    clock_t start2, stop2;

    //queue sizes to calculate ratio
    int queue1_size = 1000;
    int queue2_size = 2000;

    //variable to count time difference
    int T1; //time for remove 1000 nodes from a tree
    int T2; //time for remove 2000 nodes from a tree

    //insert 1000 node for queue1 skew heap
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<queue1_size;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    //start the clock before extraction of queue1 and then stop it after
    start = clock();

    //iterate and remove 1000 node from queue1
    for (int i=0;i< queue1_size;i++){
        queue1.getNextPost();
    }

    stop = clock();

    //calculates the difference to find the number of clock ticks the algorithm took for extract 1000 nodes
    T1 = stop - start;


    //check if heap is emptied
    if(queue1.m_size > 0){
        return false;
    }

    //insert 2000 node for queue2 skew heap
    SQueue queue2(priorityFn1, MAXHEAP);
    for (int i=0;i<queue2_size;i++){
        Post aPost(idGen.getRandNum(),likesGen.getRandNum(), conLevelGen.getRandNum(), timeGen.getRandNum(),interestGen.getRandNum());
        queue2.insertPost(aPost);
    }

    //start the clock before extraction of queue2 and then stop it after
    start2 = clock();

    //iterate and remove 2000 node from queue2
    for (int i=0;i< queue2_size;i++){
        queue2.getNextPost();
    }

    stop2 = clock();

    //calculates the difference to find the number of clock ticks the algorithm took for extract 1000 nodes
    T2 = stop2 - start2;
    

    //check if heap is emptied
    if(queue2.m_size > 0){
        return false;
    }


    //calc the ratio difference of queueu sizes
    float ratio_size = (float)queue2_size/queue1_size;


    //calc the time difference of extracting 1000 vs 2000 nodes
    float final_ratio;
    final_ratio = (float)T2/(ratio_size * T1);

    if(final_ratio > (1 - 0.4) && final_ratio < (1.1 + 0.4) ){
        return true; //return true if it inside range, else return false
    }

    return false;

}
