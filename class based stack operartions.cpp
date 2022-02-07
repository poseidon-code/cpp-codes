#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#if defined(_WIN32) || defined(_WIN64)
	#include<windows.h>
#endif

using namespace std;


//STACK class
class STACK {	
	private:	
		class NODE {	
			public:	int info;
				NODE *link;
				NODE(int a=-1);
		};
		NODE *hnode;

	public:		
		STACK();
		~STACK();
		void push();
		void pop();
		void display();
};

void sleep(int time) {
	#if defined(_WIN32) || defined(_WIN64)
		Sleep(time);
	#else
		usleep(time*1000);
	#endif
}

void clear() {
	#if defined(_WIN32) || defined(_WIN64)
		system("cls");
	#else
		system("clear");
	#endif
}

//NODE : parameterised contructor
STACK::NODE::NODE(int a) {	
	info = a;
	link = NULL;
}

//STACK : constructor
STACK::STACK() { hnode = new NODE(); }

//STACK : destructor
STACK::~STACK() { delete hnode; }



//stack PUSH
void STACK::push()
{	
	NODE *q;
	NODE *pnode = hnode;
	int val;
	cout<<"\nEnter the value to be PUSHED: ";
	cin>>val;
	q = new NODE(val);
	q->link = pnode->link;
	pnode->link = q;
	cout<<"\n";
	for(int i=0;i<3;i++)
	{	
		cout<<". ";
		sleep(300);
	}
	cout<<" VALUE ENTERED";
	sleep(1000);
}



//stack POP
void STACK::pop()
{	
	NODE *pnode = hnode->link;
	NODE *dnode;
	if(hnode->link == NULL)
	{	
		cout<<"\n\nSTACK IS EMPTY ! DELETION NOT POSSIBLE !";
	}
	else
	{	
		dnode = hnode->link;
		pnode = pnode->link;
		hnode->link =pnode;
		delete dnode;
		cout<<"\n";
		for(int i=0;i<3;i++)
		{	
			cout<<". ";
			sleep(300);
		}
		cout<<" VALUE DELETED";
	}
	sleep(1000);
}



//stack DISPLAY
void STACK::display()
{	
	NODE *p;
	p = hnode->link;
	if(p == NULL)
		cout<<"\n STACK IS EMPTY !";
	else
	{	
		cout<<"\n STACK : ";
		while(p)
		{	
			cout<<p->info<<" <- ";
			p = p->link;
		}
		cout<<"BOTTOM";
	}
}






//main
int main()
{ 	
	STACK s;
	int ch;
	do
	{	
		clear();
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
			case 1:	s.push();
				break;
			
			case 2:	s.pop();
				break;
			
			case 3:	clear();
				s.display();
				char ech;
				while(1)
				{	cout<<"\n\n>Execute MENU again ? (y/n)";
					cin>>ech;
					if(ech!='y' && ech!='n')
					{	
						cout<<"\n INVALID INPUT !";
						continue;
					}
					else
						break;
				}
				if(ech == 'y')
				{	
					clear();
					break;
				}
				else if(ech == 'n')
				{	
					cout<<"\n\n!! EXITING !!";
					exit(0);
				}
			case 4:	cout<<"\n\nEXITING !!";
				exit(0);
			
			default:cout<<"\nINVALID INPUT !!";
				sleep(500);
				continue;
		}
	}while(ch!=0);
	return 0;
}
