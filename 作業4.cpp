// HW4.cpp : 定義主控台應用程式的進入點。
//

//#include "stdafx.h"
#include <iostream>
using namespace std;

#define MAX_TERMS 10 /*size of terms array*/

typedef struct {
	int coef;
	int expo;
} polynomialTerm;

typedef struct linkedTerm {
	int coef;
	int expo;
	linkedTerm* nextTermPtr;
} linkedPolynomialTerm;

void clear(polynomialTerm t[])
{
	for (int i = 0; i < MAX_TERMS; i++) {
		t[i].coef = 0;
		t[i].expo = 0;
	}

	return;
}

void clear(linkedPolynomialTerm*& polynomialTermPtr)
{
	linkedPolynomialTerm* tmpPtr;

	while (polynomialTermPtr != nullptr) {
		tmpPtr = polynomialTermPtr;
		polynomialTermPtr = polynomialTermPtr->nextTermPtr;
		delete tmpPtr;
	}
}

void inputTerms(polynomialTerm terms[], int coef, int expo)
{
	// add your code here
	int input_posi=0;	
	if (coef == 0) {												//係數為0 則不存入 直接返回
		return;
	}
	if (terms[0].coef == 0 && terms[0].expo == 0) {					//若陣列沒有值 直接放第一個 放完返回
		terms[0].coef = coef;
		terms[0].expo = expo;
		return;
	}
	else {
		while (expo < terms[input_posi].expo) {
			input_posi++;
		}
		if (expo==terms[input_posi].expo) {							//若輸入的次方已存在 則直接覆蓋他的coef 並返回
			terms[input_posi].coef = coef;
			return;
		}
		for (int i = MAX_TERMS-2; i >= input_posi; i--) {			//往後移 PS.為避免資料被覆蓋，從最後面往後移 (terms[MAX_TERMS-2]移到terms[MAX_TERMS-1]，terms[MAX_TERMS-3]移到terms[MAX_TERMS-2].....)
			terms[i + 1] = terms[i];
		}		
		terms[input_posi].coef = coef;								//移動完後 terms[input_posi]裡的expo和coef應沒有值 直接存入即可
		terms[input_posi].expo = expo;
	}
	return;
}

void inputLinkTerms(linkedPolynomialTerm*& polyPtr, int coef, int expo)
{
	linkedPolynomialTerm* tmpPtr;

	tmpPtr = new linkedPolynomialTerm;
	tmpPtr->coef = coef;
	tmpPtr->expo = expo;
	// add your code here 
	linkedPolynomialTerm* pre_posi;
	linkedPolynomialTerm* cur_posi;	
	pre_posi =polyPtr;
	cur_posi =polyPtr;
	tmpPtr->nextTermPtr = nullptr;								//polyPtr->nextTermPtr原本沒指向任何東西(也沒指向nullptr) 所以無從判斷， 所一要在輸入第一個Link後 讓polyPtr->nextTermPtr指項nullptr;
	if (coef == 0) {
		return;
	}
	if (!polyPtr) {													//若polyPtr裡沒有值 則將已存好input值的tmpPtr存入polyPtr的第一個Link
		polyPtr = tmpPtr;		
		return;
	}
	else {
		while (cur_posi) {											//若cur_posi有值則繼續，若沒break;最後會停在沒有值的地方			
			if (expo >= cur_posi->expo) {							//若expo大於cur_posi的expo 就break讓cur_posi留在它之後要放入的位置
				break;
			}
			cur_posi= cur_posi->nextTermPtr;						
		}

		while (pre_posi) {											//找cur_posi的前一項pre_posi
			if (pre_posi->nextTermPtr==cur_posi) {
				break;       
			}
			pre_posi = pre_posi->nextTermPtr;
		}

		if (!cur_posi) {
			cur_posi = tmpPtr;
			pre_posi->nextTermPtr = tmpPtr;
			return;
		}
		if (expo == cur_posi->expo) {
			if(cur_posi==polyPtr){
                           polyPtr=tmpPtr；
                           return; 
                        }
                        cur_posi = tmpPtr;
			return;
		}

		if (cur_posi == polyPtr) {									//若輸入為最大次方，則將tmpPtr下一項指向cur_posi，P.S.不能直接指向polyPtr，因為polyPtr一定要是Linklist的頭
                        tmpPtr->nextTermPtr = cur_posi;
			polyPtr = tmpPtr;										//再將tmpPtr的位置改成首項
			return;													//直接回傳
		}

		pre_posi->nextTermPtr = tmpPtr;
		tmpPtr->nextTermPtr = cur_posi;
	}																																																				
	return;
}


void addArrayBasedPoly(polynomialTerm a[], polynomialTerm b[], polynomialTerm d[])
{
	// add your code here 
	int a_posi = 0;
	int b_posi = 0;
	int d_posi = 0;
	while (a[a_posi].coef!=0||a[a_posi].expo!=0||b[b_posi].coef!=0||b[b_posi].expo!=0) { //當a[a_posi].coef沒有值&&a[a_posi].expo沒有值&&b[b_posi].coef沒有值&&b[b_posi].expo沒有值 時停下，Demorgan後: 當a[a_posi].coef有值||a[a_posi].expo有值||b[b_posi].coef有值||b[b_posi].expo有值 時執行
		if (a[a_posi].expo > b[b_posi].expo) {											//當a[a_posi]的expo大於b[b_posi]的 將a的coef和expo存入d
			d[d_posi].coef = a[a_posi].coef;
			d[d_posi].expo = a[a_posi].expo;
			d_posi++;																	//存入後位置向後移，因為a和b陣列的expo都照順序排了，a[0]一定是最大次，a[1]第二大 以此類推
			a_posi++;
		}
		else if (a[a_posi].expo < b[b_posi].expo) {										////當b[b_posi]的expo大於a[a_posi]的 將b的coef和expo存入d
			d[d_posi].coef = b[b_posi].coef;
			d[d_posi].expo = b[b_posi].expo;
			d_posi++;
			b_posi++;
		}
		else if(a[a_posi].expo == b[b_posi].expo){										//若等於則將a b的coef相加
			if (a[a_posi].coef + b[b_posi].coef!=0) {									//但若相加後等於零則不存入
				d[d_posi].coef = a[a_posi].coef + b[b_posi].coef;
				d[d_posi].expo = a[a_posi].expo;
				a_posi++;
				b_posi++;
				d_posi++;
			}
			else {																		//不存入則a_posi和b_posi移動至下一位就好d_posi不用移動
				a_posi++;
				b_posi++;
			}
		}
	        if(a[a_posi].coef == 0 && a[a_posi].expo == 0&&d_posi!=MAX_TERMS){
                  while(b[b_posi].coef != 0 && b[b_posi].expo != 0 && d_posi != MAX_TERMS){
                     d[d_posi].coef = b[b_posi].coef;
	             d[d_posi].expo = b[b_posi].expo;
		     d_posi++;
	             b_posi++;
                  }
                }
                if(b[b_posi].coef == 0 && b[b_posi].expo == 0 && d_posi != MAX_TERMS){
                  while (a[a_posi].coef != 0 && a[a_posi].expo != 0 && d_posi != MAX_TERMS){
                    	d[d_posi].coef = a[a_posi].coef;
			d[d_posi].expo = a[a_posi].expo;
			d_posi++;
			a_posi++;
                  }
                 }
               
	}
	

	return;
}

linkedPolynomialTerm* addLinkBasedPoly(linkedPolynomialTerm* aPtr, linkedPolynomialTerm* bPtr)
{
	linkedPolynomialTerm* dPtr = nullptr;
	// add your code here



	return dPtr;
}

void printArrayBasedPoly(polynomialTerm terms[])
{
	if (terms[0].coef == 0)
		return;

	if (terms[0].expo == 0)
		cout << terms[0].coef;
	else
		cout << terms[0].coef << "X^" << terms[0].expo;

	for (int i = 1; i < MAX_TERMS; i++) {
		if (terms[i].coef == 0)
			return;

		if (terms[i].expo == 0)
			cout << " + " << terms[i].coef;
		else
			cout << " + " << terms[i].coef << "X^" << terms[i].expo;
	}
}

void printLinkBasedPoly(linkedPolynomialTerm* polyPtr)
{
	linkedPolynomialTerm* termPtr = polyPtr;

	if (termPtr == nullptr)
		return;

	if (termPtr->expo == 0)
		cout << termPtr->coef;
	else
		cout << termPtr->coef << "X^" << termPtr->expo;

	termPtr = termPtr->nextTermPtr;

	while (termPtr != nullptr) {
		if (termPtr->coef == 0)
			return;

		if (termPtr->expo == 0)
			cout << " + " << termPtr->coef;
		else
			cout << " + " << termPtr->coef << "X^" << termPtr->expo;

		termPtr = termPtr->nextTermPtr;
	}
}

int main()
{
	polynomialTerm a[MAX_TERMS], b[MAX_TERMS], d[MAX_TERMS];
	linkedPolynomialTerm* aPtr, * bPtr, * dPtr;

	int coef, expo;

	aPtr = bPtr = dPtr = nullptr;

	while (1) {
		clear(a); clear(b); clear(d);
		clear(aPtr); clear(bPtr); clear(dPtr);

		for (int i = 0; i < MAX_TERMS; i++) {
			cout << "\ninput a's coefficient and exponent: ";
			cin >> coef >> expo;

			if (expo == 0 && coef == 0)
				break;

			inputTerms(a, coef, expo);
			inputLinkTerms(aPtr, coef, expo);
		}

		cout << "\n\na = ";
		printArrayBasedPoly(a);
		cout << "\na = ";
		printLinkBasedPoly(aPtr);
		cout << "\n";

		for (int i = 0; i < MAX_TERMS; i++) {
			cout << "\ninput b's coefficient and exponent: ";
			cin >> coef >> expo;

			if (expo == 0 && coef == 0)
				break;

			inputTerms(b, coef, expo);
			inputLinkTerms(bPtr, coef, expo);
		}

		cout << "\n\nb = ";
		printArrayBasedPoly(b);

		cout << "\nb = ";
		printLinkBasedPoly(bPtr);

		cout << "\n";

		// d =a + b, where a, b, and d are polynomials

		addArrayBasedPoly(a, b, d);
		cout << "\na + b = ";
		printArrayBasedPoly(d);

		dPtr = addLinkBasedPoly(aPtr, bPtr);
		cout << "\na + b = ";
		printLinkBasedPoly(dPtr);

		cout << "\n";
	}
	return 0;
}
