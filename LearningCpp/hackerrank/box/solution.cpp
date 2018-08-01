#include <bits/stdc++.h>
#include <iostream>

using namespace std;
//Implement the class Box  
//l,b,h are integers representing the dimensions of the box

// The class should have the following functions : 

// Constructors: 
// Box();
// Box(int,int,int);
// Box(Box);


// int getLength(); // Return box's length
// int getBreadth (); // Return box's breadth
// int getHeight ();  //Return box's height
// long long CalculateVolume(); // Return the volume of the box

//Overload operator < as specified
//bool operator<(Box& b)

//Overload operator << as specified
//ostream& operator<<(ostream& out, Box& B)

class Box {
    private:
    int l, b, h;
    
    public:
    Box();
    Box(int l_in, int b_in, int h_in);
    Box(Box& box_in);
    int getLength();
    int getHeight();
    int getBreadth();
    long long CalculateVolume();
};
    Box::Box() {
        l = 0;
        b = 0;
        h = 0;
    }
    Box::Box(int l_in, int b_in, int h_in) {
        l = l_in;
        b = b_in;
        h = h_in;
    }
    
    Box::Box(Box& box_in) {
        l = box_in.getLength();
        b = box_in.getBreadth();
        h = box_in.getHeight();
    }
    
    int Box::getLength(){
        return l;
    }
    
    int Box::getHeight() {
        return h;
    }

    int Box::getBreadth() {
        return b;
    }
    
    long long Box::CalculateVolume() {
        long long out = (long long)l*h*b;
        return out;
    }
    
    bool operator<(Box& lhs, Box& rhs) {
        if (lhs.getLength() != rhs.getLength()){
            return lhs.getLength()<rhs.getLength();
        }
        else if (lhs.getBreadth() != rhs.getBreadth()) {
            return lhs.getBreadth() < rhs.getBreadth();
        }
        else {
            return lhs.getHeight() < rhs.getHeight();
        }
        return 0;
    }
    
    ostream& operator<<(ostream& os, Box& B) {
        // stringstream ss;
        os << B.getLength() << " " << B.getBreadth() << " " << B.getHeight();
        return os;
    }



void check2()
{
    int n;
    cin>>n;
    Box temp;
    for(int i=0;i<n;i++)
    {
        int type;
        cin>>type;
        if(type ==1)
        {
            cout<<temp<<endl;
        }
        if(type == 2)
        {
            int l,b,h;
            cin>>l>>b>>h;
            Box NewBox(l,b,h);
            temp=NewBox;
            cout<<temp<<endl;
        }
        if(type==3)
        {
            int l,b,h;
            cin>>l>>b>>h;
            Box NewBox(l,b,h);
            if(NewBox<temp)
            {
                cout<<"Lesser\n";
            }
            else
            {
                cout<<"Greater\n";
            }
        }
        if(type==4)
        {
            cout<<temp.CalculateVolume()<<endl;
        }
        if(type==5)
        {
            Box NewBox(temp);
            cout<<NewBox<<endl;
        }

    }
}

int main()
{
    check2();
}