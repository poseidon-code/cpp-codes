#include<iostream>	//basic I/O operations
#include<iomanip>	//for setw()
#include<stdlib.h>	//for system("cls")
#include<windows.h>	//for Sleep()

using namespace std;

//linked list CLASS
class LIST
{	
	private:	class NODE
			{	
				public:	int info;
					NODE *link;
					NODE(int a=-1);
					~NODE();
			};
	
 	public:		NODE *hnode;
			LIST();
			void create();
			void insert_beg();
			void insert_specific();
			void insert_end();
 			void delete_beg();
			void delete_specific();
			void delete_end();
			void display();
				
};

//NODE class: parameterised constructor
LIST::NODE::NODE(int a)
{	
	info = a;
	link = NULL;
}

//NODE class: destructor
LIST::NODE::~NODE()
{ }

//LIST class: constructor
LIST::LIST()
{	
	hnode = new NODE;
}



//1st LIST creation function
void LIST::create()
{	
	NODE *pnode = hnode;
	char choice;
	int val;
	while(1)
	{	
		cout<<"\nInsert node ?? (y/n)";
		cin>>choice;
		if(choice == 'n')
			break;	
		else if(choice =='y')
		{	
			cout<<"Enter INFO part of node : ";
			cin>>val;
			NODE *q = new NODE(val);
			pnode->link = q;
			pnode = q;
		}
		else
			cout<<"Invalid choice !"<<endl;
	}
}


//inserting at BEGINNING of LIST
void LIST::insert_beg()
{	
	NODE *q;
	NODE *pnode = hnode;
	int val;
	cout<<"\nEnter the value to be INSERTED: ";
	cin>>val;
	q = new NODE(val);
	q->link = pnode->link;
	pnode->link = q;
	cout<<"\n\n . . . NODE INSERTED"<<endl;
	Sleep(2000);
}


//inserting at SPECIFIED POSITION of LIST
void LIST::insert_specific()
{	
	NODE *ptr;
	NODE *temp;
	int c=1;
	int p;
	int i;
	int val;
	while(1)
	{	
		if(hnode->link == NULL)
		{	
			cout<<"\n\n !! LIST IS EMPTY !!";
			Sleep(1000);
			break;
		}
		else
		{	
			NODE *cptr;
			cptr = hnode->link;
			while(cptr->link)
			{	
				cptr = cptr->link;
				c++;
			}
			while(1)
			{	
				cout<<"\n\nEnter the position : ";
				cin>>p;
				temp = hnode->link;
				if(p == 1)
				{	
					insert_beg();
					break;
				}
				else if(p == 0)
				{	
					cout<<"\n !! POSITION CANNOT BE ZERO !!";
					Sleep(500);						
					continue;
				}
				else if(p>c)
				{	
					cout<<"\n !! POSITION NOT AVAILABLE !!";
					Sleep(500);
					continue;
				}
				else
				{	
					cout<<"\nEnter the value to be INSERTED : ";										
					cin>>val;
					ptr = new NODE(val);
					for(i=1;i<p-1;i++)
						temp = temp->link;
					ptr->link = temp->link;
					temp->link = ptr;
					cout<<"\n\n . . . NODE INSERTED";
					Sleep(2000);
					break;
				}
			}
			break;
		}
	}
}


//inserting at END of LIST
void LIST::insert_end()
{	
	NODE *pnode = hnode;
	NODE *q;
	int val;
	cout<<"\nEnter the value to be INSERTED : ";
	cin>>val;
	q = new NODE(val);
	while(pnode->link)
		pnode = pnode->link;
	pnode->link = q;
	cout<<"\n\n . . . NODE INSERTED";
	Sleep(2000);
}


//deletion at BEGINNING of LIST
void LIST::delete_beg()
{	
	NODE *pnode = hnode->link;
	NODE *dnode;
	if(hnode->link == NULL)
	{	
		cout<<"\n\nList is empty ! DELETION NOT POSSIBLE !";
		Sleep(1000);
	}
	else
	{	
		dnode = hnode->link;
		pnode = pnode->link;
		hnode->link =pnode;
		delete dnode;
		cout<<"\n\n . . . NODE DELETED";
		Sleep(2000);
	}
}


//deletion at SPECIFIC POSITION of LIST
void LIST::delete_specific()
{	
	NODE *ptr = hnode;
	NODE *temp;
	int c=1;
	int p;
	int i;
	int val;
	while(1)
	{	
		if(hnode->link == NULL)
		{	
			cout<<"\n\nList is empty ! DELETION NOT POSSIBLE !";
			Sleep(1000);
			break;
		}
		else
		{	
			NODE *cptr;
			cptr = hnode->link;
			while(cptr->link)
			{	
				cptr = cptr->link;
				c++;
			}
			while(1)
			{	
				cout<<"\n\nEnter the position : ";
				cin>>p;
				temp = hnode->link;
				if(p == 1)
				{	
					delete_beg();
					break;
				}
				else if(p == 0)
				{	
					cout<<"\n !! POSITION CANNOT BE ZERO !!";
					Sleep(500);						
					continue;
				}
				else if(p>c)
				{	
					cout<<"\n !! POSITION NOT AVAILABLE !!";
					Sleep(500);
					continue;
				}
				else if(p==c)
				{	
					delete_end();
					break;
				}
				else
				{	
					for(i=0;i<p-1;i++)
					{	
						temp = temp->link;
						ptr = ptr->link;
					}
					ptr->link = temp->link;
					delete temp;
					cout<<"\n\n . . . NODE DELETED";
					Sleep(2000);
					break;
				}
			}
			break;
		}
	}
}


//deletion at END of LIST
void LIST::delete_end()
{	
	NODE *pnode = hnode;
	NODE *dnode = hnode->link;
	if(hnode->link == NULL)
	{	
		cout<<"\n\nList is empty ! DELETION NOT POSSIBLE !";
		Sleep(1000);
	}
	else
	{	
		while(dnode->link)
		{	
			dnode = dnode->link;
			pnode = pnode->link;
		}
		delete dnode;
		pnode->link = NULL;
		cout<<"\n\n . . . NODE DELETED";
		Sleep(2000);
	}
}




//displaying the LIST
void LIST::display()
{	
	NODE *pnode = hnode->link;
	if(pnode == NULL)
		cout<<"\n!! LIST IS EMPTY !!";
	else
	{	
		cout<<"\nLIST : ";
		while(pnode)
		{	
			cout<<pnode->info<<setw(4)<<"->";
			pnode = pnode->link;
		}
		cout<<" NULL"<<endl;
	}
}






//main
int main()
{	
	LIST a;
	int ch;
	cout<<"\nLIST CREATION \n";
	a.create();
	system("cls");
	cout<<"! LIST CREATION COMPLETED !";
	Sleep(2000);
	do
	{	
		system("cls");
		cout<<"\n############################";
		cout<<"\n#         MAIN MENU        #";
		cout<<"\n############################";
		cout<<"\n1.\t INSERTION";
		cout<<"\n2.\t DELETION";
		cout<<"\n3.\t DISPLAY";
		cout<<"\n4.\t EXIT";
		cout<<"\n\n  >Enter choice : ";
		cin>>ch;
		switch(ch)
		{	
			case 1:	int inso;
				do
				{	
					system("cls");
					cout<<"\n\t:::: INSERTION ::::";
					cout<<"\n1.    insertion at BEGINNING";
					cout<<"\n2.    insertion at SPECIFIC POSITION";
					cout<<"\n3.    insertion at END";
					cout<<"\n\n  >Enter choice : ";
					cin>>inso;
					if(inso == 1)
					{	
						system("cls");
						cout<<"\n  INSERT AT BEGINNING";
						a.insert_beg();
						break;
					}
					else if(inso == 2)
					{	
						system("cls");
						cout<<"\n  INSERT AT SPECIFIC POSITION";
						a.insert_specific();
						break;
					}	
					else if(inso == 3)
					{	
						system("cls");
						cout<<"\n  INSERT AT END";
						a.insert_end();
						break;
					}	
					else
					{	
						cout<<"\n! INVALID INPUT !";
						Sleep(500);
						cout<<"\n choose again !";
						Sleep(1000);
						continue;
					}
				}while(inso!=1||inso!=2||inso!=3);
			system("cls");
			break;
			
			case 2:	int delo;
				do
				{	system("cls");
					cout<<"\n\t:::: DELETION ::::";
					cout<<"\n1.    deletion at BEGINNING";
					cout<<"\n2.    deletion at SPECIFIC POSITION";
					cout<<"\n3.    deletion at END";
					cout<<"\n\n  >Enter choice : ";
					cin>>delo;
					if(delo == 1)
					{	
						system("cls");
						cout<<"\n  DELETION AT BEGINNING";
						a.delete_beg();
						break;
					}
					else if(delo == 2)
					{
						system("cls");
						cout<<"\n  DELETION AT SPECIFIC POSITION";
						a.delete_specific();
						break;
					}	
					else if(delo == 3)
					{	
						system("cls");
						cout<<"\n  DELETION AT END";
						a.delete_end();
						break;
					}	
					else
					{	
						cout<<"\n! INVALID INPUT !";
						Sleep(500);
						cout<<"\n choose again !";
						Sleep(1000);
						continue;
					}
				}while(delo!=1||delo!=2||delo!=3);
				system("cls");
				break;
			
			case 3:	system("cls");
				a.display();
				char ech;
				while(1)
				{	cout<<"\n\n>Execute MENU again ? (y/n)";
					cin>>ech;
					if(ech!='y' && ech!='n')
					{	cout<<"\n INVALID INPUT !";
						continue;
					}
					else
						break;
				}
				if(ech == 'y')
				{	system("cls");
					break;
				}
				else if(ech == 'n')
				{	cout<<"\n\n!! EXITING !!";
					exit(0);
				}
		
			case 4:	cout<<"\n!! EXITING !!";
				exit(0);
			
			default:cout<<"\n INVALID INPUT !!";
				Sleep(500);
				continue;
		}
	}while(ch!=0);
	return 0;
}
