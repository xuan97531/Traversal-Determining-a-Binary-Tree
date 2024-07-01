//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
#include "gifimage.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define max_size 1000000
TForm1 *Form1;


struct BSTNode{
	struct BSTNode *leftchild;
	int data;
	struct BSTNode *rightchild;
};

struct DBSTNode{
	struct DBSTNode *leftchild;
	String data;
	int index;
	struct DBSTNode *rightchild;
};

struct StackNode{
	struct BSTNode *treenode;
	struct StackNode *next;
};

struct QueueNode{
	struct BSTNode *treenode;
	struct QueueNode *next;
};

struct BSTNode *root;
struct DBSTNode *Droot;
struct StackNode *top;
struct QueueNode *front,*rear;
String tree;
int stack[max_size];
int top_data = -1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	 ComboBox1->Items->Add("HBJAFDGCE");
	 ComboBox1->Items->Add("BCAEDGHFI");
	 ComboBox1->Items->Add("ADEBCGHJIF");

	 ComboBox2->Items->Add("ABHJCDFGE");
	 ComboBox2->Items->Add("ABCDEFGHI");
	 ComboBox2->Items->Add("CABDEFGHIJ");

	 ComboBox3->Items->Add("HJBFGDECA");
	 ComboBox3->Items->Add("CBEHGIFDA");
	 ComboBox3->Items->Add("EDBAJIHGFC");
}
//---------------------------------------------------------------------------
/*
	�إ߷s�`�I(BST)
*/
struct BSTNode *NewBSTNode(int x){
	struct BSTNode*node = new struct BSTNode;
	node->data = x;
	node->leftchild = node->rightchild = NULL;
	return node;
}

//---------------------------------------------------------------------------
/*
	�إ߷s�`�I(Stack BST)
*/
struct StackNode *NewStackNode(struct BSTNode *Bnode,struct StackNode *top){
	struct StackNode*node = new struct StackNode;
	node->treenode = Bnode;
	node->next = top;
	return node;
}
//---------------------------------------------------------------------------
/*
	�إ߷s�`�I(Queue BST)
*/
struct QueueNode *NewQueueNode(struct BSTNode *Bnode){
	struct QueueNode*node = new struct QueueNode;
	node->treenode = Bnode;
	node->next = NULL;
	return node;
}
//---------------------------------------------------------------------------
/*
	<�D���j>�j�M  p => root
	 1. �� p != NULL (while)
		(1) �j�M�ؼ� x == p->data => return 1 ��ܷj�M���\
		(2) �j�M�ؼ� x  < p->data => p ���ʦ� p->leftchild (���l��)
		(3) �j�M�ؼ� x  > p->data => p ���ʦ� p->rightchild (�k�l��)
	 2. ��j�鵲�� or p == NULL => ��ܷj�M���G or BST���Ū� => return 0

*/
int SearchBST(int x){
	struct BSTNode *p;
	p = root;
	while(p!=NULL){
	   if(x == p->data) return 1;
	   else if(x < p->data) p = p->leftchild;
	   else p = p->rightchild;
	}
	return 0;
}
//---------------------------------------------------------------------------
/*
	<���j>�j�M  node => root
	 1. �� node == NULL => ���BST���Ū� => return 0
	 2. ��j�M�ؼ� x == p->data => return 1 ��ܷj�M���\
		��j�M�ؼ� x  < p->data => return  RSearchBST(node->leftchild,x)(���l��)
		��j�M�ؼ� x  > p->data => return  RSearchBST(node->rightchild,x)(�k�l��)

*/
int RSearchBST(struct BSTNode*node,int x){
	if(node == NULL) return 0;
	else if(x == node->data) return 1;
	else if(x < node->data) return RSearchBST(node->leftchild,x);
	else return RSearchBST(node->rightchild,x);
}
//---------------------------------------------------------------------------
/*
	<���j>�s�W  node => root
	1. �� node == NULL => ���BST���Ū� => return �إߧt�ؼи��X���s�`�I
	2. ��ؼ� x  < p->data => node->leftchild = RInsertBST(node->leftchild,x)(��s���l��)
	   ��ؼ� x  > p->data => node->rightchild = RSearchBST(node->rightchild,x)(��s�k�l��)
	3. return ��s�᪺BST

*/
struct BSTNode* RInsertBST(struct BSTNode* node,int x){
	if(node == NULL) return NewBSTNode(x);
	else if(x < node->data) node->leftchild = RInsertBST(node->leftchild,x);
	else node->rightchild = RInsertBST(node->rightchild,x);
	return node;
}
//---------------------------------------------------------------------------
/*
	<�D���j>�s�W  p => root , q => NULL
	1. �� p != NULL(while) => �����ݷs�W�B
	   (1) q = p => q ���� p �����`�I
	   (2) ��ؼ� x == p->data => return 0 => ���BST���w���ؼ� x ���ݦA�s�W
	   (3) ��ؼ� x  < p->data => p ���ʦ� p->leftchild (���l��)
	   (4) ��ؼ� x  > p->data => p ���ʦ� p->rightchild (�k�l��)
	2.  p = �t�ؼ� x ���s�`�I
	3. �� root == NULL => BST ���Ū� => root = p (�s�Wx�broot)
	4. ��ؼ� x < q->data => �ݷs�W p �b q �����l��
	5. ��ؼ� x > q->data => �ݷs�W p �b q ���k�l��
	6. return 1 => ��ܷs�W���\

*/
int InsertBST(int x){
	struct BSTNode *p,*q;
	p = root, q = NULL;
	while(p!=NULL){
	   q = p;
	   if(x == p->data) return 0;
	   else if(x < p->data) p = p->leftchild;
	   else p = p->rightchild;
	}
	p = NewBSTNode(x);
	if(root == NULL) root = p;
	else if(x < q->data) q->leftchild = p;
	else q->rightchild = p;
	return 1;
}
//---------------------------------------------------------------------------
/*
	<���j>�s�W n �Ӷü�
	1. count => �������\�s�W�üƪ��Ӽ� , list => �������\�s�W���ü�
	2. count < n (while)
	   (1) ���Ͷü� x (rangemin ~ rangemax��)
	   (2) �Y root == NULL(BST����) or BST�������ƪ�
		   <1> (���j)�s�W x
		   <2>  list ���� x
		   <3>  count++
	3. return �s�W�����᪺ root

*/
struct BSTNode* RInsertRandom(int n,int rangemin,int rangemax){
	 int count=0;
	 String list=" => ";
	 Form1->Memo1->Lines->Add("-----(���j)�s�W�üƦ��\("+IntToStr(rangemin)+" ~ "+IntToStr(rangemax)+")------");
	 while(count < n){
		int x = rand()%rangemax+rangemin;
		if(root == NULL || !(SearchBST(x))){
		   root = RInsertBST(root,x);
		   list+=IntToStr(x)+"  ";
		   count++;
		}
	 }
	 Form1->Memo1->Lines->Add(list);
	 return root;
}
//---------------------------------------------------------------------------
/*
	<�D���j>�s�W n �Ӷü�
	1. count => �������\�s�W�üƪ��Ӽ� , list => �������\�s�W���ü�
	2. count < n (while)
	   (1) ���Ͷü� x (rangemin ~ rangemax��)
	   (2) �Y root == NULL(BST����) or BST�������ƪ�
		   <1> (�D���j)�s�W x
		   <2>  list ���� x
		   <3>  count++
	3. return �s�W�����᪺ root

*/
struct BSTNode* InsertRandom(int n,int rangemin,int rangemax){
	 int count=0;
	 String list=" => ";
	 Form1->Memo1->Lines->Add("-----(�D���j)�s�W�üƦ��\("+IntToStr(rangemin)+" ~ "+IntToStr(rangemax)+")------");
	 while(count < n){
		int x = rand()%rangemax+rangemin;
		if(InsertBST(x)){
		   list+=IntToStr(x)+"  ";
		   count++;
		}
	 }
	 Form1->Memo1->Lines->Add(list);
	 return root;
}

//---------------------------------------------------------------------------
/*
	 �s�Wnode�iStack(BST)
*/
void push(struct BSTNode*node){
	 struct StackNode *oldtop;
	 oldtop = top;
	 top = NewStackNode(node,oldtop);
}
//---------------------------------------------------------------------------
/*
	 �s�Wnode�iQueue(BST)
*/
void AddQ(struct BSTNode*node){
	 struct QueueNode*Qnode = NewQueueNode(node);
	 if(front == NULL) front = Qnode;
	 else rear->next = Qnode;
	 rear = Qnode;
}
//---------------------------------------------------------------------------
/*
	�M��k�l�𤤳̥���
	node => �ݷj�M���k�l�� , p => ���� node ���̥���
*/
struct BSTNode* Right_Left(struct BSTNode*node){
	   struct BSTNode *p;
	   for(p = node; p->leftchild!=NULL; p = p->leftchild);
	   return p;
}
//---------------------------------------------------------------------------
/*
	<���j>�R��
	�ݧR���`�I
		1. ��(214~219,221)
				=> t �����ݧR���`�I => �ݧR���`�I = NULL => �����R�� t
		2. �u���@�l��(214-215,220-221)
				=> t �����ݧR���`�I���l���
				=> t �ҫ�����Ӥl����N�ݧR���`�I��m
				=> �R�� t
		3. ��Ӥl��(225~229)
				=> t �����ݧR���`�I�k�l��̥���
				=> �ݧR���`�I->data ������ t->data
				=> �R���ݧR���`�I�k�l�� t->data ���`�I


*/
struct BSTNode* RDeleteBST(struct BSTNode*node,int x){
	   struct BSTNode *t;
	   if(x < node->data) node->leftchild = RDeleteBST(node->leftchild,x);
	   else if(x > node->data) node->rightchild = RDeleteBST(node->rightchild,x);
	   else{
		  if((node->leftchild == NULL) || (node->rightchild == NULL)){
			 t = (node->leftchild) ? node->leftchild : node->rightchild;
			 if(t == NULL){
				t = node;
				node = NULL;
			 }
			 else *node = *t;
			 free(t);
		  }
		  else{
			 t = Right_Left(node->rightchild);
			 node->data = t->data;
			 node->rightchild = RDeleteBST(node->rightchild,t->data);
		  }
	   }
	   return node;

}
//---------------------------------------------------------------------------
/*
	<�D���j>�R��

*/
int DeleteBST(int x){
	struct BSTNode *p, *pf, *q, *qf;
	p = root; pf = NULL;
	while(p!=NULL){
	   if(x == p->data){
		  if((p->leftchild == NULL) && (p->rightchild == NULL)) q = NULL;
		  else if(p->leftchild!=NULL){
			 qf = p;
			 q  = p->leftchild;
			 while(q->rightchild!=NULL){
				qf = q;
				q  = q->rightchild;
			 }
			 if(p == qf) qf->leftchild = q->leftchild;
			 else qf->rightchild = q->leftchild;
		  }
		  else{
			 qf = p;
			 q  = p->rightchild;
			 while(q->leftchild!=NULL){
				qf = q;
				q  = q->leftchild;
			 }
			 if(p == qf) qf->rightchild = q->rightchild;
			 else qf->leftchild = q->rightchild;
		  }
		  if(q!=NULL){
			 q->leftchild = p->leftchild;
			 q->rightchild = p->rightchild;
		  }
		  if(pf == NULL) root = q;
		  else{
			 if(x < pf->data) pf->leftchild = q;
			 else pf->rightchild = q;
		  }
		  free(p);
		  return 1;
	   }
	   else{
		  pf = p;
		  if(x < p->data) p = p->leftchild;
		  else p = p->rightchild;
	   }
	}
	return 0;
}
//---------------------------------------------------------------------------
/*
	 �qStack���R��node(BST)
*/
struct BSTNode* pop(){
	   struct BSTNode *Broot;
	   struct StackNode *oldtop;
	   if(top == NULL) Form1->Memo1->Lines->Add("Stack���ŪŦp�]~~ �O�A�R�F!!!");
	   else{
		  Broot = top->treenode;
		  oldtop = top;
		  top = top->next;
		  free(oldtop);
		  return Broot;
	   }
}
//---------------------------------------------------------------------------
/*
	 �qQueue���R��node(BST)
*/
struct BSTNode* DeleteQ(){
	   struct BSTNode *Broot;
	   struct QueueNode *oldfront;
	   if(rear == NULL) Form1->Memo1->Lines->Add("Queue���ŪŦp�]~~ �O�A�R�F!!!");
	   else{
		  Broot = front->treenode;
		  oldfront = front;
		  front = front->next;
		  free(oldfront);
		  return Broot;
	   }
}
//---------------------------------------------------------------------------
/*
	<���j>���Ǩ��X(LVR)

*/
void printBST_infix(struct BSTNode*node){
	 if(node!=NULL){
		printBST_infix(node->leftchild);
		tree += IntToStr(node->data) + " - ";
		printBST_infix(node->rightchild);
	 }
}
//---------------------------------------------------------------------------
/*
	<�D���j>���Ǩ��X(LVR) --> �Q��stack

*/
void printBST_infix_stack(struct BSTNode*node){
     do{
	   while(node!=NULL){
		  push(node);
		  node = node->leftchild;
	   }
	   if(top!=NULL){
		  node = pop();
		  tree += IntToStr(node->data) + " - ";
		  node = node->rightchild;
	   }
	 }
     while((top!=NULL)||(node!=NULL));
}
//---------------------------------------------------------------------------
/*
	<���j>�e�Ǩ��X(VLR)

*/
void printBST_prefix(struct BSTNode*node){
	 if(node!=NULL){
		tree += IntToStr(node->data) + " - ";
		printBST_prefix(node->leftchild);
        printBST_prefix(node->rightchild);
	 }
}
//---------------------------------------------------------------------------
/*
	<�D���j>�e�Ǩ��X(VLR) --> �Q��stack

*/
void printBST_prefix_stack(struct BSTNode*node){
	 do{
	   while(node!=NULL){
		  tree += IntToStr(node->data) + " - ";
		  push(node);
		  node = node->leftchild;
	   }
	   if(top!=NULL){
		  node = pop();
		  node = node->rightchild;
	   }
	 }
	 while((top!=NULL)||(node!=NULL));
}
//---------------------------------------------------------------------------
/*
	<���j>��Ǩ��X(LRV)

*/
void printBST_postfix(struct BSTNode*node){
	 if(node!=NULL){
		printBST_postfix(node->leftchild);
		printBST_postfix(node->rightchild);
		tree += IntToStr(node->data) + " - ";
	 }
}
//---------------------------------------------------------------------------
/*
	 �s�Wdata�istack(�x�})
*/
void push_data(int data){
	 if(top_data == max_size-1) Form1->Memo1->Lines->Add("�w�g���F\n�S�k�A�[�F!!!");
	 else stack[++top_data] = data;
}
//---------------------------------------------------------------------------
/*
	 �qstack���R��data(�x�})
*/
int pop_data(){
	 if(top_data == -1) Form1->Memo1->Lines->Add("�ŤF~�ŤF~\n�S�F�赹�A�R�F!!!");
	 else return stack[top_data--];
}
//---------------------------------------------------------------------------
/*
	<�D���j>��Ǩ��X(LRV) --> �Q��stack

*/
void printBST_postfix_stack(struct BSTNode*node){
	 do{
	   while(node!=NULL){
		  push_data(node->data);
		  push(node);
		  node = node->rightchild;
	   }
	   if(top!=NULL){
		  node = pop();
		  node = node->leftchild;
	   }
	 }
	 while((top!=NULL)||(node!=NULL));
	 while(top_data!=-1) tree += IntToStr(pop_data()) + " - ";
}
//---------------------------------------------------------------------------
/*
	<���j>���h���X

*/
void printBST_levelfix(struct BSTNode*node){
	 if(node!=NULL){
		AddQ(node);
		while(front!=NULL){
		  node = DeleteQ();
		  tree += IntToStr(node->data) + " - ";
		  if(node->leftchild!=NULL) AddQ(node->leftchild);
		  if(node->rightchild!=NULL) AddQ(node->rightchild);
		}
	 }
}
//---------------------------------------------------------------------------
/*
	�M��r����m
*/
int AnsiPosition(char c,AnsiString target){
	for(int i=1;i<=target.Length();i++)
		if(c == target[i]) return i;
	return -1;
}
//---------------------------------------------------------------------------
/*
	���ǡB�e�ǨM�w�ߤ@�G����
*/
struct BSTNode* infix_prefix_BT(AnsiString infix,AnsiString prefix){
	   int k;
	   struct BSTNode*node;
	   if(infix.Length() == 0) return NULL;
	   node = NewBSTNode((int)prefix[1]);
	   k = AnsiPosition(prefix[1],infix);
	   if(k!=-1){
		  Form1->Memo2->Lines->Add("left_substree  => "+infix.SubString(1,k-1)+" vs. "+prefix.SubString(2,k-1));
		  node->leftchild = infix_prefix_BT(infix.SubString(1,k-1),prefix.SubString(2,k-1));
		  Form1->Memo2->Lines->Add("right_substree => "+infix.SubString(k+1,infix.Length()-k)+" vs. "+prefix.SubString(k+1,prefix.Length()-k));
		  node->rightchild = infix_prefix_BT(infix.SubString(k+1,infix.Length()-k),prefix.SubString(k+1,prefix.Length()-k));
		  return node;
	   }
	   else return NULL;
}
//---------------------------------------------------------------------------
/*
	���ǡB��ǨM�w�ߤ@�G����
*/
struct BSTNode* infix_postfix_BT(AnsiString infix,AnsiString postfix){
	   int k;
	   struct BSTNode*node;
	   if(infix.Length() == 0) return NULL;
	   node = NewBSTNode((int)postfix[postfix.Length()]);
	   k = AnsiPosition(postfix[postfix.Length()],infix);
	   if(k!=-1){
		  Form1->Memo2->Lines->Add("left_substree  => "+infix.SubString(1,k-1)+" vs. "+postfix.SubString(1,k-1));
		  node->leftchild = infix_postfix_BT(infix.SubString(1,k-1),postfix.SubString(1,k-1));
		  Form1->Memo2->Lines->Add("left_substree  => "+infix.SubString(k+1,infix.Length()-k)+" vs. "+postfix.SubString(k,postfix.Length()-k));
		  node->rightchild = infix_postfix_BT(infix.SubString(k+1,infix.Length()-k),postfix.SubString(k,postfix.Length()-k));
		  return node;
	   }
	   else return NULL;
}
//---------------------------------------------------------------------------
/*
	<���j>�M��BST
*/
void ClearBST(struct BSTNode*node){
	 if(node!=NULL){
		ClearBST(node->leftchild);
		ClearBST(node->rightchild);
		DeleteBST(node->data);
	 }
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 if(root == NULL || !(SearchBST(data))){
		root = RInsertBST(root,data);
		tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(���j)�s�W���\ => �w�s�W "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(���j)�s�W���� => BST�w�s�b"+IntToStr(data));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
     int data = StrToInt(Edit1->Text);
	 int suc = InsertBST(data);
	 if(suc){
		tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(�D���j)�s�W���\ => �w�s�W "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(�D���j)�s�W���� => BST�w�s�b"+IntToStr(data));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 if(SearchBST(data)) Memo1->Lines->Add("(�D���j)�j�M���\ => "+IntToStr(data));
	 else{
		// �j�M���G�A�s�W��]
		if(CheckBox2->Checked){
		   Memo1->Lines->Add("(�D���j)�j�M���� => "+IntToStr(data)+"���bBST�� => (�D���j)�s�W�iBST");
		   InsertBST(data);
           tree = "";
		   printBST_infix(root);
		   Memo1->Lines->Add("BST(infix) => "+tree);
		}
		else Memo1->Lines->Add("(�D���j)�j�M���� => "+IntToStr(data)+"���bBST��");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 if(RSearchBST(root,data))  Memo1->Lines->Add("(���j)�j�M���\ => "+IntToStr(data));
	 else{
		// �j�M���G�A�s�W��]
        if(CheckBox2->Checked){
		   Memo1->Lines->Add("(���j)�j�M���� => "+IntToStr(data)+"���bBST�� => (���j)�s�W�iBST");
		   root = RInsertBST(root,data);
           tree = "";
		   printBST_infix(root);
		   Memo1->Lines->Add("BST(infix) => "+tree);
		}
		else Memo1->Lines->Add("(���j)�j�M���� => "+IntToStr(data)+"���bBST��");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 // ���P�_�ؼ�data �O�_�s�b��BST
	 if(SearchBST(data)){
		root = RDeleteBST(root,data);
		tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(���j)�R�����\ => �w�R�� "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(���j)�R������ => BST�����s�b"+IntToStr(data));

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 int suc = DeleteBST(data);
	 if(suc){
        tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(�D���j)�R�����\ => �w�R�� "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(�D���j)�R������ => BST�����s�b"+IntToStr(data));
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button9Click(TObject *Sender)
{
	 int n = StrToInt(Edit3->Text);
	 int rangemin = StrToInt(Edit2->Text);
	 int rangemax = StrToInt(Edit4->Text);
	 root = RInsertRandom(n,rangemin,rangemax);
	 tree = "";
	 printBST_infix(root);
	 Memo1->Lines->Add("BST(infix) => "+tree);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
	 int n = StrToInt(Edit3->Text);
	 int rangemin = StrToInt(Edit2->Text);
	 int rangemax = StrToInt(Edit4->Text);
	 root  = InsertRandom(n,rangemin,rangemax);
	 tree = "";
	 printBST_infix(root);
	 Memo1->Lines->Add("BST(infix) => "+tree);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{
	 tree = "";
	 if(CheckBox3->Checked){
		printBST_prefix(root);
		Memo1->Lines->Add("�e��(prefix)���XBST(���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_prefix_stack(root);
		Memo1->Lines->Add("�e��(prefix)���XBST(�D���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	 tree = "";
	 if(CheckBox3->Checked){
		printBST_infix(root);
		Memo1->Lines->Add("����(infix)���XBST(���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_infix_stack(root);
		Memo1->Lines->Add("����(prefix)���XBST(�D���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
	 tree = "";
	 if(CheckBox3->Checked){
		printBST_postfix(root);
		Memo1->Lines->Add("���(prefix)���XBST(���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_postfix_stack(root);
		Memo1->Lines->Add("���(prefix)���XBST(�D���j)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button14Click(TObject *Sender)
{
	 tree = "";
	 printBST_levelfix(root);
	 Memo1->Lines->Add("���h(level)���XBST(Queue)------------------------");
	 Memo1->Lines->Add("=> "+tree);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button11Click(TObject *Sender)
{
	 ClearBST(root);
	 Memo1->Lines->Add("�w�M��BST");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{
	 Memo1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button15Click(TObject *Sender)
{
	 AnsiString infix = ComboBox1->Items->Strings[ComboBox1->ItemIndex];
	 AnsiString prefix = ComboBox2->Items->Strings[ComboBox2->ItemIndex];
	 Memo2->Lines->Add("infix(LVR)  => "+infix);
	 Memo2->Lines->Add("prefix(VLR) => "+prefix);
	 root = infix_prefix_BT(infix,prefix);
	 if(root == NULL) Memo2->Lines->Add("(����)�L�k�M�w�G����");
	 Memo2->Lines->Add("==============================================");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button16Click(TObject *Sender)
{
	 AnsiString infix = ComboBox1->Items->Strings[ComboBox1->ItemIndex];
	 AnsiString postfix = ComboBox3->Items->Strings[ComboBox3->ItemIndex];
	 Memo2->Lines->Add("infix(LVR)  => "+infix);
	 Memo2->Lines->Add("postfix(LRV) => "+postfix);
	 root = infix_postfix_BT(infix,postfix);
	 if(root == NULL) Memo2->Lines->Add("(����)�L�k�M�w�G����");
	 Memo2->Lines->Add("==============================================");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button18Click(TObject *Sender)
{
    ClearBST(root);
	Memo1->Lines->Add("�w�M��BST");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender)
{
	 Memo2->Clear();
}
//---------------------------------------------------------------------------

