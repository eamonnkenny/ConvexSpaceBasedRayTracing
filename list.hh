//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 19th 2001
// Previous Projects:     Storms, FMW/ARG
// Modifications:         Dated as "Added dd/mm/yy" below
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef TCD_LIST_HH
#define TCD_LIST_HH

#include <iostream>
#include <fstream>

#include "item.hh"

#ifdef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class 
#else
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport)
#endif

template<class Type>
EXPORT_CLASS_DECLSPEC List
//=========================================================================
// = FUNCTION
//     list.hh
//
// = AUTHOR
//     Liam O'Brien, Eamonn M. Kenny
//
// = DESCRIPTION
//     This class is used for dynamically allocating memory to a linked
//     list
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-1999 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
   public:
     TCDListItem<Type> *front, *back, *previous_;
     int _size;
     int previousCount_;
     
     List() 
     {  
       front = NULL; 
       back  = NULL; 
       _size = 0; 
     }

     ~List(){ Destroy() ; }
     //
     // destructor
     //

     /*List( const List<Type>& L1 )
     {
       //cout << " Copy constructing list " << endl;
       front = 0;
       back = 0;
       _size = 0;
       clear();
       if ( this == NULL )
         cerr << " this pointer is NULL " << endl;
       copy( L1 );
     }*/

     void insert_before( const Type& t, int i );
     //
     // insert before TCDListItem i an TCDListItem of type t.
     //

     void append( const Type& t);
     //
     // insert at end of list
     //

     void appendNewItem( const Type& t);
     //
     // append at end of list after checking that the new
     // TCDListItem is not already on the list
     //

     void push( const Type& t ) { insert_before( t, 0 ); }
     //
     // places t on front of list
     //

     void insert( const Type& t, int i );
     //
     // insert after TCDListItem i;
     //

     void swap( int i, const Type& t );
     //
     // this swaps t with the i-th TCDListItem on the list
     // It is equivalent to inserting after i and then
     // deleting the i-th TCDListItem 
     //

     void Transfer( List<Type>& L1 );
     //
     // This copies L1 into the present list. NOT utilised yet
     //

     void Delete( int i );
     //
     // Removes the i-th element
     //

     Type GetAndDelete( int i );
     //
     //Type& GetAndDelete( int i );
     //
     // Returns the value of the i-th TCDListItem after deleting it.
     //

     void clearAndClearSubList();
     //
     // remove all elements of the current list and elements of the sublist
     //

     void clear();
     //
     // Removes all the elements.
     //

     Type pop(){ return( GetAndDelete( 0 ) );}
     //Type& pop(){ return( GetAndDelete( 0 ) );}
     //
     // Returns the value of the first TCDListItem after deleting it.
     //

     Type Pop(){ return( GetAndDelete( _size-1 ) ); }
     //Type& Pop(){ return( GetAndDelete( _size-1 ) ); }
     //
     // Returns the value of the last TCDListItem after deleting it.
     //

     int size() const { return _size; }
     //
     // returns the number of elements on the list
     //

     boolean empty();
     //
     // Returns true if list is empty, otherwise false
     //

     void Destroy();
     //
     // 
     void PointerToFront() { cout << " front = " << front << endl; }
     //
     // Prints the value of the pointer to front
     //

     Type& GetItem( int i ) ;
     //
     // Returns the value of the i-th
     //

     //Type& GetItem( int i ) const ;
     //
     // Returns the value of the i-th. Overloading of the other method
     //  for const argument
     //

     Type& first();
     //
     // Returns the value of the first TCDListItem
     //

     Type sum();
     //
     // Returns the sum of all values in the list (useful for complex/doubles)
     //

     Type& last()  const;
     //
     // Returns the value of the last TCDListItem
     //

     void copy( const List<Type>& L1 );
     //
     //  copies list L1 to the present List
     //

     List<Type>& operator=( const List<Type>& L1 )
     {
       // = operator
       clear();
       if ( this == NULL )
         cerr << " this pointer is NULL " << endl;
       copy( L1 );

       return (*this) ;
     }
};

/*template<class Type>
List<Type>& List<Type>::operator=( const List<Type>& L1 )
{
  List<Type> *NewList;
  int        i;

  NewList = new List<Type>;

  for ( i = 0; i < L1._size; i++ )
  {
    cerr << "Got to here" << endl;
    NewList->append( L1.GetItem(i));
  }
  cerr << "NewList size= " << NewList->_size << endl;
  return( *NewList );
}*/

template<class Type>
void List<Type>::append( const Type& T )
{
  //cerr << " Start append " << endl;
  TCDListItem<Type> *pt = new TCDListItem<Type>( T );
  if ( !pt )
  {
    cerr << " append: error 1" << endl;
    exit(0);
  }
  if ( !front )
  {
    front = pt;
    front->next = NULL;
  }
  else
  {
    if ( !back )
      cout << " append: error 2 " << endl;
    back->next = pt;
    if ( _size == 1 )
      front->next = pt;
  }
  back = pt;
  back->next = NULL;
  _size++;
  if ( front->next != NULL )
    if ( ((front->next)->next) == NULL && _size == 3 )
  {
    cout  << _size << endl;
    cout  << front << endl;
  }
  previous_ = front; // Added 22/11/01
  previousCount_ = 0;
}

template<class Type>
void List<Type>::appendNewItem( const Type& T1 )
{
  TCDListItem<Type> *pt = new TCDListItem<Type>( T1 );
  if ( !front )
  {
    append( T1 );
    return;
  }
  if ( !back )
    cout << " error " << endl;
  Type T_on_list = front->T;
  if ( !pt )
  {
    cout << " appendNewItem: error 1 " << endl;
    exit(0);
  }
  int SIZE = _size;
  int k = 0;
  TCDListItem<Type> *current = front, *next = front;
  next = front->next;

  while ( k < _size && _size > 1 )
  {
    if ( !current )
      cout << " appendNewItem: error 2" << endl;
    T_on_list = current->T;
    if ( T_on_list == T1 )
    {
      delete pt;
      return;
    }
    current = next;
	if ( k+1 < _size )
	  next = current->next;
    k++;
  }
  //current->next = pt;
  //pt->next = next;
  if ( !front )
  {
    front = pt;
  }
  else
  {
    back->next = pt;
    if ( _size == 1 )
      front->next = pt;
  }
  back = pt;
  back->next = NULL;
  _size++;
  previous_ = front; // Added 22/11/01
  previousCount_ = 0;
}

template<class Type>
void List<Type>::Transfer( List<Type>& L1 )
{
  clear();
  _size = L1._size ;
  TCDListItem<Type> *current = L1.front, old_current;
  while ( current )
  {
    append( current->T );
    old_current = current;
    current = current->next;
    delete old_current;
  }
}

template<class Type>
boolean List<Type>::empty()
{
  if ( _size == 0 && front == 0 )
    return true;
  return false;
}

template<class Type>
Type& List<Type>::GetItem( int i )
{
  if ( i >= _size )
  {
    cout << " GetItem: List exceeds limit: " << _size << endl;
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next;
  //Type *tptr = new Type;
  if ( !front )
  {
    cout << " GetItem: error 1" << endl;
    exit(0);
  }

  // Added 22/11/01 previous counter to speed up search
  // found that for raytracing this could speed computational time by 33%
  if ( previousCount_ == i )
    return (previous_->T);

  if ( previousCount_ == i-1 )
  {
    previousCount_++;
    previous_ = previous_->next;
    return (previous_->T);
  }

  // we can start search at item previousCount_ instead of item 0
  if ( previousCount_ < i )
  {
    k       = previousCount_;
    current = previous_;
  } 

  next = current->next;

  while ( k < i )
  {
    // current is address of i TCDListItem on exit of loop
    // next is address of i+1 TCDListItem
    current = next;
    next    = current->next;
    k++;
  }
  previousCount_ = i;
  previous_      = current;

  return (current->T);
}

template<class Type>
Type& List<Type>::first()
{
  if ( !front )
  {
    cout << " first: error " << endl;
    exit(0);
  }

  return (front->T);
}

template<class Type>
Type& List<Type>::last() const
{
  if ( !back )
  {
    cout << " last: error " << endl;
    exit(0);
  }
  return (back->T);
}

/*template<class Type>
Type& List<Type>::GetItem( int i ) const
{
  if ( !front )
  {
    cout << " GetItem: error " << endl;
    exit(0);
  }
  int SIZE = _size;
  if ( i >= _size )
  {
    cout << " GetItem: List exceeds limit: " << _size << endl;
    exit(0);
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next = front;
  if ( previousCount_ == i-1 )
  {
    previousCount_++;
    previous_ = previous_->next;
    return (previous_->T);
  }
  next = front->next;
  while ( k < i )
  {
    current = next;
    // current is address of i TCDListItem
    next    = current->next;
    // next is address of i+1 TCDListItem
    k++;
  }
  if ( !current )
    cout << " error " << endl;
  return (current->T);
}*/

template<class Type>
void List<Type>::copy( const List<Type>& L1 )
{
  TCDListItem<Type> *current = L1.front;
  if ( !current || L1._size == 0 )
  {
    _size = 0;
    return;
  }
  if ( L1._size == 0 )
    cout << " size = " << (L1._size) << endl;
  while ( current )
  {
    append( current->T );
    current = current->next;
  }
  _size          = L1._size;
  previous_      = front;
  previousCount_ = 0;
}

template<class Type>
Type List<Type>::sum()
{
  Type sum = 0;

  for ( int i = 0; i < _size; i++ )
  {
    sum = sum + GetItem(i);
  }
  return sum;
}

template<class Type>
void List<Type>::insert( const Type& T, int i )
{
  // insert AFTER i
  TCDListItem<Type> *pt = new TCDListItem<Type>( T );
  if ( i >= _size )
  {
    cout << " insert:  exceeds limit: " << _size << endl;
    return;
  }
  if ( !pt )
  {
    cout << " insert:  error " << endl;
    exit(0);
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next = front;
  next = front->next;
  while ( k < i )
  {
    current = next;
    next    = current->next;
    k++;
  }
  current->next = pt;
  pt->next = next;
  if ( _size == i+1 )
    back = pt ;
  ++_size;

  previous_ = front; // Added 22/11/01
  previousCount_ = 0; 
}

template<class Type>
void List<Type>::insert_before( const Type& T, int i )
{
  // insert before i
  TCDListItem<Type> *pt = new TCDListItem<Type>( T );

  if ( !pt )
  {
    cout << " insert before:  error " << endl;
    exit(1);
  }
  if ( i > _size )
  {
    cout << " insert before:  exceeds limit: " << _size << endl;
    delete pt;
    return;
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next = front;
  if ( front == 0 || _size == 0 )
  {
    front = pt;
    back = pt;
    pt->next = NULL;
    _size++;
    previousCount_ = 0;
    previous_      = front;
    return;
  }
  if ( i == 0 )
  {
    front = pt;
    pt->next = current;
    _size++;
    previousCount_ = 0;
    previous_      = front;
    return;
  }

  next = front->next;
  while ( k < i-1 )
  {
    current = next;
    next    = current->next;
    k++;
  }
  current->next = pt;
  pt->next = next;
  ++_size;
  previousCount_ = 0;
  previous_      = front;
}

template<class Type>
void List<Type>::Delete( int i )
{
  //  delete i
  previousCount_ = 0;
  if ( i >= _size )
  {
    cout << " Delete: List exceeds limit: " << _size << endl;
    previous_ = front;
    return ;
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next = front, *previous = front;
  if ( i == 0 )
  {
    next     = front->next;
    front    = next;
    delete current;
    --_size;
    if ( _size == 0 )
    {
      front = 0;
      back  = 0;
    }
    previous_ = front;
    return ;
  }
  else if ( i == 1 )
  {
    current     = front->next;
    next        = current->next;
    front->next = next;
    delete current;
    if ( _size == i+1 )
      back = front ;
    --_size;
    previous_ = front;
    return ;
  }
  next = front->next;
  while ( k < i )
  {
    if ( k == i-2 )
      previous = current->next;
    // previous is address of i-1 TCDListItem
    current = next;
    // current is address of i TCDListItem
    next    = current->next;
    // next is address of i+1 TCDListItem
    k++;
  }
  previous->next = next;
  // pointer of i-1 points to i+1
  delete current;
  if ( _size == i+1 )
    back = previous ;
  --_size;
  previous_ = front;
  return ;
}

template<class Type>
Type List<Type>::GetAndDelete( int i )
{
  previousCount_ = 0;
  if ( !front )
  {
    cout << " GetandDelete: error " << endl;
    exit(0);
  }
  //  return TCDListItem i and then delete it
  if ( i >= _size )
  {
    cout << " GetandDelete: exceeds limit: " << _size << endl;
    exit(1);
  }
  int k = 0;
  TCDListItem<Type> *current = front, *next = front, *previous = front;
  Type T1;
  if ( i == 0 )
  {
    next     = front->next;
    front    = next;
    T1 = (current->T);
    delete current;
    --_size;
    if ( _size == 0 )
    {
      front = 0;
      back  = 0;
    }
    previous_ = front;
    return (T1);
  }
  else if ( i == 1 )
  {
    current     = front->next;
    //(*tptr) = (current->T);
    T1 = (current->T);
    next        = current->next;
    front->next = next;
    delete current;
    if ( _size == i+1 )
      back = front ;
    --_size;
    //return (*tptr);
    previous_ = front;
    return (T1);
  }
  next = front->next;
  while ( k < i )
  {
    if ( k == i-2 )
      previous = current->next;
    // previous is address of i-1 TCDListItem
    current = next;
    // current is address of i TCDListItem
    next    = current->next;
    // next is address of i+1 TCDListItem
    k++;
  }
  previous->next = next;
  // pointer of i-1 points to i+1
  //tptr = &(current->T);
  T1 = (current->T);
  //(*tptr) = (current->T);
  delete current;
  if ( _size == i+1 )
    back = previous ;
  --_size;
  //return (*tptr);
  previous_ = front;
  return (T1);
}

template<class Type>
void List<Type>::swap( int i, const Type& T )
{
  int SIZE = _size;

  previousCount_ = 0;
  if ( i >= _size )
  {
    cout << "Swap: List exceeds limit: " << _size << endl;
    return;
  }
  TCDListItem<Type> *pt = new TCDListItem<Type>( T );
  if ( !pt )
  {
    cout << " swap: error " << endl;
    exit(0);
  }
  TCDListItem<Type> *current = front, *next = front, *previous = front;
  if ( i == 0 )
  {
    next     = front->next;
    front    = pt;
    pt->next = next;
    delete current;
    if ( _size == 1 )
      back = pt;
    previous_ = front;
    return;
  }
  else if ( i == 1 )
  {
    current     = front->next;
    next        = current->next;
    front->next = pt;
    pt->next    = next;
    delete current;
    if ( _size == i+1 )
      back = pt;
    previous_ = front;
    return;
  }
  int k = 0;
  next = front->next;
  while ( k < i )
  {
    if ( k == i-2 )
      previous = current->next;
    // previous is address of i-1 TCDListItem when k == i-1
    current = next;
    // current is address of i TCDListItem when k == i-1
    next    = current->next;
    // next is address of i+1 TCDListItem when k == i-1
    k++;
  }
  delete current;
  previous->next = pt;
  // pointer of i-1 points to pt
  pt->next       = next;
  // pointer of pt points to i+1
  if ( _size == i+1 )
    back = pt;
  previous_ = front;
}

/*template<class Type>
void List<Type>::clear()
{
  TCDListItem<Type> *current = front, *pt = current;
  if ( _size == 0 )
  {
   // cout << " finish of 0-list clearence " << endl;
    front = 0;
    back  = 0;
    if ( this == NULL )
      cout << " Null This " << endl;
    return;
  }
  while ( current )
  {
    pt = current->next;
    delete current;
    current = pt;
  }
  _size = 0;
  front = 0;
  back  = 0;
  if ( this == NULL )
    cout << " Null This " << endl;
}*/

template<class Type>
void List<Type>::clear()
{
  TCDListItem<Type> *current = front, *next = front;

  if ( _size == 0 )
  {
   // cout << " finish of 0-list clearence " << endl;
    front = 0;
    back = 0;
    if ( this == NULL )
      cout << " Null This " << endl;
    return;
  }

  while ( current )
  {
    next    = current->next;
    delete  current;
    current = next;
    --_size;
  }

  _size = 0;
  front = 0;
  previous_ = 0;
  back = 0;
  if ( this == NULL )
    cout << " Null This " << endl;
}

template<class Type>
void List<Type>::clearAndClearSubList()
{
  TCDListItem<Type> *current = front, *next = front;

  if ( _size == 0 )
  {
   // cout << " finish of 0-list clearence " << endl;
    front = 0;
    back = 0;
    if ( this == NULL )
      cout << " Null This " << endl;
    return;
  }


  while ( current )
  {
    next    = current->next;
    (current->T).clear();
    delete current;
    current = next;
    --_size;
  }

  _size = 0;
  front = 0;
  previous_ = 0;
  back  = 0;
  if ( this == NULL )
    cout << " Null This " << endl;
}

template<class Type>
void List<Type>::Destroy()
{
  //cout << " Start of Destroy() List " << endl;
  TCDListItem<Type> *ptr = front, *ptr_1;
  while ( ptr )
  {
    ptr_1 = ptr;
    ptr = ptr->next;
    delete ptr_1;
  }
  front = 0;
  back  = 0;
  //cout << " Finish of Destroy() List " << endl;
}

#endif
