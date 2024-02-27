/// *Name: Ruben Langarica, 5002376361 , Assignment number 6
/// *description: features a decoder that will take a level order binary 
/// tree list, generate the tree, and then return all of the available 
/// traversals. Using this technology, you should be able to intercept 
/// and decipher any potential messages that Spectre's agents may be 
/// attempting to send within your vicinity.
/// *input: the input would be by the user, by setting which decription
/// would be done used as 1-5, and 0 to exit the program.
/// *output: the program will output the traversals and mirror it and 
/// then print out the traversals of that to find what the encripted 
/// message would be. 
/// Build as: g++ cs302hw6.cpp -g -std=++17
/// Execute as: ./a.out
/// Debug as: 'gdb ./a.out'
/// Check for memory leaks with valgrind ./a.out

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

//
#define DEBUG //Uncomment This to run checks, Comment it to run final version. 
//Program should work both ways. TAs will grade with both settings... they better!
//

/// --------------------------------------------------------------------
    /// @note This is a template class for the node, this node has
   /// a left, right and data pointer. This also has an overloaded 
   /// constructor and assigment opperator. While also having a << >>
   /// overloaded. 
/// --------------------------------------------------------------------
template < class T>
class node {
  public:
  
    //Your Code Here: Default Constructor
    node() : left(nullptr), right(nullptr), data(new T) {}
	
    //Initializes a node with left and right pointers set to nullptr and sets the value of data to the provided variable var.
    node(const T &var) : left(nullptr), right(nullptr) {
    data = new T;
    *data = var;
	//~ this->left = nullptr;
	//~ this->right=nullptr;
        
}

    //Your Code Here for Destructor
	//Deallocates the memory allocated for data and sets left and right pointers to nullptr.
~node(){
delete data;
left = nullptr;
right = nullptr;
//~ delete left;
//~ delete right;
}

/// --------------------------------------------------------------------
    /// @return a node
    /// @note this is a function to overload the copy contructor so a
    /// node(node) can be made.
	/// Overloads the copy constructor to create a new node with the same data, left, and right values as the provided other node.
/// --------------------------------------------------------------------
    node(const node& other);
     //~ if(other.left){
	 //~ left = new node<T>(other.left);	 
		 
	  //~ }
	  //~ if(other.right){
	//~ right = new node<T>(other.right);
		 
	  //~ }
		
	//~ }
	
/// --------------------------------------------------------------------
    /// @return a node
    /// @note this is a function to overload the assignment contructor 
    /// so a node can be assigned to a node. part of rule of 3.
	/// Overloads the assignment operator to assign the values of data, left, and right from the provided other node.
/// --------------------------------------------------------------------	
    node& operator=(const node& other){
	 data = other.data;
	 node<T> *left_T = left;
	 left = new node(*other.left);
	 delete left_T;
     node<T> *right_T = right;
	 left = new node(*other.right);
	 delete right_T; 
	 
	 return *this;
}

/// --------------------------------------------------------------------
    /// @note this is a node swap function that would swap this node 
    /// with other node.
	/// Swaps the values of left, right, and data between the current node and the provided other node.
/// --------------------------------------------------------------------
	void swap(node& other) {

    node *tempL; //node used to store left pointer
    node *tempR; //node used to store right pointer
    node *tempdata; //node used to store temp data
    //left pointer swap
    tempL = left;
	left = other.left;
	other.left = tempL;
	//right pointer swap
	tempR = right;
	right = other.right;
	other.right = tempR;
	//data swap
	tempdata = data;
	data = other.data;
	other.data = tempdata;
	
}
	
/// --------------------------------------------------------------------
    /// @note this is an Assignment Operator Overload to take literals
	/// Overloads the assignment operator to set the value of data to the provided literal variable var.
/// --------------------------------------------------------------------
    void operator=(const T &var){ 
        *data = var;
    }
    
/// --------------------------------------------------------------------
    /// @note this is a function to set the data of the node class.
	/// Sets the value of data to the provided variable var.
/// --------------------------------------------------------------------
    void setData(T var) {
        *data = var;
    }
    
/// --------------------------------------------------------------------
    /// @return *data of T
    /// @note this is a function to return data
	/// Returns the value stored in data
/// --------------------------------------------------------------------
    T getData() const {
        //Your Code Here
        return *data;
}

     // << operator overloading to read into n.data of a node&n.
	 // Overloads the output stream operator to print the value stored in data.
    friend std::ostream& operator<<(std::ostream& out, const node& n){
        out << *n.data;
}
    
    // >> operator overloading to read into n.data of a node&n. Make it inline
	// Overloads the input stream operator to read a value into data.
    friend std::istream& operator>>(std::istream& in, const node& n){
		in >> *n.data;
}
  protected:
    T    *data; //data storage variable  Pointer to the data storage variable.
    node *left; //left side pointer      pointer to the left child node.
    node *right; //right side pointer     Pointer to the right child node.
    template <class U> friend class BinaryTree; //<--Don't you dare to @#$%$ touch this. Not even Chat GPT can touch this 
};
/// --------------------------------------------------------------------
    /// @note this is a class of BinaryTree with the friend class to 
    /// node class for protected member access. This class has a root
    /// that is set to nullptr. While a size for it set to 0.
/// --------------------------------------------------------------------
template <class T>
class BinaryTree{
  public:
  

    //this is the Default contructor for Binarytree().
    BinaryTree() : root(nullptr), size(0){}
    
/// --------------------------------------------------------------------
    /// @note this is the function to have a destructor. This is part of
    /// rule of 3.
/// --------------------------------------------------------------------
    virtual ~BinaryTree(){                
    #ifdef DEBUG
        cout << "De-allocating Tree here" << endl;
    #endif
        DeleteTree(&root);
        root = nullptr;
}
    
/// --------------------------------------------------------------------
    /// @return a BinaryTree
    /// @note this is the function to have a copy constructor. This is
    /// part of rule of 3.
/// --------------------------------------------------------------------
    BinaryTree(const BinaryTree &b2){
    #ifdef DEBUG
        cout << "Manually copying tree in copy constructor" << endl;
    #endif
        //Your Code Here (Should call copyTree)
    this->size = copyTree(b2.root, &(this->root));
}
    
/// --------------------------------------------------------------------
    /// @return size of the copied tree which is an int.
    /// @note this is a copy function that takes in a tree and a double
    /// pointer to another tree to copy them and make them the same.
    /// this function is called by the copy constructor.
/// --------------------------------------------------------------------
    int copyTree(node<T>* currOG, node<T>** currNEW){
        int total; //variable used for the total return
        int totalright; //variable used for the rightside return
        int totalleft; //variable used for the left side return
        if(currOG ==nullptr){
			*currNEW = nullptr;
			return 0;
		}else{
			
		*currNEW = new node<T>(*currOG->data);
		//calculation for total size
		totalright=copyTree(currOG->left,&((*currNEW)->left));
		totalleft=copyTree(currOG->right,&((*currNEW)->right));
		total= 1+totalright +totalleft;
		return total;
	}	
}

/// --------------------------------------------------------------------
    /// @return a BinaryTree that is mirrored.
    /// @note this is a function overload for | operator to act as a
    /// mirror :D.
/// --------------------------------------------------------------------
    BinaryTree operator|(const BinaryTree& OG){
        #ifdef DEBUG
        cout << "Mirroring Tree!" << endl;
        #endif
        if(OG.root == nullptr) return OG;
        DeleteTree(&root);
        this->root = nullptr;
        this->size = mirrorTree(OG.root, &(this->root));
        return OG;
}
    
/// --------------------------------------------------------------------
    /// @return an int of the mirrored tree.
    /// @note this is the function to mirror a tree which is the same 
    /// roughtly as copy tree, this would just have the recursive
    /// currNEW switched so a mirror can happen.
/// --------------------------------------------------------------------
    int mirrorTree(node<T>* currOG, node<T>** currNEW){
        //Your Code Here
        int total; //variable used for the total return
        int totalright; //variable used for the rightside return
        int totalleft; //variable used for the left side return
        if (currOG == nullptr){
			currNEW = nullptr;
			return 0; //return 0 for no elements added
		}else{
		
		*currNEW = new node<T>(*currOG->data);
		//calculation for total size
		totalleft=mirrorTree(currOG->left,&((*currNEW)->right));
		totalright=mirrorTree(currOG->right,&((*currNEW)->left));
		total= 1+totalright +totalleft;
		return total;
	}
}
    
/// --------------------------------------------------------------------
    /// @note this is an assignment operator overload function 
    /// this is part rule of 3.
/// --------------------------------------------------------------------
    void operator=(const BinaryTree &b2){ 
        #ifdef DEBUG
        cout << "Manually copy tree in assignment operator" << endl;
        #endif
        DeleteTree(&root);
        this->root = nullptr;
        this->size = copyTree(b2.root, &(this->root));
}
    
/// --------------------------------------------------------------------
    /// @note this is a function that would read and generate the tree.
    /// this is done by taking the array and array size, while passing
    /// them to recInsert for it to use them, This will print what it is
    /// generated from and the size of the array. while also getting the
    /// number of items inserted from recInsert.
/// --------------------------------------------------------------------
    void readnGenerate(const T a[], int arr_size){
        if(arr_size <= 0) return; //empty
        #ifdef DEBUG
        cout << "Generating From: '" << a << "'\nOG Length: 00" << arr_size << endl;
        #endif
        DeleteTree(&root);
        this->size = recInsert(a, arr_size, &root, 1);
        #ifdef DEBUG
        cout << "Total Inserted: "  << this->size << endl;
        #endif

}
    
/// --------------------------------------------------------------------
    /// @return the size of the items inserted as an int
    /// @note this is a function that takes in an array, arraysize,
    /// a root, and a currentindex. This does it by level insert. This
    /// function will take any char into the tree while skipping over
    /// % and not adding into the count 
/// --------------------------------------------------------------------  
    int recInsert(const T *a, int arrSize, node<T>**curr, int currIndex){
        //Your Code Here
        int leftchild;  //left side calculation storage
        int rightchild; //right side calculation storage
        //int size;
		
		//base case 
        if(currIndex < arrSize+1){
		//~ if (*curr->data != '%'){
			//~ if(*curr != nullptr){
				//delete curr and  its possible children
			//~ }
			
			if (!(a[currIndex-1] == '%')){
				*curr = new node<T>(a[currIndex-1]);
			}else{
			return 0; //*curr = new node<T>('?');
		}
		
		//~ }
        //node<T> temp;
        //*temp.data = a[currIndex-1];
		//curr = temp;
		//put data into the root
		//~ *((*curr)->data) = a[currIndex-1];
		// right and left calculations
        leftchild = currIndex *2;
        rightchild = currIndex *2 +1;
        
return 1 + recInsert(a , arrSize, &((*curr)->left) , leftchild) + recInsert(a , arrSize, &((*curr)->right) , rightchild);
}
return 0;
}

/// --------------------------------------------------------------------
    /// @note this is a inorder traversal function which will allow the
    /// print out of the tree to be inorder. This has the root set 
    /// within it.
/// --------------------------------------------------------------------
    void inOrderTraversal(node<T>* curr){
    //curr set to root
	if (curr == nullptr){
		curr = root;
	}
	// check to see if root would be nullptr
		if (curr == nullptr){
		return;
		}
		//left check of the tree
		if (curr->left != nullptr){
		inOrderTraversal(curr->left);
	}
		cout << *curr->data;
		//right check of the tree
		if (curr->right !=nullptr){
		inOrderTraversal(curr->right);
	}
	
}
    
/// --------------------------------------------------------------------
    /// @note this is a preorder traversal function which will allow the
    /// print out of the tree to be preorder. This has the root set 
    /// within it.
/// --------------------------------------------------------------------
    void preOrderTraversal(node<T>* curr){
        //curr set to root
        if (curr == nullptr){
		curr = root;
		}
		// check to see if root would be nullptr
		if (curr == nullptr){
		return;
		
		}
        cout << *curr->data;
        //left check of the tree
        if (curr->left !=nullptr){
        preOrderTraversal(curr->left);
	}
		//right check of the tree
        if (curr->right !=nullptr){
        preOrderTraversal(curr->right);
	}

}
    
/// --------------------------------------------------------------------
    /// @note this is a postorder traversal function which will allow the
    /// print out of the tree to be postorder. This has the root set 
    /// within it.
/// --------------------------------------------------------------------
    void postOrderTraversal(node<T>* curr){
        //curr set to root
        if (curr == nullptr){
		curr = root;
		
		}
		// check to see if root would be nullptr
		if (curr == nullptr){
		return;
		
		}
		//left check of the tree
		if (curr->left !=nullptr){
        postOrderTraversal(curr->left);
	}
		//right check of the tree
		if (curr->right !=nullptr){
        postOrderTraversal(curr->right);
	}
        cout << *curr->data;


}

/// --------------------------------------------------------------------
    /// @return an int with the height of the curr.
    /// @note this is a function to get the hight of what curr is
/// --------------------------------------------------------------------
    int getHeight(node<T> *curr){
        //Compute Height of both subtrees and keep the larger one
        if(curr == nullptr) return 0; //base case
        else{
            int lheight = getHeight(curr->left); 
            int rheight = getHeight(curr->right);
            if  (lheight > rheight) return lheight+1;
            else return rheight+1;
        }
}
    
/// --------------------------------------------------------------------
    /// @note this is a funtion to print in a specificlevel
/// --------------------------------------------------------------------    
    void printSpecificLevel(node<T> *root, int level){
        if(root == nullptr) return;
        if(level == 1) cout << *root;
        else if(level > 1){
            printSpecificLevel(root->left, level-1);
            printSpecificLevel(root->right, level-1);
        }
}
    
/// --------------------------------------------------------------------
    /// @note this is a function that prints in level order
/// --------------------------------------------------------------------
    void printLevelOrderTraversal(){
        for(int i=0; i <= getHeight(root); i++){
            //Your Code Here. Should be one line of code, 
            //a fucntion call to printSpecificLevel with root and i as parameters
            printSpecificLevel(root,i);
        }
}

  protected:
    node<T> *root; //binarytree root
    int size; //size of the Binarytree
  private:

/// --------------------------------------------------------------------
    /// @note this is the funtion to destroy the tree. This would 
    /// usually take an address of the root. This function would be used
    /// in the destructor, this would traverse the tree first while 
    /// deleting the left node first before the right and setting *curr
    /// = to nullptr to avoid any dangling pointer.
/// --------------------------------------------------------------------
    void DeleteTree(node<T>** curr){
        //~ //Your Code Here


        if (*curr == nullptr){
		return;
		}

        DeleteTree(&((*curr)->left));
        DeleteTree(&((*curr)->right));
        
        delete *curr;
        *curr = nullptr;
    }
			
		//~ }
		//~ if (curr == nullptr){
		//~ return;
	//~ }
	   //~ if (curr == nullptr){
		//~ return;
		
		//~ }
		
		//~ if ((&(*curr)->left) != nullptr){
			//~ DeleteTree(&((*curr)->left));
		//~ }
		
		//~ else if ((&(*curr)->right) != nullptr){
			//~ DeleteTree(&((*curr)->right));
		//~ }
		//~ curr = nullptr;
		//~ delete curr;
		
		 
    
};

/// --------------------------------------------------------------------
    /// @note this is main class function that is used in the main class
    /// the client (main code) interacts with and contains a BinaryTree 
    /// and a BinaryTree temp pointer so it can handle copying around 
    /// data when mirroring the shape of the trees. This has a 
    /// copy, assignment overload and destructor.
/// --------------------------------------------------------------------
template<class T>
class OmegaSeamaster {
  public:
	//Default constructor for OmegaSeamaster.
    OmegaSeamaster() : BinaryTreeTemp(nullptr) {}
 
/// --------------------------------------------------------------------
    /// @return OmegaSeamaster
    /// @note this is a function to overload the copy constructor.
/// --------------------------------------------------------------------    
    OmegaSeamaster(const OmegaSeamaster& other){
	//~ *BinaryTreeTemp = *other.BinaryTreeTemp;	
	BinaryTreeTemp = new T;
	*BinaryTreeTemp = (other);
}
    
/// --------------------------------------------------------------------
    /// @return a OmegaSeamaster
    /// @note this is a function to voerload the assigment opperator.
/// --------------------------------------------------------------------
    OmegaSeamaster& operator=(const OmegaSeamaster& other){
    //~ if (this != &other) {
    //~ OmegaSeamaster(other).swap(*this);
    //~ }
    //~ return *this;
    //~ auto tempB = BinaryTree<T>(nullptr);
    
    //~ tempB = this;
	//~ this = other;
	//~ other = tempB;
	*BinaryTreeTemp = *(other.BinaryTreeTemp);
	*BinaryTreeFunction = *(other.BinaryTreeFunction);
}
	
    
    BinaryTree<T>  BinaryTreeFunction; //Binarytree
    BinaryTree<T>* BinaryTreeTemp; //Binarytree temp
    
/// --------------------------------------------------------------------
    /// @note this is a function that would be called when anything 
    /// other then 0-5 is inserted, this is to sound the alarm for going
    /// out of bound.
/// --------------------------------------------------------------------
    void Alarm(){
        cout << "Alarm: BOOM!" << endl;
}
    
/// --------------------------------------------------------------------
    /// @note this is a function to get the time and print it out, this
    /// is done in hour, min, and seconds.
/// --------------------------------------------------------------------
    void Time(){
        time_t t = std::time(0);
        cout << "                      Time: " << localtime(&t)->tm_hour 
             << ":" << localtime(&t)->tm_min 
             << ":" << localtime(&t)->tm_sec << endl;
}
    
/// --------------------------------------------------------------------
    /// @note this is the function that would clean up the temp 
    /// BinaryTree that would be created. This is done with delete, and 
    /// letting the destructor of BinaryTree class take care of the rest
/// --------------------------------------------------------------------
    void CleanTemp(){
       //~ BinaryTree<T>::DeleteTree(&(BinaryTreeTemp->root));
       delete BinaryTreeTemp;
        
}
    
/// --------------------------------------------------------------------
    /// @note this is a function to make the clock draw out in the
    /// program 
/// --------------------------------------------------------------------
    void Draw(){
        cout << "                           ||||                           \n";
        cout << "                   /\\      ||||      /\\                   \n";
        cout << "                   \\/             /| \\/                   \n";
        cout << "               /\\      |\\        / |     /\\               \n";
        cout << "               \\/       \\\\      / /      \\/               \n";
        cout << "                         \\\\    / /                        \n";
        cout << "           ____           \\\\__/ /         ____            \n";
        cout << "          |____|           |__|/         |____|           \n";
        cout << "                            /                             \n";
        cout << "                           /                              \n";
        cout << "               /\\         /              /\\               \n";
        cout << "               \\/        / Omega         \\/               \n";
        cout << "                   /\\   /            /\\                   \n";
        cout << "                   \\/  /    ||       \\/                   \n";
        cout << "                            ||                            " << endl; //Feel Free to make this pretier.
    } //Extra Credit Points if you make the watch actually work and draw the hands in the right spots representing current time.
    //Reedem your test grade and do the extra credit!!!
    
/// --------------------------------------------------------------------
    /// @note this is the destructor for OmegaSeamaster.
/// --------------------------------------------------------------------
    ~OmegaSeamaster(){
        //Your Code Here
        delete BinaryTreeTemp;
    }
};

/// --------------------------------------------------------------------
    /// @return a node<T>
    /// @note this is the function for an overloaded copy constructor so
    /// a node(node) can be made.
/// --------------------------------------------------------------------
template <class T>
node<T>::node(const node& other) {
	data = new T;
	*data = *(other.data);
	this->left = other.left;
	this->right= other.right;
	/*data = other.data;

	auto *nodes = new node; 
	*nodes = node<T>();
	
	 left=node<T>(*other.left, &(this->left));
	 right=node<T>(other.right, &(this->right));
	 
	 //~ data = other.data;
	*/
	 //~ if(other.left){
	//~ left = new node<T>(other.left);	 
		 
	 //~ }
	 //~ if(other.right){
	//~ right = new node<T>(other.right);
		 
	 //~ }
}

/// --------------------------------------------------------------------
    /// @return 007 upon success
    /// @note this is main function, where everything is used 
    ///this has a debug mode which would allow testing of the node class
    ///seeing if node is templeted correctly. This will then also have 
    /// a binaryTree class test to see if that class is working fine.
    /// These test are helpful when determining where the problem would 
    /// be coming from. Finally the main function does it test on the
    /// OmegaSeamaster class to see that everything in there is 
    /// implmented correctly. When debug is turned off the program will
    /// run as completed version of just watch decription of 1-5 and
    /// 0 to exit.
/// --------------------------------------------------------------------
//You are not allowed to modify any code beneath this line. DO NOT TOUCH MAIN!
#ifdef DEBUG
#include<sstream>
#endif
//Client Code Given To Q Branch
int main(void){
#ifdef DEBUG
    cout << "Q BRANCH DEBUG MODE ONLINE. SHOULD HAVE 0 MEMORY LEAKS" << endl;
    //Node Tests
    node<int>* testNode0 = new node<int>;
    testNode0->setData(007);
    node<int> testNode1(*testNode0);
    cout << testNode0->getData() << " = "<< *testNode0 << " = " << testNode1 << " = 7"<< endl;
    *testNode0 = 0;
    cout << *testNode0 << " = 0 and " << *testNode0 << " != " << testNode1 << endl;

    node<string>* testNode2 = new node<string>;
    string agentname = "James Bond";
    stringstream ss(agentname);
    ss >> *testNode2;
    cout << *testNode2 << " ";
    ss >> *testNode2;
    cout << *testNode2 << endl;
    node<float> testNode3 = 0.07f;
    cout << testNode3 << " = 0.07" << endl;
    cout << reinterpret_cast<void*>(testNode2) << " <--Some Memory Address that is not 0" << endl;
    node<int>* testNode4 = testNode0;
    delete testNode4; delete testNode2;
    //Binary Tree Tests
    char testmsg1[] = {'R','O','L','E','X','%', '?','\0'};
    //All % symbols indicate empty spots in a tree and should not be inserted, but skipped instead
    BinaryTree<char> *testTree0 = new BinaryTree<char>;
    BinaryTree<char> *testTree1 = new BinaryTree<char>;
    cout << "Read and Traversal Tests: " << endl;
    testTree0->readnGenerate(testmsg1, string(testmsg1).length());
    cout << "Total Inserted Should be 6 since % was skipped" << endl;
    testTree0->inOrderTraversal(NULL);   cout<<" = EOXRL?"<<endl;
    testTree0->preOrderTraversal(NULL);  cout<<" = ROEXL?"<<endl;
    testTree0->postOrderTraversal(NULL); cout<<" = EXO?LR"<<endl;
    testTree0->printLevelOrderTraversal();cout<<" = ROLEX?"<<endl;
    cout << "End Traversal Tests" << endl;
    cout << "Mirror Test and Traversals:" << endl;
    BinaryTree<char> testTree2; 
    testTree2 | *testTree0; //Mirror
    testTree2.printLevelOrderTraversal();cout<<" = RLO?XE"<<endl;
    testTree2.inOrderTraversal(NULL); cout<<" = ?LRXOE"<<endl;
    testTree2.preOrderTraversal(NULL); cout<<" = RL?OXE"<<endl;
    testTree2.postOrderTraversal(NULL); cout<<" = ?LXEOR"<<endl;
    cout << "End Mirror Traversal Tests" << endl;
    *testTree1 = *testTree0;
    cout << "Assignment Operator Test" << endl;
    testTree1->printLevelOrderTraversal(); cout<<" = ROLEX?"<<endl;
    BinaryTree<char> testTree3(*testTree0);
    cout << "Copy Constructor Test" << endl;
    testTree3.printLevelOrderTraversal(); cout<<" = ROLEX?"<<endl;
    delete testTree0; testTree0 = NULL;
    testTree1->printLevelOrderTraversal();cout<<" = ROLEX?"<<endl;
    delete testTree1; testTree1 = NULL;
    BinaryTree<int> testTree4; cout<<"Align Arrows -->" << flush;
    testTree4.postOrderTraversal(NULL);cout<<"<--"<<endl;
    cout << "Good Job, Agent. End of Debug Section" << endl;
    cout << "\n************************************************" << endl;
#endif

    //Captured Chatter:
    string msg0 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string msg1 = "Shaken, not Stirred";
    string msg2 = "You were the chosen one!";
    string msg3 = "Bond, James Bond";
    string cryptmsg[6];
    cryptmsg[0] = "C3PO Bot Never Forgets";
    cryptmsg[1] = "adl 3aa .nr..idn erioBe fmno oerRuraw eBnF .isy  acS";
    cryptmsg[2] = "Roaleuue g tfl%x?Oma,Bei";
    cryptmsg[3] = "..R.  ad . nnrin ewsdneiau racSoBforaeBya 3i mo lreF";
    cryptmsg[4] = "nS r   !ef'grAh%!cp osteaeasTeT%%!et";
    cryptmsg[5] = "Wehi er% ?700 s%%%%%%%%%%%%%%%%392aaahuusf%riiffooambnctnas";
    //End of Captured Chatter

    cout << "\nGood evening Mr. Bond, your latest Q Watch v2.01 Beta features a decoder that\n"
         << "will take a level order binary tree list, generate the tree, and then return\n"
         << "all of the available traversals. Using this technology you should be able to\n"
         << "intercept and decipher any potential messages that Spectre's agents may be\n"
         << "attempting to send within your vicinity.\n"
         << "You should find it quite useful in the field. Oh, one word of warning.\n"
         << "The alarm is rather loud. If you know what I mean.\n" << endl;
    OmegaSeamaster<char> myWatch;

    //Cypher Tester
    myWatch.BinaryTreeFunction.readnGenerate(msg0.c_str(), string(msg0).length());
    #ifdef DEBUG
    cout << "LVL : "; myWatch.BinaryTreeFunction.printLevelOrderTraversal();cout<<endl;
    cout << "IN  : "; myWatch.BinaryTreeFunction.inOrderTraversal(NULL);cout<<endl;
    cout << "PRE : "; myWatch.BinaryTreeFunction.preOrderTraversal(NULL);cout<<endl;    
    cout << "POST: "; myWatch.BinaryTreeFunction.postOrderTraversal(NULL);cout<<endl;
    #endif
    myWatch.BinaryTreeTemp = new BinaryTree<char>(myWatch.BinaryTreeFunction);
    myWatch.BinaryTreeFunction | *myWatch.BinaryTreeTemp;
    #ifdef DEBUG
    cout << "LVL : "; myWatch.BinaryTreeFunction.printLevelOrderTraversal();cout<<endl;
    cout << "IN  : "; myWatch.BinaryTreeFunction.inOrderTraversal(NULL);cout<<endl;
    cout << "PRE : "; myWatch.BinaryTreeFunction.preOrderTraversal(NULL);cout<<endl;    
    cout << "POST: "; myWatch.BinaryTreeFunction.postOrderTraversal(NULL);cout<<endl;
    #endif

    myWatch.Time();
    myWatch.Draw();

    //Cryptomsg
    int msgnum = 0;
    for(;;){
        cout << "\nEnter a Frequency from 1-5 to Decrypt. Enter 0 to Exit: " << endl;
        cin >> msgnum; if(msgnum == 0) goto q; if(msgnum < 0 || msgnum > 5) goto BondJamesBond;
        myWatch.BinaryTreeFunction.readnGenerate(cryptmsg[msgnum].c_str(), string(cryptmsg[msgnum]).length());

        cout << "LVL : "; myWatch.BinaryTreeFunction.printLevelOrderTraversal();cout<<endl;
        cout << "IN  : "; myWatch.BinaryTreeFunction.inOrderTraversal(NULL);cout<<endl;
        cout << "PRE : "; myWatch.BinaryTreeFunction.preOrderTraversal(NULL);cout<<endl;    
        cout << "POST: "; myWatch.BinaryTreeFunction.postOrderTraversal(NULL);cout<<endl;

        myWatch.CleanTemp();
        myWatch.BinaryTreeTemp = new BinaryTree<char>(myWatch.BinaryTreeFunction);
        myWatch.BinaryTreeFunction | *myWatch.BinaryTreeTemp;

        cout << "LVL : "; myWatch.BinaryTreeFunction.printLevelOrderTraversal();cout<<endl;
        cout << "IN  : "; myWatch.BinaryTreeFunction.inOrderTraversal(NULL);cout<<endl;
        cout << "PRE : "; myWatch.BinaryTreeFunction.preOrderTraversal(NULL);cout<<endl;    
        cout << "POST: "; myWatch.BinaryTreeFunction.postOrderTraversal(NULL);cout<<endl;
    } BondJamesBond:

    myWatch.Alarm();
    cout << "OmegaLuL" << endl; q:;
    return 007;
}
