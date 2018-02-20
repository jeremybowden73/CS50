/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

bool search(int value, int values[], int n)
{
    int x = n-1;        // the upper limit of the search range, initially the largest element
    int y = 0;          // the lower limit of the search range, initially the smallest element
    int mid = (x+y)/2;  // the middle element of the array
    
    // if n is non-positive return false
    if (n <=0)
    {
        return false;
    }
    
    // binary search loop, continues as long as number of elements in search array is > 0
    while (n>0)
        {
            if (value > values[mid])
            {
                // if the search value is greater than the middle value, move the lower limit of the next
                // search to the middle value + 1
                y = ((x + y)/2) +1;
                mid=(x+y)/2;
                n/=2;
            }
    
            else if (value < values[mid])
            {
                // if the search value is less than the middle value, move the upper limit of the next
                // search to the middle value - 1
                x = ((x + y)/2) -1;
                mid=(x+y)/2;
                n/=2;
            }
    
            else
            {
                // if the search value is the middle value, return true!
                return true;
            }
            
        }
        
        // if the search value is not found when n=0, return false
        return false;
    
}









/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // int i is defined as the element we are currently sorting
    // int j is defined as the element in the 'sorted' side that we are comparing i to, in order to put i in the right place
    // int shifters is the number of elements that shift 'up' the array when an insertion is needed
    // int temp is a temp location to store an element value that needs to be moved 'down' the array
    int shifters;
    int temp;
    
    //loop to check each element in turn, to see if it needs to move 'down' the array
    for (int i = 1 ; i < n ; i++)
    {
        // loop to check if i is smaller than the j'th element in the array.
        // If it is then i will need to be sorted 'down' the array to the j'th elements position
        // and all the other elements (the shifters) moved 'up'.
        for (int j=0 ; j < i ; j++)
        {
            if (values[i] < values[j])
            {
                temp = values[i];
                shifters = i-j;
                
                // loop to move each affected element 'up' 1 place in the array
                for (int k=0; k < shifters; k++)
                {
                    values[i-k] = values[i-k-1];
                }   
                                
                // put the value of the i'th element in the j'th position by using temp
                values[j] = temp;
                
                // if the element has moved the "for" loop needs to end
                // and we move on the the next i
                break;
            }
            
        }
        
    }
    
    // when the loop has reached the n'th element, return
    return;
    
}
