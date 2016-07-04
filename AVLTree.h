#include <iostream>
#include <queue>
#include <stdlib.h>

using namespace std;

template <class K, class V>
//三叉链形式
struct AVLTreeNode
{
	typedef AVLTreeNode<K, V> Node;

	K _key;
	V _value;
	int _bf;		//平衡因子
	Node* _left;	//左孩子
	Node* _right;	//右孩子
	Node* _parent;	//父亲

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		,_value(value)
		,_bf(0)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}
};

template <class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}
public:
	bool Insert(const K& key, const V& value)
	{
		if(_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		Node* cur = _root;
		Node* prev = NULL;
		while(cur)
		{
			if(key > cur->_key) //key大，走右子树
			{
				prev = cur;
				cur = cur->_right;
			}
			else if(key < cur->_key) //key小，走左子树
			{
				prev = cur;
				cur = cur->_left;
			}
			else
			{
				return false;	//有存在的元素，结束，插入失败
			}
		}
		//插入新节点
		//cur就是要插入的位置
		cur = new Node(key, value);
		cur->_parent = prev;
	
		//if(cur->_key < key)
		if(prev->_key >key)
		{
			prev->_left = cur;

		}
		else
		{
			prev->_right = cur;
		}

		//更新平衡因子
		Node* parent = cur->_parent;
		while(parent)
		{
			//更新平衡因子的值
			if(parent->_left == cur)
			{
				parent->_bf--;	//往左子树走
			}
			else
			{
				parent->_bf++;	//往右子树走
			}
			
			if(parent->_bf == 0)
			{
				break;
			}
			else if(parent->_bf == -1 || parent->_bf == 1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			else
			{
				//调整，使其再次成为AVL树
				if(parent->_bf == 2)
				{
					if(cur->_bf == 1)
					{
						_RotateL(parent); //左单旋，bf(2,1)
					}
					else
					{
						_RotateRL(parent);//右左双旋,bf(2,-1)
					}
				}
				else
				{
					if(cur->_bf == -1)
					{
						_RotateR(parent);//右单旋，bf(-2,-1)
					}
					else
					{
						_RotateLR(parent);//左右双旋,bf(-2,1)
					}
				}
				break;
			}
		}

	}
	int Height()
	{
		return _Height(_root);
	}
	bool IsBalance()
	{
		return _IsBalance(_root);	
	}
	void LevelOrder()
	{
		_LevelOrder(_root);
		cout<<endl;
	}
	void InOrder()
	{
		_InOrder(_root);
		cout<<endl;
	}
	
protected:
	int _Height(Node* root)
	{
		if(root == NULL)
		{
			return 0;
		}
		int left = _Height(root->_left);
		int right = _Height(root->_right);
		return left > right ? left+1 : right+1;
	}
	bool _IsBalance(Node* root)
	{
		if(root == NULL)
		{
			return true;
		}
		if(abs(_Height(root->_right) - _Height(root->_left)) > 1 || abs(root->_bf) > 1)
		{
			cout<<"false: "<<root->_key<<endl;
			return false;
		}
		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}
	void _LevelOrder(Node* root)
	{
		if(root == NULL)
		{
			return;
		}
		queue<Node*> q;
		q.push(root);
		while(!q.empty())
		{
			if(q.front()->_left != NULL)
			{
				q.push(q.front()->_left);
			}
			if(q.front()->_right != NULL)
			{
				q.push(q.front()->_right);
			}
			cout<<q.front()->_key<<" ";
			q.pop();
		}
	}
	void _InOrder(Node* root)
	{
		if(root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout<<root->_key<<" ";
		_InOrder(root->_right);
	}
	void _RotateL(Node* &parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if(subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subR;

		subR->_parent = ppNode;
		if(ppNode == NULL)
		{
			_root = subR;
		}
		else
		{
			if(ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
		}
		parent->_bf = subR->_bf = 0;
		
		//parent=subR;//
	}
	void _RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		
		parent->_left = subLR;
		if(subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		subL->_parent = ppNode;
		if(ppNode == NULL)
		{
			_root = subL;
		}
		else
		{
			if(ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}
		}
		//更新平衡因子
		parent->_bf = subL->_bf = 0;
		//parent=subL;
	}
	void _RotateRL(Node* &parent)
	{
		Node* sub = parent;
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		_RotateR(parent->_right);
		_RotateL(parent);

		if(bf == 1)
		{
			sub->_bf = -1;
		}
		else
		{
			subR->_bf = 1; 
		}
	}
	void _RotateLR(Node* &parent)
	{
		Node* sub = parent;
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;	

		_RotateL(parent->_left);
		_RotateR(parent);

		if(bf == -1)
		{
			sub->_bf = 1;
		}
		else
		{
			subL->_bf = -1;
		}
	}
private:
	Node* _root;
};


void Test()
{
	int arr[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
	AVLTree<int, int> at;
	for(int i = 0; i<sizeof(arr)/sizeof(arr[0]); ++i)
	{
		at.Insert(arr[i], i);
	}
	at.LevelOrder();
	at.InOrder();
	cout<<"IsBalance? "<<at.IsBalance()<<endl;
	cout<<at.Height()<<endl;
}
