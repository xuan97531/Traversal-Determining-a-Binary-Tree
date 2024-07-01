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
	建立新節點(BST)
*/
struct BSTNode *NewBSTNode(int x){
	struct BSTNode*node = new struct BSTNode;
	node->data = x;
	node->leftchild = node->rightchild = NULL;
	return node;
}

//---------------------------------------------------------------------------
/*
	建立新節點(Stack BST)
*/
struct StackNode *NewStackNode(struct BSTNode *Bnode,struct StackNode *top){
	struct StackNode*node = new struct StackNode;
	node->treenode = Bnode;
	node->next = top;
	return node;
}
//---------------------------------------------------------------------------
/*
	建立新節點(Queue BST)
*/
struct QueueNode *NewQueueNode(struct BSTNode *Bnode){
	struct QueueNode*node = new struct QueueNode;
	node->treenode = Bnode;
	node->next = NULL;
	return node;
}
//---------------------------------------------------------------------------
/*
	<非遞迴>搜尋  p => root
	 1. 當 p != NULL (while)
		(1) 搜尋目標 x == p->data => return 1 表示搜尋成功
		(2) 搜尋目標 x  < p->data => p 移動至 p->leftchild (左子樹)
		(3) 搜尋目標 x  > p->data => p 移動至 p->rightchild (右子樹)
	 2. 當迴圈結束 or p == NULL => 表示搜尋未果 or BST為空的 => return 0

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
	<遞迴>搜尋  node => root
	 1. 當 node == NULL => 表示BST為空的 => return 0
	 2. 當搜尋目標 x == p->data => return 1 表示搜尋成功
		當搜尋目標 x  < p->data => return  RSearchBST(node->leftchild,x)(左子樹)
		當搜尋目標 x  > p->data => return  RSearchBST(node->rightchild,x)(右子樹)

*/
int RSearchBST(struct BSTNode*node,int x){
	if(node == NULL) return 0;
	else if(x == node->data) return 1;
	else if(x < node->data) return RSearchBST(node->leftchild,x);
	else return RSearchBST(node->rightchild,x);
}
//---------------------------------------------------------------------------
/*
	<遞迴>新增  node => root
	1. 當 node == NULL => 表示BST為空的 => return 建立含目標資料X的新節點
	2. 當目標 x  < p->data => node->leftchild = RInsertBST(node->leftchild,x)(更新左子樹)
	   當目標 x  > p->data => node->rightchild = RSearchBST(node->rightchild,x)(更新右子樹)
	3. return 更新後的BST

*/
struct BSTNode* RInsertBST(struct BSTNode* node,int x){
	if(node == NULL) return NewBSTNode(x);
	else if(x < node->data) node->leftchild = RInsertBST(node->leftchild,x);
	else node->rightchild = RInsertBST(node->rightchild,x);
	return node;
}
//---------------------------------------------------------------------------
/*
	<非遞迴>新增  p => root , q => NULL
	1. 當 p != NULL(while) => 為找到待新增處
	   (1) q = p => q 紀錄 p 的父節點
	   (2) 當目標 x == p->data => return 0 => 表示BST中已有目標 x 不需再新增
	   (3) 當目標 x  < p->data => p 移動至 p->leftchild (左子樹)
	   (4) 當目標 x  > p->data => p 移動至 p->rightchild (右子樹)
	2.  p = 含目標 x 的新節點
	3. 當 root == NULL => BST 為空的 => root = p (新增x在root)
	4. 當目標 x < q->data => 需新增 p 在 q 的左子樹
	5. 當目標 x > q->data => 需新增 p 在 q 的右子樹
	6. return 1 => 表示新增成功

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
	<遞迴>新增 n 個亂數
	1. count => 紀錄成功新增亂數的個數 , list => 紀錄成功新增的亂數
	2. count < n (while)
	   (1) 產生亂數 x (rangemin ~ rangemax間)
	   (2) 若 root == NULL(BST為空) or BST未有重複的
		   <1> (遞迴)新增 x
		   <2>  list 紀錄 x
		   <3>  count++
	3. return 新增完成後的 root

*/
struct BSTNode* RInsertRandom(int n,int rangemin,int rangemax){
	 int count=0;
	 String list=" => ";
	 Form1->Memo1->Lines->Add("-----(遞迴)新增亂數成功("+IntToStr(rangemin)+" ~ "+IntToStr(rangemax)+")------");
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
	<非遞迴>新增 n 個亂數
	1. count => 紀錄成功新增亂數的個數 , list => 紀錄成功新增的亂數
	2. count < n (while)
	   (1) 產生亂數 x (rangemin ~ rangemax間)
	   (2) 若 root == NULL(BST為空) or BST未有重複的
		   <1> (非遞迴)新增 x
		   <2>  list 紀錄 x
		   <3>  count++
	3. return 新增完成後的 root

*/
struct BSTNode* InsertRandom(int n,int rangemin,int rangemax){
	 int count=0;
	 String list=" => ";
	 Form1->Memo1->Lines->Add("-----(非遞迴)新增亂數成功("+IntToStr(rangemin)+" ~ "+IntToStr(rangemax)+")------");
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
	 新增node進Stack(BST)
*/
void push(struct BSTNode*node){
	 struct StackNode *oldtop;
	 oldtop = top;
	 top = NewStackNode(node,oldtop);
}
//---------------------------------------------------------------------------
/*
	 新增node進Queue(BST)
*/
void AddQ(struct BSTNode*node){
	 struct QueueNode*Qnode = NewQueueNode(node);
	 if(front == NULL) front = Qnode;
	 else rear->next = Qnode;
	 rear = Qnode;
}
//---------------------------------------------------------------------------
/*
	尋找右子樹中最左樹葉
	node => 待搜尋的右子樹 , p => 紀錄 node 的最左樹葉
*/
struct BSTNode* Right_Left(struct BSTNode*node){
	   struct BSTNode *p;
	   for(p = node; p->leftchild!=NULL; p = p->leftchild);
	   return p;
}
//---------------------------------------------------------------------------
/*
	<遞迴>刪除
	待刪除節點
		1. 樹葉(214~219,221)
				=> t 紀錄待刪除節點 => 待刪除節點 = NULL => 直接刪除 t
		2. 只有一子樹(214-215,220-221)
				=> t 紀錄待刪除節點的子樹根
				=> t 所指的整個子樹取代待刪除節點位置
				=> 刪除 t
		3. 兩個子樹(225~229)
				=> t 紀錄待刪除節點右子樹最左樹葉
				=> 待刪除節點->data 替換成 t->data
				=> 刪除待刪除節點右子樹中 t->data 的節點


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
	<非遞迴>刪除

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
	 從Stack中刪除node(BST)
*/
struct BSTNode* pop(){
	   struct BSTNode *Broot;
	   struct StackNode *oldtop;
	   if(top == NULL) Form1->Memo1->Lines->Add("Stack中空空如也~~ 別再刪了!!!");
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
	 從Queue中刪除node(BST)
*/
struct BSTNode* DeleteQ(){
	   struct BSTNode *Broot;
	   struct QueueNode *oldfront;
	   if(rear == NULL) Form1->Memo1->Lines->Add("Queue中空空如也~~ 別再刪了!!!");
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
	<遞迴>中序走訪(LVR)

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
	<非遞迴>中序走訪(LVR) --> 利用stack

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
	<遞迴>前序走訪(VLR)

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
	<非遞迴>前序走訪(VLR) --> 利用stack

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
	<遞迴>後序走訪(LRV)

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
	 新增data進stack(矩陣)
*/
void push_data(int data){
	 if(top_data == max_size-1) Form1->Memo1->Lines->Add("已經滿了\n沒法再加了!!!");
	 else stack[++top_data] = data;
}
//---------------------------------------------------------------------------
/*
	 從stack中刪除data(矩陣)
*/
int pop_data(){
	 if(top_data == -1) Form1->Memo1->Lines->Add("空了~空了~\n沒東西給你刪了!!!");
	 else return stack[top_data--];
}
//---------------------------------------------------------------------------
/*
	<非遞迴>後序走訪(LRV) --> 利用stack

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
	<遞迴>階層走訪

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
	尋找字元位置
*/
int AnsiPosition(char c,AnsiString target){
	for(int i=1;i<=target.Length();i++)
		if(c == target[i]) return i;
	return -1;
}
//---------------------------------------------------------------------------
/*
	中序、前序決定唯一二元樹
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
	中序、後序決定唯一二元樹
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
	<遞迴>清空BST
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
		Memo1->Lines->Add("-----------(遞迴)新增成功 => 已新增 "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(遞迴)新增失敗 => BST已存在"+IntToStr(data));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
     int data = StrToInt(Edit1->Text);
	 int suc = InsertBST(data);
	 if(suc){
		tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(非遞迴)新增成功 => 已新增 "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(非遞迴)新增失敗 => BST已存在"+IntToStr(data));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 if(SearchBST(data)) Memo1->Lines->Add("(非遞迴)搜尋成功 => "+IntToStr(data));
	 else{
		// 搜尋未果，新增其也
		if(CheckBox2->Checked){
		   Memo1->Lines->Add("(非遞迴)搜尋失敗 => "+IntToStr(data)+"不在BST中 => (非遞迴)新增進BST");
		   InsertBST(data);
           tree = "";
		   printBST_infix(root);
		   Memo1->Lines->Add("BST(infix) => "+tree);
		}
		else Memo1->Lines->Add("(非遞迴)搜尋失敗 => "+IntToStr(data)+"不在BST中");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 if(RSearchBST(root,data))  Memo1->Lines->Add("(遞迴)搜尋成功 => "+IntToStr(data));
	 else{
		// 搜尋未果，新增其也
        if(CheckBox2->Checked){
		   Memo1->Lines->Add("(遞迴)搜尋失敗 => "+IntToStr(data)+"不在BST中 => (遞迴)新增進BST");
		   root = RInsertBST(root,data);
           tree = "";
		   printBST_infix(root);
		   Memo1->Lines->Add("BST(infix) => "+tree);
		}
		else Memo1->Lines->Add("(遞迴)搜尋失敗 => "+IntToStr(data)+"不在BST中");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 // 先判斷目標data 是否存在於BST
	 if(SearchBST(data)){
		root = RDeleteBST(root,data);
		tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(遞迴)刪除成功 => 已刪除 "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(遞迴)刪除失敗 => BST中不存在"+IntToStr(data));

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	 int data = StrToInt(Edit1->Text);
	 int suc = DeleteBST(data);
	 if(suc){
        tree = "";
		printBST_infix(root);
		Memo1->Lines->Add("-----------(非遞迴)刪除成功 => 已刪除 "+IntToStr(data)+"--------");
		Memo1->Lines->Add("BST(infix) => "+tree);
	 }
	 else Memo1->Lines->Add("(非遞迴)刪除失敗 => BST中不存在"+IntToStr(data));
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
		Memo1->Lines->Add("前序(prefix)走訪BST(遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_prefix_stack(root);
		Memo1->Lines->Add("前序(prefix)走訪BST(非遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	 tree = "";
	 if(CheckBox3->Checked){
		printBST_infix(root);
		Memo1->Lines->Add("中序(infix)走訪BST(遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_infix_stack(root);
		Memo1->Lines->Add("中序(prefix)走訪BST(非遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
	 tree = "";
	 if(CheckBox3->Checked){
		printBST_postfix(root);
		Memo1->Lines->Add("後序(prefix)走訪BST(遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
	 else{
		printBST_postfix_stack(root);
		Memo1->Lines->Add("後序(prefix)走訪BST(非遞迴)------------------------");
		Memo1->Lines->Add("=> "+tree);
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button14Click(TObject *Sender)
{
	 tree = "";
	 printBST_levelfix(root);
	 Memo1->Lines->Add("階層(level)走訪BST(Queue)------------------------");
	 Memo1->Lines->Add("=> "+tree);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button11Click(TObject *Sender)
{
	 ClearBST(root);
	 Memo1->Lines->Add("已清除BST");
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
	 if(root == NULL) Memo2->Lines->Add("(失敗)無法決定二元樹");
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
	 if(root == NULL) Memo2->Lines->Add("(失敗)無法決定二元樹");
	 Memo2->Lines->Add("==============================================");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button18Click(TObject *Sender)
{
    ClearBST(root);
	Memo1->Lines->Add("已清除BST");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender)
{
	 Memo2->Clear();
}
//---------------------------------------------------------------------------

