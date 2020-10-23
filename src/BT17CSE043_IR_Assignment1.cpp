/*
																		Programming Assignment No. 1
																				
Suppose that we are designing a program to simulate the search in a dictionary. 
Words appear with different frequencies, however and it may be the case that a frequently used word 
which is in the stop list like "the" appear far from the root if they are sorted lexicographically 
while a rarely used word such as "consciousness" appears near the root. 
We want that the words that occur frequently in the text to be placed nearer to the root.
Moreover, there may be words in the dictionary for which there is no definition. 
Organize an optimal binary search tree that simulates the storage and search of words in a dictionary. 

=============================================================================================================================================================
Approach:

1. Read from file and clean (remove punctuations, convert to lower case) the words.
2. Calculate the frequencies and sort.
3. Build the optimal binary tree.
4. Search for words.
5. Insert a word with/without meaning in dictionary.
===============================================================================================================================================================

Made by

Author: Mugdha Satish Kolhe
Enrollment No: BT17CSE043
Course Code: CSL-436
Course: Information Retrieval
================================================================================================================================================================

*/

#include <bits/stdc++.h>
using namespace std;

typedef struct node
{
	string s;			//string
	int f;				//frequency
	int ht;				//level
	int m;				//meaning
	struct  node *left, *right; //child
}node;


void printBinaryTree(node *root, int space = 0, int height = 10)	//2D printing of Binary tree
{
    if (root == NULL)
        return;
	space += height;
	printBinaryTree(root->right, space);
    cout << endl;
    //fout<<endl;
	for (int i = height; i < space; i++)
        cout << ' ';
    cout << root->s;
	cout << endl;
	//fout << root->s;
	//fout << endl;
    printBinaryTree(root->left, space);
}

/*void printOBST(node *start)	//print optimal BST
{
	if(start==NULL)
	{
		cout<<"Error at 67"<<endl;
		return;
	}
	cout<<"The tree is:"<<endl;
	queue<node*>q;
	node*temp=NULL;
	int ht=start->ht;
	q.push(start);
	while(!q.empty())
	{
		temp=q.front();
		if(temp!=NULL)
		{
			q.push(temp->left);
			q.push(temp->right);
			if(temp->ht==ht)
			{
				cout<<temp->s<<"\n";
			}
			else
			{
				cout<<endl;
				ht=temp->ht;
				cout<<temp->s<<"\n";
			}
		}
		q.pop();
	}
	cout<<endl;
}
*/

void print_word_freq(vector <pair< string, int > > const &a) 	//print word and its frequency
{
   cout << "The vector elements are : "<<endl;

   for(int i=0; i < a.size(); i++)
   		cout << a[i].first<<"-"<<a[i].second << endl;
}

void insert(vector<pair<string, int > > &v,string s)	//Check if the word exist in vector and insert the vector
{
	bool t=false;
	vector<pair<string, int > >::iterator it;
	for(it = v.begin(); it!=v.end() && t==false; it++ )
	{ 
		if(it->first == s )
		{ 
			it->second++; 
			t=true;
		}
	}
	if(t==false)
	{
		v.push_back(pair<string,int>(s,1));
	}
}

int add_freq(vector <pair<string,int > > const &a, int i, int j)  //sum the freq
{  
    int s = 0;  
    for (int k = i; k <= j; k++)  
    	s += a[k].second;  
    return s;  
}

void build_cost_matrix(vector<pair<string,int > > &v,int ** dp, int ** root)	// insert the cost matrix named dp and root matrix containing key to be put at nodes
{
	int n=v.size();
	for (int i = 0; i < n; i++)	//single key
	{
		dp[i][i] = v[i].second;
		root[i][i]=i;
	} 
    for (int L = 2; L <= n; L++)  //construction of matrix chains of 2, 3 .....
    {   
        for (int i = 0; i < n-L+1; i++)  
        {  
            int j = i+L-1;  
            dp[i][j] = INT_MAX;  
  
             
            for (int r = i; r <= j; r++)  
            {  
	           int c = ((r > i)? dp[i][r-1]:0) + ((r < j)? dp[r+1][j]:0) +  add_freq(v, i, j);  //formula  c = cost when keys[r] becomes root of this subtree  
	            if (c < dp[i][j]) 
				{
	            	dp[i][j] = c; 
	                root[i][j]=r;
	            }
                 
            }  
        }  
    } 
   cout<<"The cost of optimal BST is: "<<dp[0][n-1]<<endl;
    
}

node* build_opt_bst(vector<pair<string,int > > &v,int ** dp, int ** root,int i,int j,int height)	//Create optimal bst
{
	node* ret=NULL;
	if(i<=j)
	{
		ret=new node;
		if(ret==NULL)
		{
			cout<<"Error at line 169"<<endl;
		}
		else
		{
			if(i==j)
			{
				ret->s=v[i].first;
				ret->f=v[i].second;
				ret->left=ret->right=NULL;
				ret->ht=height;
			}
			else
			{
				int r=root[i][j];
				ret->s=v[r].first;
				ret->f=v[r].second;
				ret->ht=height;
				ret->left=build_opt_bst(v,dp,root,i,r-1,height+1);
				ret->right=build_opt_bst(v,dp,root,r+1,j,height+1);
			}
		}
	}
	return ret;
}
string searchOBST(node* start,string str)	//search optimal bst
{
	node* temp=start;
	string m="";
	if(temp!=NULL)
	{
		if(temp->s==str)
		{
			cout<<endl<<str<<" found at depth "<<temp->ht<<endl;
			m=temp->m;
		}
		else if(temp->s>str)
		{
			searchOBST(temp->left,str);
		}
		else if(temp->s<str)
		{
			searchOBST(temp->right,str);
		}
		else
		{
			cout<<"Error at 214"<<endl;
		}
	}
	else
	{
		cout<<str<<" not found "<<endl;
	}
	return m;
}


int main()
{
	//string stores the word, int stores the frequency, index will be used to calculate root.
	vector<pair<string,int > >freq;
    vector<pair<string, int > >::iterator it;

	//file operations
	fstream file;
	string filename = "input2.txt",word;
	file.open(filename.c_str());
	while (file >> word) 
    { 
        //adding frequency
        for (int i = 0, len = word.size(); i < len; i++) 
   		{ 
        	if (ispunct(word[i])) 		// check whether parsing character is punctuation or not 
        	{ 
            	word.erase(i--, 1); 
            	len = word.size(); 
        	} 
    	}
        transform(word.begin(), word.end(), word.begin(), ::tolower);
		if(!(find_if(word.begin(), word.end(), ::isdigit) != word.end()))  
        	insert(freq,word);
    } 
    file.close();
    sort(freq.begin(), freq.end());
    //prints frequency vector
	/*print_word_freq(freq);
	int size=freq.size();
	int ** dp=new int*[size];
    int ** root=new int*[size];
    for(int i=0;i<freq.size();i++)
	{
    	dp[i]=new int[size];
    	root[i]=new int[size];
    }
    build_cost_matrix(freq,dp,root);*/
    node * start=NULL;
    int k, space=0, height=10;
    
    while(1)
 	{
 		cout<<endl<<endl<<"===================================================================="<<endl;
 		cout<<"\n1.Construct tree\n2.Display tree\n3.Search Word\n4.Insert word\n5.Print word and freq\n6.Exit\n\n\n Enter no: ";
 		cin>>k;
 		switch(k)
 		{
 			case 1:
 				{
 					int size=freq.size();
					int ** dp=new int*[size];
    				int ** root=new int*[size];
    				for(int i=0;i<freq.size();i++)
					{
    					dp[i]=new int[size];
    					root[i]=new int[size];
    				}
    				build_cost_matrix(freq,dp,root);
 					start=build_opt_bst(freq,dp,root,0,freq.size()-1,1);
 				}
 				break;
 			case 2:
 				//printOBST(start);
 				printBinaryTree(start, space, height);
 				break;
 			case 3:
 			{	
 				cout<<"Enter Word to search: ";
    			string str;
    			cin>>str;
   	 			transform(str.begin(), str.end(), str.begin(), ::tolower);
    			searchOBST(start,str);	//search word
 			}
 			break;
 			case 4:
 			{
				string w, m;
				int f;
 				cout<<"Enter a word: ";
 				cin>>w;
 				cout<<"Enter its meaning: ";
 				cin>>m;
 				cout<<"Enter its frequency: ";
 				cin>>f;
 				transform(word.begin(), word.end(), word.begin(), ::tolower);
				if(!(find_if(w.begin(), w.end(), ::isdigit) != w.end()))
				for(int i=0; i<f; i++)  
        			insert(freq,w);
        		sort(freq.begin(), freq.end());
    			//prints frequency vector
				print_word_freq(freq);
				int size=freq.size();
				int ** dp=new int*[size];
   		 		int ** root=new int*[size];
    			for(int i=0;i<freq.size();i++)
				{
    				dp[i]=new int[size];
    				root[i]=new int[size];
   	 			}
    			build_cost_matrix(freq,dp,root);
 				
 			}
 			break;
 			case 5:
			 	print_word_freq(freq);	
			 	break;
 			case 6:
	 			exit(0);
 				break;
 		}
 	}
return 0;
}
