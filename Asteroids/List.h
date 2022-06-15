#pragma once
#include <assert.h>

template<class T>
class LIST{
public:
		
private:
	
	struct Record{
		T*	data;
		Record* previous;
		Record* next;

		Record  (T* dat, Record* prev, Record* nxt) : data(dat), previous(prev), next(nxt) {};  
		Record  (const Record& org) : data(org.data), previous(org.previous), next(org.next) {};
		~Record () { delete data; previous = NULL; next = NULL; }
	};

	Record *pFirst;
	Record *pLast;
			
public:

						LIST()	: pFirst(NULL), pLast(NULL) {};  
	 bool				isempty();
	 void				insert(T* obj);
	 void				clear();
	
	 class iterator;
	 friend class iterator;
	 class iterator{
			Record* ptr; 
		public:
			iterator(const LIST<T>& lst) : ptr(lst.pFirst) {};
			iterator(const iterator& itr) : ptr(itr.ptr) {};
			iterator() : ptr(NULL) {};

			bool operator++() {
				if(ptr->next) {
					ptr = ptr->next;
					return true;
				} else { 
					ptr = NULL;
					return false;
				};
			};
			bool operator++(int) { return operator++(); }

			bool operator--() {
				if(ptr->previous) {
					ptr = ptr->previous;
					return true;
				} else { 
					ptr = NULL;
					return false;
				};
			};
			bool operator--(int) { return operator--(); }

			T* current() const {
				if(!ptr) return NULL;
				return ptr->data;
			};

			T* operator->() const {
				assert(ptr != 0);
				return current();
			};

			T* operator*() const { return current(); };

			operator bool() const { return bool(ptr); };

			bool operator==(const iterator&) const { return ptr==0;	};

			bool operator!=(const iterator&) const { return ptr!=0; };

			Record* GetPtr() {
				return ptr;
			}

	 };

	 iterator begin() const {
		 return iterator(*this);
	 };
	 iterator end() const {
		 return iterator();
	 };
	 void remove(iterator& toRemove);
};


template<class T>
bool LIST<T>::isempty(){
	if(pFirst == NULL && pLast == NULL) { return true; 
	} else { return false; };
};

template<class T>					//element na koniec listy
void LIST<T>::insert(T* obj) {
	if (isempty()) {
		pFirst = new Record(obj, NULL, NULL);
		pLast = pFirst;
	} else {
		pLast->next = new Record(obj, pLast, NULL);
		pLast = pLast->next;
	};
};

template<class T>
void LIST<T>::remove(iterator& toRemove)
{	 
	if((pFirst!=NULL) && (pLast!=NULL)) {
		 Record* tmp = toRemove.GetPtr();
		 if (tmp != NULL) {
			if( (tmp->previous!=NULL) && (tmp->next!=NULL) ){
				toRemove++;
				tmp->previous->next = tmp->next;
				tmp->next->previous = tmp->previous; }
			else if (tmp->previous==NULL && tmp->next!=NULL) {
				toRemove++;
				tmp->next->previous = NULL;
				pFirst = tmp->next; }
			else if (tmp->previous!=NULL && tmp->next==NULL) {
				toRemove--;
				tmp->previous->next = NULL;
				pLast = tmp->previous; }
			else if (tmp->previous==NULL && tmp->next==NULL) {
				pLast = pFirst = NULL;
				toRemove = iterator(); };

			delete tmp;
			};
	};
};

template<class T>					
void LIST<T>::clear() {

	if((pFirst!=NULL) && (pLast!=NULL)) {
	Record * tmp;
	while (pLast != NULL){
		tmp = pLast->previous;
		delete pLast;
		pLast = tmp;
	};
	pLast = pFirst = NULL;
	};
};