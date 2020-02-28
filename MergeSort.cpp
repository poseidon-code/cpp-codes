#include<iostream>

using namespace std;

class MS
{
	private :	int *a;
			int size;
			int ub, lb;
	
	public :	MS(int n)
			{
				size = n;
				a = new int[n];
				input();
				ub = size;
				lb = 0;
				mergesort(a, lb, ub);
			};
		
			void input();
			void mergesort(int a[], int lb, int ub);
			void merge(int a[], int lb, int mid, int ub);
			void display();
};

void MS :: input()
{
	cout<<"Enter "<<size<<" elements : ";
	for(int i=0;i<size;i++)
		cin>>a[i];
		
	cout<<"\n: ARRAY : \n";
	for(int i=0;i<size;i++)
		cout<<a[i]<<" ";
}

void MS :: mergesort(int a[], int lb, int ub)
{
	if(lb<ub)
	{
		int mid = (ub+lb)/2;
		mergesort(a, lb, mid);
		mergesort(a, mid+1, ub);
		merge(a, lb, mid, ub);
	}
}

void MS :: merge(int a[], int lb, int mid, int ub)
{
	int *sa = new int[ub+1];
	int i=lb;
	int j=mid+1;
	int k=lb;
	while(i<=mid && j<=ub)
	{
		if(a[i]<a[j])
			sa[k++] = a[i++];
		else
			sa[k++] = a[j++];
	}
	while(i<=mid)
		sa[k++] = a[i++];
	while(j<=ub)
		sa[k++] = a[j++];
	for(int i=lb;i<=ub;i++)
		a[i] = sa[i];
}

void MS :: display()
{
	int s, i;
	if(size%2 == 0)
	{
		s = size;
		i = 0;
	}
	else
	{
		s = size + 1;
		i = 1;
	}
	cout<<"\n\n: SORTED ARRAY : \n";
	for(int j=i;j<s;j++)
		cout<<a[j]<<" ";
	cout<<"\n";
}

int main()
{
	int n;
	cout<<"Enter the size of the ARRAY :";
	cin>>n;
	MS a(n);
	a.display();
	return 0;
}
