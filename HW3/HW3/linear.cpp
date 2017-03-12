//
//  linear.cpp
//  HW3
//
//  Created by Kevin on 2/12/17.
//  Copyright © 2017 Kevin. All rights reserved.
//


// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if(n <= 0)
        return false;
    if(n==1)
        return somePredicate(a[0]);
    else
        return (somePredicate(a[0]) and allTrue(a+1, n-1));
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if(n <= 0)
        return 0;
    if(somePredicate(a[0]) == false)
        return 1 + countFalse(a+1, n-1);
    else
        return countFalse(a+1, n-1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if(n <= 0)
        return -1;
    if(somePredicate(a[0]) == false)
        return 0;
    else
        if(firstFalse(a+1, n-1) == -1)
            return -1;
        else
            return 1 + firstFalse(a+1, n-1);
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if(n<=0)
        return -1;
    int i = 1 + indexOfMin(a+1, n-1);
    if(a[0] < a[i])
        return 0;
    else
        return i;
    
        
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 <= 0)
        return true;
    else if(n1 <= 0 and n2 > 0)
        return false;
    if(a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);
    else
        return includes(a1+1, n1-1, a2, n2);
}
